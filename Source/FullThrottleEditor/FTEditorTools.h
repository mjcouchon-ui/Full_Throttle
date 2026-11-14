#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FTEditorTools.generated.h"

class UNiagaraSystem;
class UStaticMesh;
class UGeometryCollection;

/** Called from the editor's Python (over the Unreal MCP connector) to author assets Python can't reach. */
UCLASS()
class FULLTHROTTLEEDITOR_API UFTEditorTools : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/** Dump an object's editable properties (recursing into instanced sub-objects) as text. */
	UFUNCTION(BlueprintCallable, Category = "FT Tools")
	static FString DumpObject(UObject* Root, int32 MaxDepth = 4);

	/** Set a property by path, e.g. "EmitterHandles[0].StatelessEmitter.Modules[1].LifetimeRange.Min". Value is UE text format. */
	UFUNCTION(BlueprintCallable, Category = "FT Tools")
	static bool SetPath(UObject* Root, const FString& Path, const FString& Value, FString& Error);

	/** Read a property by path as UE text. */
	UFUNCTION(BlueprintCallable, Category = "FT Tools")
	static FString GetPath(UObject* Root, const FString& Path);

	/** Append a new instanced sub-object of ClassPath to the object-array property at Path; returns its index. */
	UFUNCTION(BlueprintCallable, Category = "FT Tools")
	static int32 AddInstanced(UObject* Root, const FString& ArrayPath, const FString& ClassPath, FString& Error);

	/** Remove index from an array property at Path. */
	UFUNCTION(BlueprintCallable, Category = "FT Tools")
	static bool RemoveAt(UObject* Root, const FString& ArrayPath, int32 Index);

	/** Recompile a Niagara system after edits and wait for it. */
	UFUNCTION(BlueprintCallable, Category = "FT Tools")
	static void CompileNiagara(UNiagaraSystem* System);

	/** Make a Chaos geometry collection asset by Voronoi-fracturing a static mesh. */
	UFUNCTION(BlueprintCallable, Category = "FT Tools")
	static UGeometryCollection* MakeFractured(UStaticMesh* Mesh, const FString& PackagePath, const FString& Name, int32 Sites, int32 Seed);
};
