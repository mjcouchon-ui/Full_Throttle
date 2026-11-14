#include "FTEditorTools.h"
#include "Modules/ModuleManager.h"
#include "UObject/UnrealType.h"
#include "UObject/Package.h"
#include "NiagaraSystem.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "GeometryCollection/GeometryCollectionObject.h"
#include "GeometryCollection/GeometryCollection.h"
#include "GeometryCollection/GeometryCollectionEngineConversion.h"
#include "FractureEngineFracturing.h"
#include "Dataflow/DataflowSelection.h"
#include "AssetRegistry/AssetRegistryModule.h"

IMPLEMENT_MODULE(FDefaultModuleImpl, FullThrottleEditor);

namespace
{
	/** Walk a dotted path with [i] indices. Returns the leaf property and the memory that holds it, plus the UObject that owns that memory. */
	bool Resolve(UObject* Root, const FString& Path, FProperty*& OutProp, void*& OutContainer, UObject*& OutOwner, FString& Err)
	{
		OutOwner = Root;
		UStruct* Struct = Root->GetClass();
		void* Container = Root;
		TArray<FString> Parts;
		Path.ParseIntoArray(Parts, TEXT("."), true);
		for (int32 p = 0; p < Parts.Num(); ++p)
		{
			FString Name = Parts[p];
			int32 Index = INDEX_NONE;
			int32 Br;
			if (Name.FindChar('[', Br))
			{
				Index = FCString::Atoi(*Name.Mid(Br + 1));
				Name = Name.Left(Br);
			}
			FProperty* Prop = FindFProperty<FProperty>(Struct, *Name);
			if (!Prop) { Err = FString::Printf(TEXT("no property '%s' on %s"), *Name, *Struct->GetName()); return false; }
			void* ValuePtr = Prop->ContainerPtrToValuePtr<void>(Container);
			FProperty* Inner = Prop;
			if (Index != INDEX_NONE)
			{
				if (FArrayProperty* AP = CastField<FArrayProperty>(Prop))
				{
					FScriptArrayHelper H(AP, ValuePtr);
					if (!H.IsValidIndex(Index)) { Err = FString::Printf(TEXT("index %d out of range for %s (%d)"), Index, *Name, H.Num()); return false; }
					ValuePtr = H.GetRawPtr(Index);
					Inner = AP->Inner;
				}
				else { Err = Name + TEXT(" is not an array"); return false; }
			}
			const bool bLast = p == Parts.Num() - 1;
			if (bLast)
			{
				if (Index != INDEX_NONE)
				{
					// leaf is an array element: present it as a property with offset 0 over the element memory
					OutProp = Inner; OutContainer = (uint8*)ValuePtr - Inner->GetOffset_ForInternal();
				}
				else { OutProp = Prop; OutContainer = Container; }
				return true;
			}
			if (FObjectPropertyBase* OP = CastField<FObjectPropertyBase>(Inner))
			{
				UObject* Obj = OP->GetObjectPropertyValue(ValuePtr);
				if (!Obj) { Err = FString::Printf(TEXT("'%s' is null"), *Parts[p]); return false; }
				OutOwner = Obj; Struct = Obj->GetClass(); Container = Obj;
			}
			else if (FStructProperty* SP = CastField<FStructProperty>(Inner))
			{
				Struct = SP->Struct; Container = ValuePtr;
			}
			else { Err = FString::Printf(TEXT("can't descend into '%s'"), *Parts[p]); return false; }
		}
		Err = TEXT("empty path");
		return false;
	}

	void Dump(UObject* Obj, int32 Depth, int32 MaxDepth, FString& Out, TSet<UObject*>& Seen)
	{
		if (!Obj || Seen.Contains(Obj) || Depth > MaxDepth) return;
		Seen.Add(Obj);
		const FString Ind = FString::ChrN(Depth * 2, ' ');
		Out += FString::Printf(TEXT("%s[%s] %s\n"), *Ind, *Obj->GetClass()->GetName(), *Obj->GetName());
		for (TFieldIterator<FProperty> It(Obj->GetClass()); It; ++It)
		{
			FProperty* P = *It;
			if (!P->HasAnyPropertyFlags(CPF_Edit) && !P->HasAnyPropertyFlags(CPF_InstancedReference | CPF_ContainsInstancedReference)) continue;
			FString V;
			P->ExportTextItem_Direct(V, P->ContainerPtrToValuePtr<void>(Obj), nullptr, Obj, PPF_None);
			if (V.Len() > 300) V = V.Left(300) + TEXT("...");
			Out += FString::Printf(TEXT("%s  .%s = %s\n"), *Ind, *P->GetName(), *V);
			auto Visit = [&](FProperty* Pr, void* Ptr)
			{
				if (FObjectPropertyBase* OP = CastField<FObjectPropertyBase>(Pr))
				{
					UObject* Sub = OP->GetObjectPropertyValue(Ptr);
					if (Sub && Sub->IsIn(Obj->GetOutermost()) && !Sub->IsA<UPackage>()) Dump(Sub, Depth + 2, MaxDepth, Out, Seen);
				}
			};
			if (FArrayProperty* AP = CastField<FArrayProperty>(P))
			{
				FScriptArrayHelper H(AP, AP->ContainerPtrToValuePtr<void>(Obj));
				for (int32 i = 0; i < H.Num(); ++i)
				{
					if (FStructProperty* SP = CastField<FStructProperty>(AP->Inner))
					{
						for (TFieldIterator<FProperty> SIt(SP->Struct); SIt; ++SIt) Visit(*SIt, SIt->ContainerPtrToValuePtr<void>(H.GetRawPtr(i)));
					}
					else Visit(AP->Inner, H.GetRawPtr(i));
				}
			}
			else Visit(P, P->ContainerPtrToValuePtr<void>(Obj));
		}
	}

	void Touch(UObject* Owner, FProperty* Prop)
	{
		Owner->Modify();
		FPropertyChangedEvent Ev(Prop, EPropertyChangeType::ValueSet);
		Owner->PostEditChangeProperty(Ev);
		// let every outer (emitter -> system) know too
		for (UObject* O = Owner->GetOuter(); O && !O->IsA<UPackage>(); O = O->GetOuter())
		{
			FPropertyChangedEvent Up(nullptr, EPropertyChangeType::ValueSet);
			O->PostEditChangeProperty(Up);
		}
		Owner->MarkPackageDirty();
	}
}

FString UFTEditorTools::DumpObject(UObject* Root, int32 MaxDepth)
{
	FString Out; TSet<UObject*> Seen;
	if (Root)
	{
		Dump(Root, 0, MaxDepth, Out, Seen);
		// struct arrays on the root (e.g. EmitterHandles) are walked one level
		for (TFieldIterator<FProperty> It(Root->GetClass()); It; ++It)
		{
			if (FArrayProperty* AP = CastField<FArrayProperty>(*It))
			{
				if (FStructProperty* SP = CastField<FStructProperty>(AP->Inner))
				{
					FScriptArrayHelper H(AP, AP->ContainerPtrToValuePtr<void>(Root));
					for (int32 i = 0; i < H.Num(); ++i)
						for (TFieldIterator<FProperty> SIt(SP->Struct); SIt; ++SIt)
							if (FObjectPropertyBase* OP = CastField<FObjectPropertyBase>(*SIt))
								if (UObject* Sub = OP->GetObjectPropertyValue(SIt->ContainerPtrToValuePtr<void>(H.GetRawPtr(i))))
								{
									Out += FString::Printf(TEXT("== %s[%d].%s\n"), *AP->GetName(), i, *SIt->GetName());
									Dump(Sub, 1, MaxDepth, Out, Seen);
								}
				}
			}
		}
	}
	return Out;
}

bool UFTEditorTools::SetPath(UObject* Root, const FString& Path, const FString& Value, FString& Error)
{
	if (!Root) { Error = TEXT("null root"); return false; }
	FProperty* Prop = nullptr; void* Container = nullptr; UObject* Owner = nullptr;
	if (!Resolve(Root, Path, Prop, Container, Owner, Error)) return false;
	Owner->PreEditChange(Prop);
	const TCHAR* Res = Prop->ImportText_InContainer(*Value, Container, Owner, PPF_None);
	if (!Res) { Error = FString::Printf(TEXT("could not parse '%s' for %s"), *Value, *Prop->GetCPPType()); return false; }
	Touch(Owner, Prop);
	return true;
}

FString UFTEditorTools::GetPath(UObject* Root, const FString& Path)
{
	FProperty* Prop = nullptr; void* Container = nullptr; UObject* Owner = nullptr; FString Err;
	if (!Root || !Resolve(Root, Path, Prop, Container, Owner, Err)) return TEXT("ERROR: ") + Err;
	FString V;
	Prop->ExportTextItem_InContainer(V, Container, nullptr, Owner, PPF_None);
	return V;
}

int32 UFTEditorTools::AddInstanced(UObject* Root, const FString& ArrayPath, const FString& ClassPath, FString& Error)
{
	FProperty* Prop = nullptr; void* Container = nullptr; UObject* Owner = nullptr;
	if (!Root || !Resolve(Root, ArrayPath, Prop, Container, Owner, Error)) return -1;
	FArrayProperty* AP = CastField<FArrayProperty>(Prop);
	FObjectPropertyBase* OP = AP ? CastField<FObjectPropertyBase>(AP->Inner) : nullptr;
	if (!OP) { Error = TEXT("not an object array"); return -1; }
	UClass* Cls = LoadObject<UClass>(nullptr, *ClassPath);
	if (!Cls) { Error = TEXT("no class ") + ClassPath; return -1; }
	Owner->PreEditChange(AP);
	UObject* NewObj = NewObject<UObject>(Owner, Cls, NAME_None, RF_Transactional | RF_Public);
	FScriptArrayHelper H(AP, AP->ContainerPtrToValuePtr<void>(Container));
	const int32 Idx = H.AddValue();
	OP->SetObjectPropertyValue(H.GetRawPtr(Idx), NewObj);
	Touch(Owner, AP);
	return Idx;
}

bool UFTEditorTools::RemoveAt(UObject* Root, const FString& ArrayPath, int32 Index)
{
	FProperty* Prop = nullptr; void* Container = nullptr; UObject* Owner = nullptr; FString Err;
	if (!Root || !Resolve(Root, ArrayPath, Prop, Container, Owner, Err)) return false;
	FArrayProperty* AP = CastField<FArrayProperty>(Prop);
	if (!AP) return false;
	Owner->PreEditChange(AP);
	FScriptArrayHelper H(AP, AP->ContainerPtrToValuePtr<void>(Container));
	if (!H.IsValidIndex(Index)) return false;
	H.RemoveValues(Index, 1);
	Touch(Owner, AP);
	return true;
}

void UFTEditorTools::CompileNiagara(UNiagaraSystem* System)
{
	if (!System) return;
	System->Modify();
	FPropertyChangedEvent Ev(nullptr);
	System->PostEditChangeProperty(Ev);
	System->RequestCompile(true);
	System->WaitForCompilationComplete(true, false);
	System->MarkPackageDirty();
}

UGeometryCollection* UFTEditorTools::MakeFractured(UStaticMesh* Mesh, const FString& PackagePath, const FString& Name, int32 Sites, int32 Seed)
{
	if (!Mesh) return nullptr;
	const FString PkgName = PackagePath / Name;
	UPackage* Pkg = CreatePackage(*PkgName);
	UGeometryCollection* GC = NewObject<UGeometryCollection>(Pkg, *Name, RF_Public | RF_Standalone | RF_Transactional);
	TArray<UMaterialInterface*> Mats;
	for (const FStaticMaterial& M : Mesh->GetStaticMaterials()) Mats.Add(M.MaterialInterface);
	FGeometryCollectionEngineConversion::AppendStaticMesh(Mesh, Mats, FTransform::Identity, GC, true, true, false, false);
	TSharedPtr<FGeometryCollection, ESPMode::ThreadSafe> Coll = GC->GetGeometryCollection();
	if (!Coll.IsValid()) return nullptr;

	FDataflowTransformSelection Sel;
	Sel.InitializeFromCollection(*Coll, false);
	Sel.SetSelected(0);
	const FBox B = Mesh->GetBoundingBox();
	FRandomStream R(Seed);
	TArray<FVector> Pts;
	for (int32 i = 0; i < FMath::Max(2, Sites); ++i) Pts.Add(FVector(R.FRandRange(B.Min.X, B.Max.X), R.FRandRange(B.Min.Y, B.Max.Y), R.FRandRange(B.Min.Z, B.Max.Z)));
	FFractureEngineFracturing::VoronoiFracture(*Coll, Sel, Pts, FTransform::Identity, Seed, 1.f, FIslandSplitSettings(), 0.f, 0.f, 0.1f, 0.5f, 2.f, 1, 10.f, false, 50.f);

	GC->InitializeMaterials();
	GC->InvalidateCollection();
	GC->CreateSimulationData();
	GC->RebuildRenderData();
	GC->MarkPackageDirty();
	FAssetRegistryModule::AssetCreated(GC);
	return GC;
}
