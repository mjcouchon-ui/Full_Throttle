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
	AFTBrawlPlayerController();
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;
	void ResetSystemPointer();

private:
	void PauseBrawl();
	void MenuUp();
	void MenuDown();
	void MenuAccept();
	void MenuBack();
	void MenuClick();
	int32 MouseRow() const;
	FVector2D LastMouse = FVector2D::ZeroVector;
	bool bPointerReady = false;
	double NextStickAt = 0;
};
