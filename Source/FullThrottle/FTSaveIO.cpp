#include "FTSaveIO.h"
#include "HAL/IConsoleManager.h"

#if !UE_BUILD_SHIPPING
static TAutoConsoleVariable<int32> GFTSaveFail(TEXT("ft.SaveFail"), 0,
	TEXT("Save recovery QA: 0 normal; 1 fail before writing; 2 interrupt after verified temp. Never replaces the previous save."));
#endif
