// The night's drawings. One table per character, built once per brawl map from the character's sprite folder
// (/Game/Brawl/Sprites/<Who>/T_<who>_<drawing>): it holds every drawing the folder has, so a name that is not in the
// table does not exist and is never looked for on disk (the table is both the hits and the misses). A fighter's
// SetSprite reads this table instead of asking the loader for each drawing by name.
// WarmNight loads the night's whole cast in one blocking request while the map is still opening (before the first
// frame of the night is drawn), so no drawing is loaded during play. The bank lives and dies with the brawl map's
// world: when the map ends its tables and its load handle go, and the next night or drive does not keep this cast.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "FTBrawlSpriteBank.generated.h"

class UTexture;
struct FStreamableHandle;

USTRUCT()
struct FFTBrawlSpriteTable
{
	GENERATED_BODY()
	UPROPERTY() TMap<FName, TObjectPtr<UTexture>> Frames;   // drawing name (without T_<who>_) -> texture, the whole folder
};

UCLASS()
class UFTBrawlSpriteBank : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	static UFTBrawlSpriteBank* Get(const UObject* WorldContext);
	// True if the folder has the character's idle drawing (what SetSprite needs to succeed). Asks the asset registry only.
	bool Has(const FString& Who);
	// Blocks until every drawing of these characters, and every texture in these extra folders, is in memory.
	// ExtraObjects: single assets by object path (the fighters' materials). A material among them is also made whole:
	// the uncooked game compiles a material's shaders the first time something is drawn with it and draws the engine's
	// plain grey material in its place until they are there (a fighter was a dark card for a frame); see FinishMaterials.
	void WarmNight(int32 Night, const TArray<FString>& Cast, const TArray<FString>& ExtraFolders, const TArray<FString>& ExtraObjects);
	// The character's table. Not warmed yet = his folder is loaded here and now, in one blocking request, and the log
	// says "FTBRAWL sprites late". After the night's warm-up that line is an Error: somebody is on screen tonight who is
	// not in the night's list (NightSprites, CutSprites), and a night's test run fails on it. A development command that
	// stands other characters in the room (the pose stage, ftb.MoveFixture, ftb.Variety) calls TestStage first.
	const FFTBrawlSpriteTable& Table(const FString& Who);
	static void TestStage(const UObject* WorldContext);
	// Uncooked game only (does nothing in a cooked build): a texture that was just loaded is still being prepared in the
	// background and draws as the engine's grey checker until that ends. Waits for every texture in preparation.
	static void FinishTextures();
	virtual void Deinitialize() override;

private:
	UPROPERTY() TMap<FName, FFTBrawlSpriteTable> Tables;
	UPROPERTY() TArray<TObjectPtr<UObject>> Extras;   // the item and prop folders, the foes' material (looked up by path by the fight's own code)
	TArray<TSharedPtr<FStreamableHandle>> Handles;
	bool bWarmed = false;
	bool bTestStage = false;
	void Load(TArray<struct FSoftObjectPath>&& Paths);
	static double FinishMaterials(const TArray<TObjectPtr<UObject>>& Objects);
	void List(const FString& Folder, TArray<struct FAssetData>& Out) const;
	void Fill(const FString& Who, const TArray<struct FAssetData>& Assets);
};
