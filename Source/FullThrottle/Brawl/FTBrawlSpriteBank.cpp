#include "FTBrawlSpriteBank.h"
#include "AssetRegistry/AssetData.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Engine/Texture.h"
#include "Engine/World.h"
#include "Materials/MaterialInterface.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#if WITH_EDITOR
#include "TextureCompiler.h"
#endif

namespace
{
	FString SpriteFolder(const FString& Who) { return FString::Printf(TEXT("/Game/Brawl/Sprites/%s"), *Who); }
	FString SpritePrefix(const FString& Who) { return FString::Printf(TEXT("T_%s_"), *Who.ToLower()); }
}

UFTBrawlSpriteBank* UFTBrawlSpriteBank::Get(const UObject* WorldContext)
{
	// -brawlnobank (tests): no bank, every fighter looks his drawings up by path as before, to compare the two ways
	static const bool bOff = FParse::Param(FCommandLine::Get(), TEXT("brawlnobank"));
	if (bOff) return nullptr;
	const UWorld* W = WorldContext ? WorldContext->GetWorld() : nullptr;
	return W ? W->GetSubsystem<UFTBrawlSpriteBank>() : nullptr;
}

void UFTBrawlSpriteBank::List(const FString& Folder, TArray<FAssetData>& Out) const
{
	IAssetRegistry* AR = IAssetRegistry::Get();
	if (!AR) return;
	// an uncooked game is still reading the content folders in its first seconds: read this one folder now
	if (AR->IsLoadingAssets()) AR->ScanPathsSynchronous({ Folder }, false);
	AR->GetAssetsByPath(FName(*Folder), Out, false, true);
}

void UFTBrawlSpriteBank::FinishTextures()
{
#if WITH_EDITOR
	FTextureCompilingManager::Get().FinishAllCompilation();
#endif
}

// Uncooked game only (EnsureIsComplete does nothing in a cooked build, whose materials come with all their shaders):
// compiles what is left of each material's shaders and waits. Returns the seconds it took.
double UFTBrawlSpriteBank::FinishMaterials(const TArray<TObjectPtr<UObject>>& Objects)
{
	// -brawlnoshaders (tests): leave the shaders to the first draw as before, to compare the two ways
	static const bool bOff = FParse::Param(FCommandLine::Get(), TEXT("brawlnoshaders"));
	if (bOff) return 0.0;
	const double T0 = FPlatformTime::Seconds();
	for (UObject* O : Objects) if (UMaterialInterface* M = Cast<UMaterialInterface>(O)) M->EnsureIsComplete();
	return FPlatformTime::Seconds() - T0;
}

// one request, waited for here; the handle keeps what it loaded until the map ends
void UFTBrawlSpriteBank::Load(TArray<FSoftObjectPath>&& Paths)
{
	if (Paths.Num() == 0 || !UAssetManager::IsInitialized()) return;
	TSharedPtr<FStreamableHandle> H = UAssetManager::GetStreamableManager().RequestSyncLoad(MoveTemp(Paths), false, TEXT("FTBrawlNight"));
	if (H.IsValid()) Handles.Add(H);
}

void UFTBrawlSpriteBank::TestStage(const UObject* WorldContext)
{
	if (UFTBrawlSpriteBank* Bank = Get(WorldContext)) Bank->bTestStage = true;
}

bool UFTBrawlSpriteBank::Has(const FString& Who)
{
	if (const FFTBrawlSpriteTable* T = Tables.Find(FName(*Who))) return T->Frames.Contains(TEXT("idle"));
	TArray<FAssetData> Assets;
	List(SpriteFolder(Who), Assets);
	const FName Idle(*(SpritePrefix(Who) + TEXT("idle")));
	for (const FAssetData& A : Assets) if (A.AssetName == Idle) return true;
	return false;
}

void UFTBrawlSpriteBank::Fill(const FString& Who, const TArray<FAssetData>& Assets)
{
	FFTBrawlSpriteTable& T = Tables.FindOrAdd(FName(*Who));
	const FString Prefix = SpritePrefix(Who);
	for (const FAssetData& A : Assets)
	{
		const FString Name = A.AssetName.ToString();
		if (!Name.StartsWith(Prefix)) continue;
		if (UTexture* Tx = Cast<UTexture>(A.GetAsset())) T.Frames.Add(FName(*Name.Mid(Prefix.Len())), Tx);
	}
}

void UFTBrawlSpriteBank::WarmNight(int32 Night, const TArray<FString>& Cast, const TArray<FString>& ExtraFolders, const TArray<FString>& ExtraObjects)
{
	const double T0 = FPlatformTime::Seconds();
	TArray<TArray<FAssetData>> PerWho; PerWho.SetNum(Cast.Num());
	TArray<FAssetData> ExtraAssets;
	TArray<FSoftObjectPath> Paths;
	for (int32 i = 0; i < Cast.Num(); i++)
	{
		if (Tables.Contains(FName(*Cast[i]))) continue;
		List(SpriteFolder(Cast[i]), PerWho[i]);
		const FString Prefix = SpritePrefix(Cast[i]);
		for (const FAssetData& A : PerWho[i]) if (A.AssetName.ToString().StartsWith(Prefix)) Paths.Add(A.GetSoftObjectPath());
	}
	for (const FString& Folder : ExtraFolders) List(Folder, ExtraAssets);
	for (const FAssetData& A : ExtraAssets) Paths.Add(A.GetSoftObjectPath());
	for (const FString& Path : ExtraObjects) Paths.Add(FSoftObjectPath(Path));
	const int32 Count = Paths.Num();
	Load(MoveTemp(Paths));   // the whole night in one request: the map is still opening, nothing of the night is on screen yet
	int32 Drawings = 0;
	FString Names;
	for (int32 i = 0; i < Cast.Num(); i++)
	{
		if (!Tables.Contains(FName(*Cast[i]))) Fill(Cast[i], PerWho[i]);
		const int32 N = Tables.FindChecked(FName(*Cast[i])).Frames.Num();
		Drawings += N;
		Names += FString::Printf(TEXT("%s%s:%d"), i ? TEXT(",") : TEXT(""), *Cast[i], N);
	}
	for (const FAssetData& A : ExtraAssets) if (UObject* O = A.GetAsset()) Extras.Add(O);
	for (const FString& Path : ExtraObjects) if (UObject* O = FSoftObjectPath(Path).ResolveObject()) Extras.Add(O);
	FinishTextures();
	const double Shaders = FinishMaterials(Extras);
	bWarmed = true;
	UE_LOG(LogTemp, Display, TEXT("FTBRAWL sprites warm night=%d assets=%d drawings=%d extras=%d seconds=%.2f shaders=%.2f cast=%s"),
		Night, Count, Drawings, Extras.Num(), FPlatformTime::Seconds() - T0, Shaders, *Names);
}

const FFTBrawlSpriteTable& UFTBrawlSpriteBank::Table(const FString& Who)
{
	const FName Key(*Who);
	if (const FFTBrawlSpriteTable* T = Tables.Find(Key)) return *T;
	const double T0 = FPlatformTime::Seconds();
	TArray<FAssetData> Assets;
	List(SpriteFolder(Who), Assets);
	TArray<FSoftObjectPath> Paths;
	const FString Prefix = SpritePrefix(Who);
	for (const FAssetData& A : Assets) if (A.AssetName.ToString().StartsWith(Prefix)) Paths.Add(A.GetSoftObjectPath());
	Load(MoveTemp(Paths));   // his whole folder in one blocking request: he is whole before he is first shown
	Fill(Who, Assets);
	FinishTextures();
	const FFTBrawlSpriteTable& T = Tables.FindChecked(Key);
	const double Ms = (FPlatformTime::Seconds() - T0) * 1000.0;
	if (bWarmed && !bTestStage)
	{
		UE_LOG(LogTemp, Error, TEXT("FTBRAWL sprites late who=%s drawings=%d ms=%.0f afterwarm=1 (not in the night's list: NightSprites / CutSprites)"), *Who, T.Frames.Num(), Ms);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("FTBRAWL sprites late who=%s drawings=%d ms=%.0f afterwarm=%d teststage=%d"), *Who, T.Frames.Num(), Ms, bWarmed ? 1 : 0, bTestStage ? 1 : 0);
	}
	return T;
}

void UFTBrawlSpriteBank::Deinitialize()
{
	if (Tables.Num() || Handles.Num())
		UE_LOG(LogTemp, Display, TEXT("FTBRAWL sprites released characters=%d handles=%d"), Tables.Num(), Handles.Num());
	for (TSharedPtr<FStreamableHandle>& H : Handles) if (H.IsValid()) H->ReleaseHandle();
	Handles.Reset(); Tables.Reset(); Extras.Reset(); bWarmed = bTestStage = false;
	Super::Deinitialize();
}
