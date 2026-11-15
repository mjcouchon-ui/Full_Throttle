#include "FTBrawlPlayerController.h"
#include "FTBrawlGameMode.h"
#include "FTInput.h"
#include "InputKeyEventArgs.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "Engine/GameViewportClient.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/SViewport.h"
#if !UE_BUILD_SHIPPING
#include "HAL/IConsoleManager.h"
namespace
{
	TAutoConsoleVariable<int32> CVarBrawlInputTrace(TEXT("ftb.InputTrace"),0,
		TEXT("Log platform gamepad events received by Brawl. Excludes simulated UE input; default off. Axis events are rate limited."));
}
#endif

bool AFTBrawlPlayerController::InputKey(const FInputKeyEventArgs& Params)
{
#if !UE_BUILD_SHIPPING
	// Observe the existing path without changing dispatch or consuming any input.
	// A virtual OS controller can still produce platform events: device trace alone
	// does not establish that a native physical DualShock 4 was connected.
	if (CVarBrawlInputTrace.GetValueOnGameThread()!=0 && Params.IsGamepad() && !Params.IsSimulatedInput())
	{
		bool bLog=Params.Event!=IE_Repeat && Params.Event!=IE_Axis;
		if (Params.Event==IE_Axis)
		{
			const FString Axis=FString::Printf(TEXT("%d:%s"),Params.InputDevice.GetId(),*Params.Key.ToString());
			const float Previous=TraceAxisValue.FindRef(Axis);
			const double Now=FPlatformTime::Seconds(),Last=TraceAxisTime.FindRef(Axis);
			const bool bNeutral=FMath::Abs(Previous)>.2f && FMath::Abs(Params.AmountDepressed)<=.2f;
			bLog=bNeutral || (FMath::Abs(Params.AmountDepressed-Previous)>=.15f && Now-Last>=.15);
			if (bLog) { TraceAxisValue.Add(Axis,Params.AmountDepressed); TraceAxisTime.Add(Axis,Now); }
		}
		if (bLog) UE_LOG(LogTemp,Display,TEXT("FTBRAWL INPUT source=platform simulated=0 device=%d user=%d controller=%d key=%s event=%d value=%.3f timestamp=%llu"),
			Params.InputDevice.GetId(),Params.GetPlatformUser().GetInternalId(),Params.ControllerId,*Params.Key.ToString(),
			int32(Params.Event),Params.AmountDepressed,static_cast<unsigned long long>(Params.EventTimestamp));
	}
#endif
 if (UFTInputSubsystem* Input=UFTInputSubsystem::Get(this)) Input->ObserveInput(Params);
 if (Params.Event==IE_Pressed || Params.Event==IE_Repeat) HeldInputKeys.Add(Params.Key);
 else if (Params.Event==IE_Released) HeldInputKeys.Remove(Params.Key);
 return Super::InputKey(Params);
}
bool AFTBrawlPlayerController::HasHeldActionInput() const
{
 auto Held=[this](const FKey& Key)
 {
  if (HeldInputKeys.Contains(Key)) return true;
  if (Key==EKeys::LeftShift || Key==EKeys::RightShift) return HeldInputKeys.Contains(EKeys::LeftShift) || HeldInputKeys.Contains(EKeys::RightShift);
  if (Key==EKeys::LeftControl || Key==EKeys::RightControl) return HeldInputKeys.Contains(EKeys::LeftControl) || HeldInputKeys.Contains(EKeys::RightControl);
  if (Key==EKeys::LeftAlt || Key==EKeys::RightAlt) return HeldInputKeys.Contains(EKeys::LeftAlt) || HeldInputKeys.Contains(EKeys::RightAlt);
  return false;
 };
 for (const FKey Key:{EKeys::Enter,EKeys::Tab,EKeys::R,EKeys::LeftMouseButton,EKeys::RightMouseButton,EKeys::Gamepad_Special_Left,EKeys::Gamepad_FaceButton_Left,EKeys::Gamepad_FaceButton_Right,EKeys::Gamepad_FaceButton_Top,EKeys::Gamepad_FaceButton_Bottom})
  if (Held(Key)) return true;
 if (const UFTInputSubsystem* Input=UFTInputSubsystem::Get(this))
  for (const EFTBrawlAction Action:{EFTBrawlAction::Punch,EFTBrawlAction::Kick,EFTBrawlAction::Jump,EFTBrawlAction::GrabThrow})
   if (Held(Input->BrawlKeyboardKey(Action))) return true;
 return false;
}
AFTBrawlPlayerController::AFTBrawlPlayerController()
{
 bShouldPerformFullTickWhenPaused=true;
 PrimaryActorTick.bTickEvenWhenPaused=true;
}
void AFTBrawlPlayerController::BeginPlay()
{
	Super::BeginPlay(); SetInputMode(FInputModeGameOnly()); bShowMouseCursor=false;
	bMenuInputReady=false; MenuInputReadyAfter=FPlatformTime::Seconds()+.12;
	if (IsLocalController() && FSlateApplication::IsInitialized())
	{
		FSlateApplication& Slate=FSlateApplication::Get();
		ApplicationFocusHandle=Slate.OnApplicationActivationStateChanged().AddWeakLambda(this,[this](bool bActive)
		{ ReconcileFocusInput(!bActive); });
		ViewportFocusHandle=Slate.OnFocusChanging().AddWeakLambda(this,[this](const FFocusEvent& Event,const FWeakWidgetPath&,const TSharedPtr<SWidget>& OldWidget,const FWidgetPath&,const TSharedPtr<SWidget>& NewWidget)
		{
			if (Event.GetUser()!=FSlateApplication::Get().GetUserIndexForKeyboard()) return;
			UGameViewportClient* Viewport=GetWorld() ? GetWorld()->GetGameViewport() : nullptr;
			const TSharedPtr<SViewport> Widget=Viewport ? Viewport->GetGameViewportWidget() : nullptr;
			if (!Widget.IsValid()) return;
			if (OldWidget.Get()==Widget.Get() && NewWidget.Get()!=Widget.Get()) ReconcileFocusInput(true);
			else if (NewWidget.Get()==Widget.Get() && OldWidget.Get()!=Widget.Get()) ReconcileFocusInput(false);
		});
	}
}
void AFTBrawlPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().OnApplicationActivationStateChanged().Remove(ApplicationFocusHandle);
		FSlateApplication::Get().OnFocusChanging().Remove(ViewportFocusHandle);
	}
	Super::EndPlay(EndPlayReason);
}
void AFTBrawlPlayerController::ReconcileFocusInput(bool bLostFocus)
{
	// Unreal flushes PlayerInput on focus loss without routing releases through our
	// InputKey override. Those raw holds can no longer be trusted: key-up may happen
	// in another window. Ordinary menu flushes deliberately do NOT clear this cache.
	if (bLostFocus) HeldInputKeys.Reset();
	bMenuInputReady=false; MenuInputReadyAfter=FPlatformTime::Seconds()+.12;
	ResetSystemPointer();
	if (UWorld* World=GetWorld()) if (auto* G=World->GetAuthGameMode<AFTBrawlGameMode>()) G->SuppressActionInput();
	UE_LOG(LogTemp,Display,TEXT("FT BRAWL focus=%s rawHeld=%d"),bLostFocus ? TEXT("lost") : TEXT("returned"),HeldInputKeys.Num());
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
 for(FKey K:{EKeys::Left,EKeys::A,EKeys::Gamepad_DPad_Left}) Bind(K,&AFTBrawlPlayerController::MenuLeft);
 for(FKey K:{EKeys::Right,EKeys::D,EKeys::Gamepad_DPad_Right}) Bind(K,&AFTBrawlPlayerController::MenuRight);
 for(FKey K:{EKeys::Enter,EKeys::SpaceBar,EKeys::Gamepad_FaceButton_Bottom}) Bind(K,&AFTBrawlPlayerController::MenuAccept);
 for(FKey K:{EKeys::Gamepad_FaceButton_Right,EKeys::Gamepad_Special_Left}) Bind(K,&AFTBrawlPlayerController::MenuBack);
 Bind(EKeys::LeftMouseButton,&AFTBrawlPlayerController::MenuClick);
}
bool AFTBrawlPlayerController::MenuReady() const { return bMenuInputReady && FPlatformTime::Seconds()>=MenuReadyAt; }
void AFTBrawlPlayerController::PauseBrawl() { if(MenuReady()) if(auto* G=GetWorld()->GetAuthGameMode<AFTBrawlGameMode>()) G->ToggleSystemMenu(); }
void AFTBrawlPlayerController::MenuUp() { if(MenuReady()) if(auto* G=GetWorld()->GetAuthGameMode<AFTBrawlGameMode>()) G->SystemNavigate(-1); }
void AFTBrawlPlayerController::MenuDown() { if(MenuReady()) if(auto* G=GetWorld()->GetAuthGameMode<AFTBrawlGameMode>()) G->SystemNavigate(1); }
void AFTBrawlPlayerController::MenuLeft() { if(MenuReady()) if(auto* G=GetWorld()->GetAuthGameMode<AFTBrawlGameMode>()) G->SystemAdjust(-1); }
void AFTBrawlPlayerController::MenuRight() { if(MenuReady()) if(auto* G=GetWorld()->GetAuthGameMode<AFTBrawlGameMode>()) G->SystemAdjust(1); }
void AFTBrawlPlayerController::MenuAccept() { if(MenuReady()) if(auto* G=GetWorld()->GetAuthGameMode<AFTBrawlGameMode>()) G->SystemActivate(); }
void AFTBrawlPlayerController::MenuBack() { if(MenuReady()) if(auto* G=GetWorld()->GetAuthGameMode<AFTBrawlGameMode>()) if(G->SystemScreen) G->SystemBack(); }
int32 AFTBrawlPlayerController::MouseRow() const
{
 const AFTBrawlGameMode* G=GetWorld()->GetAuthGameMode<AFTBrawlGameMode>();
 float X=0,Y=0; int32 W=0,H=0; GetViewportSize(W,H);
 if(!G || H<=0 || !GetMousePosition(X,Y)) return INDEX_NONE;
 const float Scale=H/720.f;
 for(int32 Row=0;Row<G->SystemRows().Num();++Row)
  if(G->SystemItemRect(Row,W/Scale).IsInside(FVector2D(X/Scale,Y/Scale))) return Row;
 return INDEX_NONE;
}
void AFTBrawlPlayerController::MenuClick()
{
 if(!MenuReady()) return;
 if(auto* G=GetWorld()->GetAuthGameMode<AFTBrawlGameMode>()) if(G->SystemScreen)
 { const int32 Row=MouseRow(); if(G->SystemRows().IsValidIndex(Row)) { G->SystemSelection=Row; G->SyncPauseCompatibility(); G->SystemActivate(); } }
}
void AFTBrawlPlayerController::ResetSystemPointer()
{
 float X=0,Y=0; if(GetMousePosition(X,Y)) LastMouse=FVector2D(X,Y);
 bPointerReady=false; NextStickAt=0; MenuReadyAt=FPlatformTime::Seconds()+.12;
}
void AFTBrawlPlayerController::PlayerTick(float DeltaTime)
{
 AFTBrawlGameMode* G=GetWorld()->GetAuthGameMode<AFTBrawlGameMode>();
 UFTInputSubsystem* Input=UFTInputSubsystem::Get(this);
 if(G && Input)
 {
  G->bPadLast=Input->UsesGamepad();
  if(Input->ConsumeDisconnect())
  {
   // Disconnected hardware cannot release its keys; keyboard holds remain meaningful.
   for(auto It=HeldInputKeys.CreateIterator();It;++It) if(It->IsGamepadKey()) It.RemoveCurrent();
   G->OpenDisconnectedSystemMenu();
  }
  else if(G->bDisconnectedPause && !Input->HasDisconnectedController()) G->bDisconnectedPause=false;
 }
 Super::PlayerTick(DeltaTime);
 if(G) G->PollSystemSave();
 if(!bMenuInputReady)
 {
  // Super has processed this frame's platform input. Raw holds survive menu flushes.
  bool bHeld=false;
  for(const FKey Key:{EKeys::Enter,EKeys::SpaceBar,EKeys::Escape,EKeys::LeftMouseButton,
   EKeys::Gamepad_FaceButton_Bottom,EKeys::Gamepad_FaceButton_Right,
   EKeys::Gamepad_Special_Right,EKeys::Gamepad_Special_Left,
   EKeys::Up,EKeys::Down,EKeys::Left,EKeys::Right,EKeys::W,EKeys::S,EKeys::A,EKeys::D,
   EKeys::Gamepad_DPad_Up,EKeys::Gamepad_DPad_Down,EKeys::Gamepad_DPad_Left,EKeys::Gamepad_DPad_Right})
   bHeld |= IsInputKeyDown(Key) || HeldInputKeys.Contains(Key);
  bHeld |= FMath::Abs(GetInputAnalogKeyState(EKeys::Gamepad_LeftY))>=.45f
   || FMath::Abs(GetInputAnalogKeyState(EKeys::Gamepad_LeftX))>=.45f;
  if(bHeld || FPlatformTime::Seconds()<MenuInputReadyAfter) return;
  bMenuInputReady=true;
  UE_LOG(LogTemp,Display,TEXT("FT BRAWL SYSTEM input_ready=1"));
 }
 const bool bMenu=G && G->SystemScreen!=0;
 float X=0,Y=0;
 if(GetMousePosition(X,Y))
 {
  const FVector2D At(X,Y);
  if(bMenu && bPointerReady && (!Input || !Input->UsesGamepad()) && (At-LastMouse).SizeSquared()>4.f)
  { const int32 Row=MouseRow(); if(G->SystemRows().IsValidIndex(Row)) { G->SystemSelection=Row; G->SyncPauseCompatibility(); } }
  LastMouse=At; bPointerReady=bMenu;
 }
 else bPointerReady=false;
 if(!bMenu || !MenuReady()) return;
 const float Stick=GetInputAnalogKeyState(EKeys::Gamepad_LeftY); const double Now=FPlatformTime::Seconds();
 if(FMath::Abs(Stick)<.45f) NextStickAt=0;
 else if(Now>=NextStickAt) { G->SystemNavigate(Stick>0?-1:1); NextStickAt=Now+.22; }
}
