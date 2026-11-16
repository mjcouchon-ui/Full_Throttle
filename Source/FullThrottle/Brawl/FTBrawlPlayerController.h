// Input remains live while the brawl simulation and audio are paused.
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
 virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
 virtual bool InputKey(const FInputKeyEventArgs& Params) override;
 virtual void SetupInputComponent() override;
 virtual void PlayerTick(float DeltaTime) override;
 void ResetSystemPointer();
 bool HasHeldActionInput() const;
private:
 void PauseBrawl();
 void MenuUp();
 void MenuDown();
 void MenuLeft();
 void MenuRight();
 void MenuAccept();
 void MenuBack();
 void MenuClick();
 bool MenuReady() const;
 void ReconcileFocusInput(bool bLostFocus);
 public: int32 MouseRow() const; private:
 FVector2D LastMouse = FVector2D::ZeroVector;
 bool bPointerReady = false;
 double NextStickAt = 0, MenuReadyAt = 0;
 // PlayerInput flushes on menu transitions; physical releases must still gate combat.
 TSet<FKey> HeldInputKeys;
 FDelegateHandle ApplicationFocusHandle, ViewportFocusHandle;
 // A new controller must observe neutral after map travel before taking menu input.
 bool bMenuInputReady = false;
 double MenuInputReadyAfter = 0;
#if !UE_BUILD_SHIPPING
 TMap<FString,float> TraceAxisValue;
 TMap<FString,double> TraceAxisTime;
#endif
};
