#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"
class UCanvas;
class UFont;
class UFTInputSubsystem;

namespace FTInputGlyphs
{
	/** Original vector glyphs; no vendor artwork or font-specific symbol characters. */
	void DrawFace(UCanvas* Canvas, const UFTInputSubsystem* Input, const FKey& Key, FVector2D Center, float Size, UFont* Font);
}
