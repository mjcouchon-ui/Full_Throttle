#include "FTBrawlPlayerController.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void AFTBrawlPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
}

void AFTBrawlPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindKey(EKeys::Escape, IE_Pressed, this, &AFTBrawlPlayerController::QuitBrawl);
}

void AFTBrawlPlayerController::QuitBrawl()
{
	UKismetSystemLibrary::QuitGame(this, this, EQuitPreference::Quit, false);
}
