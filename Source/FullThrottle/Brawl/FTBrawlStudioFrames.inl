// Exposure charts for original studio-pass drawings. Gameplay keeps ownership
// of action duration, contacts, cancellation, and locomotion displacement.
namespace FTBrawlStudio
{
static UTexture* SelectPlayerStreetFrame(const FString& Who, const FString& S, float T, float Walk,
	int32 Combo, float StateAge, FName PreviousState, float TurnAge,
	const TMap<FName, TObjectPtr<UTexture>>& Frames)
{
	auto Get = [&](const TCHAR* Key) -> UTexture* { return Frames.FindRef(FName(Key)); };
	auto Cycle = [&](std::initializer_list<const TCHAR*> Keys, float Phase) -> UTexture* {
		const int32 Count = static_cast<int32>(Keys.size());
		const int32 Index = FMath::Clamp(FMath::FloorToInt(FMath::Frac(FMath::Max(Phase, 0.f)) * Count), 0, Count - 1);
		return Get(*(Keys.begin() + Index));
	};
	if (Who == TEXT("Nova") && Frames.Contains(TEXT("nwalk01")))
	{
		if ((S == TEXT("idle") || S == TEXT("walk")) && StateAge < 0.10f
			&& (PreviousState == TEXT("jump") || PreviousState == TEXT("jumpkick")))
			return Get(StateAge < 0.05f ? TEXT("nair04") : TEXT("nidle01"));
		if (S == TEXT("walk"))
		{
			if (TurnAge >= 0.f && TurnAge < 0.065f) return Get(TEXT("turn"));
			if (StateAge < 0.06f && PreviousState == TEXT("idle")) return Get(TEXT("nstart"));
			return Cycle({ TEXT("walk1"), TEXT("nwalk01"), TEXT("nwalk02"), TEXT("walk2"),
				TEXT("nwalk03"), TEXT("nwalk04"), TEXT("walk3"), TEXT("nwalk05"),
				TEXT("nwalk06"), TEXT("walk2"), TEXT("nwalk07"), TEXT("nwalk08") }, Walk);
		}
		if (S == TEXT("run") || S == TEXT("dash"))
			return Cycle({ TEXT("nrun01"), TEXT("nrun02"), TEXT("nrun03"), TEXT("nrun04"),
				TEXT("nrun05"), TEXT("nrun06"), TEXT("nrun07"), TEXT("nrun08") }, T / 0.3f);
		if (S == TEXT("idle"))
		{
			if (StateAge < 0.09f && (PreviousState == TEXT("walk") || PreviousState == TEXT("run"))) return Get(TEXT("nstop"));
			return Cycle({ TEXT("idle"), TEXT("nidle01"), TEXT("nidle02"), TEXT("nidle01") }, T / 1.5f);
		}
		if (S == TEXT("punch"))
		{
			if (Combo == 3)
				return Get(T < 0.03f ? TEXT("nupper01") : T < 0.06f ? TEXT("nupper02")
					: T < 0.14f ? TEXT("upper") : T < 0.21f ? TEXT("nupper03") : TEXT("nupper04"));
			if (Combo == 2)
				return Get(T < 0.03f ? TEXT("ncross01") : T < 0.06f ? TEXT("ncross02")
					: T < 0.10f ? TEXT("cross") : T < 0.14f ? TEXT("ncross03") : TEXT("ncross04"));
			return Get(T < 0.03f ? TEXT("njab01") : T < 0.06f ? TEXT("njab02")
				: T < 0.10f ? TEXT("jab") : T < 0.14f ? TEXT("njab03") : TEXT("njab04"));
		}
		if (S == TEXT("kick"))
			return Get(T < 0.06f ? TEXT("nkick01") : T < 0.12f ? TEXT("nkick02")
				: T < 0.24f ? TEXT("kick") : T < 0.32f ? TEXT("nkick03") : TEXT("nkick04"));
		if (S == TEXT("jump"))
			return Get(T < 0.034f ? TEXT("njump01") : T < 0.11f ? TEXT("njump02") : T < 0.24f ? TEXT("jump")
				: T < 0.43f ? TEXT("njump03") : T < 0.60f ? TEXT("njump04") : TEXT("jumpfall"));
		if (S == TEXT("jumpkick"))
			return Get(T < 0.06f ? TEXT("jumpkick") : T < 0.14f ? TEXT("nair02")
				: T < 0.21f ? TEXT("nair01") : TEXT("nair03"));
		if (S == TEXT("pickup"))
			return Get(T < 0.055f ? TEXT("nreach01") : T < 0.12f ? TEXT("nreach02")
				: T < 0.18f ? TEXT("nreach03") : TEXT("nweapon04"));
		if (S == TEXT("lift"))
			return Get(T < 0.05f ? TEXT("nreach01") : T < 0.10f ? TEXT("nreach02")
				: T < 0.16f ? TEXT("nreach03") : TEXT("nreach04"));
		if (S == TEXT("carry"))
			return Cycle({ TEXT("ncarry01"), TEXT("ncarry02"), TEXT("ncarry03"), TEXT("ncarry04") }, Walk);
		if (S == TEXT("drink"))
			return Get(T < 0.12f ? TEXT("ndrink01") : T < 0.28f ? TEXT("ndrink02")
				: T < 0.78f ? TEXT("ndrink03") : TEXT("ndrink04"));
		if (S == TEXT("wpn") || S == TEXT("toss") || S == TEXT("hurl"))
		{
			const float Contact = S == TEXT("hurl") ? 0.10f : S == TEXT("toss") ? 0.14f : 0.13f;
			return Get(T < Contact * 0.5f ? TEXT("nweapon01") : T < Contact ? TEXT("nweapon02")
				: T < Contact + 0.08f ? TEXT("bash") : T < 0.275f ? TEXT("nweapon03") : TEXT("nweapon04"));
		}
		if (S == TEXT("hurt"))
			return Get(T < 0.06f ? TEXT("hurt") : T < 0.12f ? TEXT("nreact01")
				: T < 0.23f ? TEXT("nreact02") : TEXT("nidle01"));
		if (S == TEXT("fly")) return T >= 0.07f && T < 0.22f ? Get(TEXT("nreact01")) : nullptr;
		if (S == TEXT("getup"))
			return Get(T < 0.10f ? TEXT("nreact03") : T < 0.19f ? TEXT("getup")
				: T < 0.28f ? TEXT("nreact04") : TEXT("nidle01"));
		return nullptr;
	}
	if (!Frames.Contains(TEXT("swalk01"))) return nullptr;
	auto Gait = [&](float Phase) -> UTexture* {
		return Cycle({ TEXT("walk1"), TEXT("swalk01"), TEXT("swalk02"), TEXT("walk2"),
			TEXT("swalk03"), TEXT("swalk04") }, Phase);
	};
	if (S == TEXT("walk"))
	{
		if ((TurnAge >= 0.f && TurnAge < 0.065f) || (StateAge < 0.07f && PreviousState == TEXT("idle"))) return Get(TEXT("sturn"));
		return Gait(Walk);
	}
	if (S == TEXT("run") || S == TEXT("dash") || S == TEXT("charge")) return Gait(T * 3.f);
	if (S == TEXT("idle"))
	{
		if (StateAge < 0.09f && PreviousState == TEXT("walk")) return Get(TEXT("sstop"));
		return Cycle({ TEXT("idle"), TEXT("sidle01"), TEXT("sidle02"), TEXT("sidle01") }, T / 1.4f);
	}
	if (S == TEXT("wind") || S == TEXT("aim"))
		return Get(T < 0.07f ? TEXT("swind01") : T < 0.16f ? TEXT("swind02") : TEXT("wind"));
	if (S == TEXT("swing") && !(Who == TEXT("Guard") && Combo != 1))
		return T < 0.07f ? nullptr : Get(T < 0.14f ? TEXT("sattack01") : TEXT("srecover01"));
	if (S == TEXT("grab") && Who == TEXT("Biker"))
		return Get(T < 0.07f ? TEXT("swind01") : T < 0.14f ? TEXT("swind02")
			: T < 0.24f ? TEXT("sattack01") : TEXT("grab"));
	if (S == TEXT("knee") && T > 0.32f) return Get(TEXT("srecover01"));
	if (S == TEXT("throw"))
	{
		if (Who == TEXT("Dealer"))
			return Get(T < 0.09f ? TEXT("swind01") : T < 0.22f ? TEXT("swind02") : T < 0.30f ? TEXT("throw")
				: T < 0.36f ? TEXT("sattack01") : T < 0.42f ? TEXT("swind02") : T < 0.50f ? TEXT("throw")
				: T < 0.59f ? TEXT("sattack01") : TEXT("srecover01"));
		return Get(T < 0.10f ? TEXT("swind01") : T < 0.32f ? TEXT("swind02")
			: T < 0.42f ? TEXT("throw") : T < 0.54f ? TEXT("sattack01") : TEXT("srecover01"));
	}
	if (S == TEXT("hurt"))
		return Get(T < 0.06f ? TEXT("hurt") : T < 0.12f ? TEXT("shit01")
			: T < 0.21f ? TEXT("shit02") : TEXT("srecover01"));
	if (S == TEXT("fly")) return T >= 0.07f && T < 0.22f ? Get(TEXT("shit01")) : nullptr;
	if (S == TEXT("getup"))
		return Get(T < 0.14f ? TEXT("sgetup01") : T < 0.23f ? TEXT("getup")
			: T < 0.36f ? TEXT("sgetup02") : TEXT("sidle01"));
	return nullptr;
}
}
