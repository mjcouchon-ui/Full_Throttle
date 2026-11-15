// Original boss breakdown paintings, studio pass 2. The simulation owns timings;
// these exposure charts add drawings inside its existing windows.
namespace FTBrawlStudio
{
static constexpr const TCHAR* BossFrameNames[] = {
	TEXT("bwalkdown"), TEXT("bwalkpass"), TEXT("bwalklift"), TEXT("bwalkpush"),
	TEXT("bsigload"), TEXT("bsigbridge"), TEXT("bsigreach"), TEXT("bsigsettle"),
	TEXT("bwindbridge"), TEXT("bstrikebridge"), TEXT("bretract"), TEXT("bhitbridge"),
	TEXT("bidleout"), TEXT("bidlein"), TEXT("bkneel"), TEXT("bentrance")
};

static UTexture* SelectBossFrame(const FString& Who, const FString& S, float T, float Walk, int32 Combo,
	const TMap<FName, TObjectPtr<UTexture>>& Frames)
{
	const bool bEarl = Who == TEXT("Earl"), bVinnie = Who == TEXT("Vinnie"), bPit = Who == TEXT("Pitboss");
	const bool bFrankie = Who == TEXT("Frankie"), bKing = Who == TEXT("Kingpin"), bSven = Who == TEXT("Sven");
	const bool bCommodore = Who == TEXT("Commodore");
	if (!(bEarl || bVinnie || bPit || bFrankie || bKing || bSven || bCommodore)
		|| !Frames.Contains(TEXT("bwalkdown"))) return nullptr;
	auto Get = [&](const TCHAR* Key) -> UTexture* { return Frames.FindRef(FName(Key)); };
	auto Gait = [&](float Phase) -> UTexture* {
		static const TCHAR* Steps[] = { TEXT("walk1"), TEXT("bwalkdown"), TEXT("bwalkpass"), TEXT("walk2"),
			TEXT("bwalklift"), TEXT("bwalkpush") };
		return Get(Steps[FMath::Clamp(FMath::FloorToInt(FMath::Frac(FMath::Max(0.f, Phase)) * 6.f), 0, 5)]);
	};
	if (S == TEXT("walk")) return Gait(Walk);
	if (S == TEXT("charge") || S == TEXT("run") || S == TEXT("dash")) return Gait(T * 3.f);
	if (S == TEXT("idle"))
	{
		const float Breath = FMath::Fmod(FMath::Max(T, 0.f), 1.6f);
		return Get(Breath < 0.3f ? TEXT("idle") : Breath < 0.65f ? TEXT("bidleout")
			: Breath < 1.f ? TEXT("idle") : Breath < 1.35f ? TEXT("bidlein") : TEXT("idle"));
	}
	if (S == TEXT("intro"))
		return Get(T < 0.16f ? TEXT("bidlein") : T < 0.4f ? TEXT("intro") : T < 1.8f ? TEXT("bentrance")
			: T < 2.06f ? TEXT("bidleout") : TEXT("intro2"));
	if (S == TEXT("intro2")) return Get(T < 2.64f ? TEXT("bidlein") : TEXT("idle"));
	if (S == TEXT("defeat")) return Get(T < 0.12f ? TEXT("bhitbridge") : TEXT("bkneel"));
	if (S == TEXT("getup")) return Get(T < 0.18f ? TEXT("bkneel") : T < 0.34f ? TEXT("bidlein") : TEXT("idle"));
	if (S == TEXT("hurt")) return Get(T < 0.07f ? TEXT("hurt") : T < 0.19f ? TEXT("bhitbridge") : TEXT("bretract"));
	if (S == TEXT("wind")) return T < 0.1f ? Get(TEXT("bwindbridge")) : nullptr;
	// Swing damage is applied on entry. Keep the established contact key on that tick.
	if (S == TEXT("swing") && T >= 0.07f)
		return Get(T < 0.13f ? TEXT("bstrikebridge") : TEXT("bretract"));
	if (S == TEXT("aim") && T < 0.16f) return Get(TEXT("bidlein"));
	if (bEarl)
	{
		if (S == TEXT("slamwind")) return Get(T < 0.10f ? TEXT("bsigload") : T < 0.22f ? TEXT("signatureprep")
			: T < 0.36f ? TEXT("bsigbridge") : T < 0.46f ? TEXT("jump") : TEXT("bsigreach"));
		if (S == TEXT("slam")) return Get(T < 0.16f ? TEXT("slam") : T < 0.32f ? TEXT("bsigsettle")
			: T < 0.52f ? TEXT("signaturefollow") : TEXT("bidlein"));
	}
	if (bVinnie)
	{
		if (S == TEXT("whirltell")) return Get(T < 0.15f ? TEXT("bsigload") : T < 0.58f ? TEXT("taunt")
			: T < 0.72f ? TEXT("bsigbridge") : TEXT("bsigreach"));
		if (S == TEXT("whirl"))
		{
			static const TCHAR* Spin[] = { TEXT("spin"), TEXT("bsigbridge"), TEXT("spinb"), TEXT("bsigreach") };
			return Get(Spin[FMath::FloorToInt(FMath::Max(0.f, T) / 0.045f) % 4]);
		}
		if (S == TEXT("signaturefollow")) return Get(T < 0.18f ? TEXT("bsigsettle") : TEXT("signaturefollow"));
		if (S == TEXT("dizzy")) return Get(FMath::Fmod(T, 0.5f) < 0.25f ? TEXT("dizzy") : TEXT("bhitbridge"));
	}
	if (bPit)
	{
		if (S == TEXT("draw")) return Get(T < 0.16f ? TEXT("bsigload") : T < 0.3f ? TEXT("bsigbridge")
			: T < 0.42f ? TEXT("draw") : T < 0.6f ? TEXT("bsigreach") : TEXT("aim"));
		if (S == TEXT("fire"))
		{
			// Actual shots at 0, .22, .44. Settle after each 70 ms muzzle flash.
			const float Beat = FMath::Fmod(FMath::Max(T, 0.f), 0.22f);
			return Get(T >= 0.6f ? TEXT("signaturefollow") : Beat < 0.07f ? TEXT("fire")
				: Beat < 0.14f ? TEXT("bsigsettle") : TEXT("aim"));
		}
		if (S == TEXT("signaturefollow")) return Get(T < 0.72f ? TEXT("bsigsettle") : TEXT("signaturefollow"));
	}
	if (bFrankie)
	{
		if (S == TEXT("swig")) return Get(T < 0.14f ? TEXT("bsigload") : T < 0.3f ? TEXT("bsigbridge")
			: T < 0.66f ? TEXT("swig") : TEXT("bsigreach"));
		// The breath is damaging throughout .9 s. Preserve its flame-bearing key.
		if (S == TEXT("breathe")) return T < 0.83f ? Get(TEXT("breathe")) : Get(TEXT("bsigsettle"));
	}
	if (bKing)
	{
		if (S == TEXT("hold")) return Get(T < 0.18f ? TEXT("bsigload") : T < 0.6f ? TEXT("hold")
			: T < 0.82f ? TEXT("bsigbridge") : TEXT("bsigreach"));
		// Balls release at 0, .12, .24; keep the empty-hand release pose until all are away.
		if (S == TEXT("bowl")) return Get(T < 0.31f ? TEXT("bowl") : T < 0.48f ? TEXT("bsigsettle")
			: T < 0.6f ? TEXT("signaturefollow") : TEXT("bidlein"));
	}
	if (bSven)
	{
		if (S == TEXT("gloves")) return Get(T < 0.22f ? TEXT("bsigload") : T < 0.52f ? TEXT("gloves") : TEXT("bsigbridge"));
		if (S == TEXT("lunge")) return Get(T < 0.10f ? TEXT("bsigreach") : T < 0.58f ? TEXT("lunge") : TEXT("bsigsettle"));
		if (S == TEXT("hug")) return Get(T < 0.12f ? TEXT("bsigreach") : T < 0.28f ? TEXT("bsigsettle") : TEXT("hug"));
		if (S == TEXT("dizzy")) return Get(FMath::Fmod(T, 0.5f) < 0.25f ? TEXT("bhitbridge") : TEXT("bidleout"));
	}
	if (bCommodore)
	{
		if (S == TEXT("windup")) return Get(T < 0.16f ? TEXT("bsigload") : T < 0.36f ? TEXT("bsigbridge")
			: T < 0.82f ? TEXT("windup") : TEXT("bsigreach"));
		// The anchor connects at .08. The downward bridge occupies the preceding interval.
		if (S == TEXT("sweep")) return Get(T < 0.07f ? TEXT("bsigreach") : T < 0.19f ? TEXT("sweep")
			: T < 0.31f ? TEXT("bsigsettle") : TEXT("signaturefollow"));
	}
	return nullptr;
}
}
