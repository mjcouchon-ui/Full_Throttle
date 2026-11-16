// 4.0.1: the standalone (game jam) build switch. 0 = the WispOS app: Shipping refuses to run without a WispOS launch
// ticket. 1 = a build that runs on its own (double-click FullThrottle.exe; saves go to the engine's Saved folder).
// Keep 0 in the repo; set 1 only to package the jam zip, then put it back. Only FTGameInstance.cpp includes this.
#pragma once
#define FT_STANDALONE 0
