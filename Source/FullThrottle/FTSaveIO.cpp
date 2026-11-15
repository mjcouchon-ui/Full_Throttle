#include "FTSaveIO.h"
#include "HAL/IConsoleManager.h"
#include "HAL/PlatformProcess.h"
#include "Async/Async.h"

#if !UE_BUILD_SHIPPING
static TAutoConsoleVariable<int32> GFTSaveFail(TEXT("ft.SaveFail"), 0,
	TEXT("Save recovery QA: 0 normal; 1 fail before writing; 2 interrupt after verified temp. Never replaces the previous save."));
static TAutoConsoleVariable<int32> GFTSaveDelay(TEXT("ft.SaveDelayMs"), 0,
	TEXT("Save responsiveness QA: extra worker-thread delay in milliseconds. UI and exit requests remain responsive."));
#endif

void FTSaveIO::WriteAsync(FString Path, FString Text, bool bPreserveBackup, TFunction<void(bool, const FString&)> Completion)
{
	check(IsInGameThread());
	int32 FaultMode = 0, DelayMs = 0;
#if !UE_BUILD_SHIPPING
	FaultMode = GFTSaveFail.GetValueOnGameThread();
	DelayMs = FMath::Clamp(GFTSaveDelay.GetValueOnGameThread(), 0, 30000);
#endif
	Async(EAsyncExecution::ThreadPool, [Path = MoveTemp(Path), Text = MoveTemp(Text), bPreserveBackup,
		FaultMode, DelayMs, Completion = MoveTemp(Completion)]() mutable
	{
		const double Started = FPlatformTime::Seconds();
		if (DelayMs > 0) FPlatformProcess::Sleep(float(DelayMs) / 1000.f);
		FString Error;
		const bool bOk = Write(Path, Text, Error, bPreserveBackup, FaultMode);
		// Keep the real pending state visible for at least a frame on fast storage.
		// This delay runs only on the worker; gameplay and menu input continue.
		const double Remaining = .15 - (FPlatformTime::Seconds() - Started);
		if (Remaining > 0.) FPlatformProcess::Sleep(float(Remaining));
		AsyncTask(ENamedThreads::GameThread, [bOk, Error = MoveTemp(Error), Completion = MoveTemp(Completion)]() mutable
		{ Completion(bOk, Error); });
	});
}
