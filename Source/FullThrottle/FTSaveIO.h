#pragma once

#include "CoreMinimal.h"
#include "HAL/PlatformFile.h"
#include "HAL/PlatformFileManager.h"
#include "HAL/IConsoleManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/ScopeLock.h"
#if PLATFORM_WINDOWS
#include "Windows/WindowsHWrapper.h"
#else
#include <cstdio>
#endif

// Small checkpoint files are serialized on the game thread and written under one
// lock. A verified, flushed sibling is renamed over the destination; the old
// primary is never deleted to make room. Interrupted temporary files are ignored.
namespace FTSaveIO
{
inline bool Replace(const FString& From, const FString& To)
{
	FString A = FPaths::ConvertRelativePathToFull(From), B = FPaths::ConvertRelativePathToFull(To);
	FPaths::MakePlatformFilename(A); FPaths::MakePlatformFilename(B);
#if PLATFORM_WINDOWS
	return ::MoveFileExW(*A, *B, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) != 0;
#else
	return ::rename(TCHAR_TO_UTF8(*A), TCHAR_TO_UTF8(*B)) == 0;
#endif
}

inline bool DurableText(const FString& Path, const FString& Text)
{
	IPlatformFile& Files = FPlatformFileManager::Get().GetPlatformFile();
	FTCHARToUTF8 Bytes(*Text);
	TUniquePtr<IFileHandle> Handle(Files.OpenWrite(*Path));
	if (!Handle || !Handle->Write(reinterpret_cast<const uint8*>(Bytes.Get()), Bytes.Length()) || !Handle->Flush(true)) return false;
	Handle.Reset();
	FString Check;
	return FFileHelper::LoadFileToString(Check, *Path) && Check == Text;
}

inline bool Write(const FString& Path, const FString& Text, FString& Error, bool bPreserveBackup = false)
{
	static FCriticalSection Mutex;
	FScopeLock Lock(&Mutex);
	Error.Reset();
	IPlatformFile& Files = FPlatformFileManager::Get().GetPlatformFile();
	const FString Token = FGuid::NewGuid().ToString(EGuidFormats::Digits);
	const FString Tmp = Path + TEXT(".tmp-") + Token, Bak = Path + TEXT(".bak"), BakTmp = Bak + TEXT(".tmp-") + Token;
	auto Fail = [&](const TCHAR* Why) { Error = Why; Files.DeleteFile(*Tmp); Files.DeleteFile(*BakTmp); return false; };
#if !UE_BUILD_SHIPPING
	const IConsoleVariable* Fault = IConsoleManager::Get().FindConsoleVariable(TEXT("ft.SaveFail"));
	const int32 FaultMode = Fault ? Fault->GetInt() : 0;
	if (FaultMode == 1) { UE_LOG(LogTemp, Warning, TEXT("FT SAVE fault=write_before_replace path=%s"), *Path); return Fail(TEXT("Test save failure (ft.SaveFail=1). Previous progress is unchanged.")); }
#endif
	if (!Files.CreateDirectoryTree(*FPaths::GetPath(Path))) return Fail(TEXT("Cannot create the save folder."));
	if (!DurableText(Tmp, Text)) return Fail(TEXT("Could not write and verify the new save. Previous progress is unchanged."));
#if !UE_BUILD_SHIPPING
	if (FaultMode == 2) { UE_LOG(LogTemp, Warning, TEXT("FT SAVE fault=interrupted_temp path=%s"), *Tmp); Error = TEXT("Test interrupted save (ft.SaveFail=2). Previous progress is unchanged."); return false; }
#endif
	// Recovery loads are read-only. On the first later, explicit save retain the
	// unreadable/incompatible primary as evidence instead of putting it in .bak.
	if (bPreserveBackup && Files.FileExists(*Path) && !Files.CopyFile(*(Path + TEXT(".previous-") + Token), *Path))
		return Fail(TEXT("Cannot preserve the original save before recovery."));
	if (!bPreserveBackup || !Files.FileExists(*Bak))
	{
		FString Previous = Text;
		if (!bPreserveBackup && Files.FileExists(*Path) && !FFileHelper::LoadFileToString(Previous, *Path)) return Fail(TEXT("Cannot read the previous save to make a backup."));
		if (!DurableText(BakTmp, Previous) || !Replace(BakTmp, Bak)) return Fail(TEXT("Cannot preserve the save backup. Previous progress is unchanged."));
	}
	if (!Replace(Tmp, Path)) return Fail(TEXT("Cannot replace the save file. Previous progress and its backup remain available."));
	return true;
}
}
