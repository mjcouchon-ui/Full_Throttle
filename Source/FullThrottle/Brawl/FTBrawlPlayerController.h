// The brawl's controller: no pawn, the game mode reads its keys (IsInputKeyDown) each frame.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FTBrawlPlayerController.generated.h"

UCLASS()
class AFTBrawlPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
};
