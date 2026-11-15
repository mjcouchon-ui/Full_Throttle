#include "FTBrawlPlayerController.h"
#include "FTBrawlGameMode.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"

AFTBrawlPlayerController::AFTBrawlPlayerController()
{
	bShouldPerformFullTickWhenPaused=true;
	PrimaryActorTick.bTickEvenWhenPaused=true;
}
void AFTBrawlPlayerController::BeginPlay()
{
	Super::BeginPlay(); SetInputMode(FInputModeGameOnly()); bShowMouseCursor=false;
}
void AFTBrawlPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	auto Bind=[this](FKey Key,void(AFTBrawlPlayerController::*Fn)())
	{ FInputKeyBinding& B=InputComponent->BindKey(Key,IE_Pressed,this,Fn); B.bExecuteWhenPaused=true; };
	Bind(EKeys::Escape,&AFTBrawlPlayerController::PauseBrawl);
	Bind(EKeys::Gamepad_Special_Right,&AFTBrawlPlayerController::PauseBrawl);
	for(FKey K:{EKeys::Up,EKeys::W,EKeys::Gamepad_DPad_Up}) Bind(K,&AFTBrawlPlayerController::MenuUp);
	for(FKey K:{EKeys::Down,EKeys::S,EKeys::Gamepad_DPad_Down}) Bind(K,&AFTBrawlPlayerController::MenuDown);
	Bind(EKeys::Enter,&AFTBrawlPlayerController::MenuAccept);
	Bind(EKeys::Gamepad_FaceButton_Bottom,&AFTBrawlPlayerController::MenuAccept);
	Bind(EKeys::Gamepad_FaceButton_Right,&AFTBrawlPlayerController::MenuBack);
	Bind(EKeys::LeftMouseButton,&AFTBrawlPlayerController::MenuClick);
}
void AFTBrawlPlayerController::PauseBrawl() { if(auto* G=GetWorld()->GetAuthGameMode<AFTBrawlGameMode>()) G->ToggleSystemMenu(); }
void AFTBrawlPlayerController::MenuUp() { if(auto* G=GetWorld()->GetAuthGameMode<AFTBrawlGameMode>()) G->SystemNavigate(-1); }
void AFTBrawlPlayerController::MenuDown() { if(auto* G=GetWorld()->GetAuthGameMode<AFTBrawlGameMode>()) G->SystemNavigate(1); }
void AFTBrawlPlayerController::MenuAccept() { if(auto* G=GetWorld()->GetAuthGameMode<AFTBrawlGameMode>()) G->SystemActivate(); }
void AFTBrawlPlayerController::MenuBack() { if(auto* G=GetWorld()->GetAuthGameMode<AFTBrawlGameMode>()) if(G->SystemScreen) G->SystemBack(); }
int32 AFTBrawlPlayerController::MouseRow() const
{
	float X=0,Y=0; int32 W=0,H=0; GetViewportSize(W,H);
	if(H<=0 || !GetMousePosition(X,Y)) return INDEX_NONE;
	const float Scale=H/720.f; X/=Scale; Y/=Scale;
	if(FMath::Abs(X-W/Scale*.5f)>290.f || Y<320.f) return INDEX_NONE;
	const int32 Row=FMath::FloorToInt((Y-320.f)/48.f);
	return FMath::Fmod(Y-320.f,48.f)<42.f?Row:INDEX_NONE;
}
void AFTBrawlPlayerController::MenuClick()
{
	if(auto* G=GetWorld()->GetAuthGameMode<AFTBrawlGameMode>()) if(G->SystemScreen)
	{ const int32 Row=MouseRow(); if(G->SystemRows().IsValidIndex(Row)) { G->SystemSelection=Row; G->SystemActivate(); } }
}
void AFTBrawlPlayerController::ResetSystemPointer()
{
	// A newly created controller and cursor release/capture are not mouse movements.
	// Seed the position now, then let the first tick establish the post-mode baseline.
	float X=0,Y=0; if(GetMousePosition(X,Y)) LastMouse=FVector2D(X,Y);
	bPointerReady=false; NextStickAt=0;
}
void AFTBrawlPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	AFTBrawlGameMode* G=GetWorld()->GetAuthGameMode<AFTBrawlGameMode>();
	if (G) G->PollSystemSave();
	const bool bMenu=G && G->SystemScreen!=0;
	float X=0,Y=0;
	if(GetMousePosition(X,Y))
	{
		const FVector2D At(X,Y);
		if(bMenu && bPointerReady && (At-LastMouse).SizeSquared()>4.f)
		{ const int32 Row=MouseRow(); if(G->SystemRows().IsValidIndex(Row)) G->SystemSelection=Row; }
		LastMouse=At; bPointerReady=bMenu;
	}
	else bPointerReady=false;
	if(!bMenu) return;
	const float Stick=GetInputAnalogKeyState(EKeys::Gamepad_LeftY); const double Now=FPlatformTime::Seconds();
	if(FMath::Abs(Stick)<.45f) NextStickAt=0;
	else if(Now>=NextStickAt) { G->SystemNavigate(Stick>0?-1:1); NextStickAt=Now+.22; }
}
