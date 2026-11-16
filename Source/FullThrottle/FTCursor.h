// The mouse pointer of the menus: a chunky inked arrow drawn on the canvas (all art is code), shared by the races'
// HUD and the nights' HUD. Cream with an aqua cel shade on a pink print shadow; over something that can be clicked it
// turns amber and throws three sparks; while the button is down it sinks onto its shadow, hot pink, with an impact star.
// The system's own arrow is switched off while a screen wants the mouse (HideSystemCursor).
#pragma once

#include "CoreMinimal.h"

class UCanvas;
class APlayerController;

namespace FTCursor
{
	enum class EState : uint8 { Normal, Hover, Pressed };
	/** Tip at At (canvas pixels). S: the HUD's scale (1 on a 720-high screen). Time: real seconds; bStill: reduced motion. */
	void Draw(UCanvas* Canvas, const FVector2D& At, float S, EState State, float Time, bool bStill);
	/** The player controller keeps bShowMouseCursor (so clicks and positions arrive as before) but shows no system pointer. */
	void HideSystemCursor(APlayerController* PC);
}
