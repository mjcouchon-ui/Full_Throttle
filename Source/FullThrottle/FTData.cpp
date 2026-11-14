// FULL THROTTLE level data, ported from the 1.7.2 web game (track.js, engine.js, director.js).
#include "FTCore.h"

DEFINE_LOG_CATEGORY(LogFT);

FLinearColor FTData::Hex(const TCHAR* H)
{
	return FLinearColor(FColor::FromHex(H));
}

// Section list format: "len[:curve:bank][:shape[:a[:b]]]" separated by '|'.
static TArray<FFTSection> ParseSections(const TCHAR* Src)
{
	TArray<FFTSection> Out;
	TArray<FString> Parts;
	FString(Src).ParseIntoArray(Parts, TEXT("|"), true);
	for (FString P : Parts)
	{
		P.TrimStartAndEndInline();
		TArray<FString> T;
		P.ParseIntoArray(T, TEXT(":"), true);
		if (T.Num() == 0) continue;
		FFTSection S;
		S.Len = FCString::Atof(*T[0]);
		int32 NumIdx = 0, ParIdx = 0;
		bool bShape = false;
		for (int32 i = 1; i < T.Num(); ++i)
		{
			const FString& K = T[i];
			if (!bShape && (FChar::IsAlpha(K[0])))
			{
				bShape = true;
				if (K == TEXT("hill")) S.Shape = EFTSec::Hill;
				else if (K == TEXT("rise")) S.Shape = EFTSec::Rise;
				else if (K == TEXT("fall")) S.Shape = EFTSec::Fall;
				else if (K == TEXT("loop")) S.Shape = EFTSec::Loop;
				else if (K == TEXT("twist")) S.Shape = EFTSec::Twist;
				else if (K == TEXT("pipe")) S.Shape = EFTSec::Pipe;
				else if (K == TEXT("jump")) S.Shape = EFTSec::Jump;
				else if (K == TEXT("half")) S.Shape = EFTSec::HalfPipe;
				else if (K == TEXT("coaster")) S.Shape = EFTSec::Coaster;
				else if (K == TEXT("bldg")) S.Shape = EFTSec::Building;
				continue;
			}
			const float V = FCString::Atof(*K);
			if (!bShape) { if (NumIdx == 0) S.Curve = V; else S.Bank = V; ++NumIdx; }
			else { if (ParIdx == 0) S.A = V; else S.B = V; ++ParIdx; }
		}
		if (S.Shape == EFTSec::Pipe && S.B == 0.f) S.B = 1.f;
		Out.Add(S);
	}
	return Out;
}

static EFTKind KindOf(TCHAR C)
{
	switch (C)
	{
	case 'm': return EFTKind::Moto;   case 'r': return EFTKind::Rammer; case 's': return EFTKind::Suv;
	case 't': return EFTKind::Truck;  case 'h': return EFTKind::Heli;   case 'g': return EFTKind::Hog;
	case 'p': return EFTKind::Ramp;   case 'c': return EFTKind::Crate;  case 'w': return EFTKind::Wrench;
	case 'B': return EFTKind::Boss;
	default: return EFTKind::None;
	}
}

static FFTScene Scene(const TCHAR* Name, float D, const TCHAR* Spawns, const TCHAR* Intro, const TCHAR* Retake)
{
	FFTScene S;
	S.Name = Name; S.D = D; S.Intro = Intro; S.Retake = Retake;
	TArray<FString> Parts;
	FString(Spawns).ParseIntoArrayWS(Parts);
	for (const FString& P : Parts)
	{
		FString L, R;
		if (!P.Split(TEXT(":"), &L, &R)) continue;
		FFTSpawn Sp; Sp.D = FCString::Atof(*L);
		for (TCHAR C : R) { const EFTKind K = KindOf(C); if (K != EFTKind::None) Sp.Kinds.Add(K); }
		S.Spawns.Add(Sp);
	}
	return S;
}

static TArray<FFTLevelDef> BuildLevels()
{
	using namespace FTData;
	TArray<FFTLevelDef> L;
	L.SetNum(8);

	// ---------------------------------------------------------------- L1 BAYSHORE
	{
		FFTLevelDef& V = L[0];
		V.Index = 0; V.Zone = TEXT("BAYSHORE"); V.Title = TEXT("THE HIGHWAY ASSASSINATION");
		V.Place = TEXT("ROUTE 9 COASTAL HIGHWAY · 03:00 HRS"); V.Map = TEXT("/Game/Maps/Bayshore/L_Bayshore");
		V.Opening = ParseSections(TEXT(
			"1200|1600:-.35:-.12:hill:.05:2|900:.7:.25|900:-.8:-.3:hill:.1:1|700:rise:.16|"
			"1300:.5:.2:hill:.12:3|800:-.6:-.25:fall:.14|1600:1.6:.95|2000:hill:.07:1|"
			"900:rise:.22|1700:jump:.32:.40|400|2100:loop|300|1400:.9:.4|"
			"1000:-.9:-.4:rise:.2|1500:.45:.18:hill:.06:2|500|1300:twist:1|1200:.6:.25:fall:.167|"
			"1000:-.5:-.2:rise:.27|400|2000:jump:.36:.45|600|1700:-1.7:-1.0|"
			"1400:.7:.3:hill:.05:1|500|2100:loop|500:1.1:.5|1000:-1:-.45|"
			"1500:.4:.15|1800:-.6:-.25:hill:.08:2|900:rise:.18|1800:jump:.30:.38|600|"
			"1600:1.5:.9|1400:-.5:-.2|1400|2000:hill:.06:3|1200:.8:.35|1200:-.8:-.35|"
			"500|2100:loop|400|1500:.5:.2:rise:.15|1300:twist:-1|800:-.4:-.15:fall:.15|"
			"1800:-.4:-.15:hill:.05:2|1700:-1.6:-.95|800|900:rise:.22|1700:jump:.32:.40|"
			"500|1600:.7:.3:hill:.08:2|2000|1400:.9:.4:rise:.2|1400:-.9:-.4:hill:.1:2|"
			"1400:1:.45|1400:-1:-.45:fall:.17|600|1300:twist:1|500|2100:loop|900:.3:.1|"
			"1600:hill:.07:2|1700:1.7:1.0|700|1000:rise:.27|400|2000:jump:.36:.45|"
			"600|1500:-.6:-.25:hill:.06:1|1500:.4:.15|1500:-.8:-.35|1500:.8:.35:hill:.06:2|"
			"500|2100:loop|300|1300:twist:-1|800:fall:.15|1000:1.1:.5|1000:-1:-.45|1000|"
			"1600:hill:.05:3|1500:.9:.4|1500:-.9:-.4|800:rise:.2|1700:jump:.32:.40|500|2100:loop|"
			"1300:.4:.15|1700:-1.6:-.95|800|1300:twist:1|1400:.8:.35:hill:.08:2|"
			"1400:-.8:-.35|500|2100:loop|800|1000"));
		V.Circuit = ParseSections(TEXT("1500:.8:.35:hill:.1:2|1200:-1.2:-.7|600|2100:loop|800:rise:.2|1400:1.4:.8|600|1300:twist:-1|1000:-.6:-.25:fall:.16"));
		V.FinaleD = 122500.f;
		V.Scenes = {
			Scene(TEXT("THE GETAWAY"), 0, TEXT(""), TEXT("Scene one. You have the codebook. Drive like they want it back!"), TEXT("Weave that traffic. Make it look dangerous!")),
			Scene(TEXT("THE TAIL"), 3300, TEXT("3300:mm 5200:mr"), TEXT("Bikers on your tail, and the road goes up and over! Take them out!"), TEXT("Guns up over those humps. Don't let them close in!")),
			Scene(TEXT("THE AMBUSH"), 7200, TEXT("7200:rr"), TEXT("Rammers on the banked turn! Ride the wall and make them miss!"), TEXT("Up on the bank! Dodge the rammers!")),
			Scene(TEXT("THE JUMP"), 9200, TEXT("9200:p 10400:s 11800:c 12600:mmm 14400:sr"), TEXT("Car carrier, then the road just ENDS! Turbo and fly the gap!"), TEXT("Ramp, then the gap, then the loop. Give me air!")),
			Scene(TEXT("THE CONVOY"), 16400, TEXT("16400:t 17200:p 19300:sm 21300:rrm"), TEXT("Armored truck! Missiles, then hang on for the corkscrew!"), TEXT("That truck is the shot. Crack it before the corkscrew!")),
			Scene(TEXT("CROSSFIRE"), 23300, TEXT("23300:ss 24500:wc 25700:tmm 26800:h"), TEXT("Gunships! Shoot back, then fly the big gap!"), TEXT("Crossfire, then the big jump. Keep moving!")),
			Scene(TEXT("THE WOLF PACK"), 28300, TEXT("28300:rrr 29700:mm 31500:pc 32700:sm"), TEXT("Three rammers and another loop. Make me gasp!"), TEXT("The wolf pack. Thread them, then loop it!")),
			Scene(TEXT("THE PIER"), 34500, TEXT("34500:mmr 36200:s 37600:c 38500:h 41100:rr 43000:t 44600:w"), TEXT("The pier! Clear the boardwalk, then fly off the end!"), TEXT("Off the end of the pier. Turbo and FLY!")),
			Scene(TEXT("NEON MILE"), 45500, TEXT("45500:sm 46800:p 48500:rmm 50000:h 51800:ss 53600:cw 55000:mmm"), TEXT("Neon Mile! Every sign in town is on. Make it glow!"), TEXT("Loop it, roll it, light them up!")),
			Scene(TEXT("THE MARINA"), 56500, TEXT("56500:tm 58300:rr 60500:c 61500:h 62500:sm 64000:p 65500:rs"), TEXT("The marina! Armor on the docks and a gap over the slips!"), TEXT("Crack that truck, then jump the boats!")),
			Scene(TEXT("THE CLIFF ROAD"), 67500, TEXT("67500:mmmm 69500:s 70500:h 71500:rr 73000:wc 75500:t 77300:mm"), TEXT("The cliff road! Bikers on every bend. Hold your line!"), TEXT("Esses, a corkscrew and a loop. Don't look down!")),
			Scene(TEXT("THE LIGHTHOUSE"), 78500, TEXT("78500:ssm 80100:rrr 82300:c 84800:tm 86500:pw 88000:sr"), TEXT("Gunships on the point! Then the biggest gap on the coast!"), TEXT("Ride the wall round the lighthouse and FLY!")),
			Scene(TEXT("THE LAST MILE"), 89500, TEXT("89500:rrmm 91500:ss 93000:cw 93800:h 95500:tr 97500:mmm 99000:sc"), TEXT("The last mile! Everything they've got. Give me everything you've got!"), TEXT("Last loop before the big man. Make it count!")),
			Scene(TEXT("THE CAUSEWAY"), 100500, TEXT("100500:smm 102300:p 104200:rr 106000:cw 108500:tm 110300:s"), TEXT("The causeway! The drawbridge is up. Fly it!"), TEXT("Over the bridge and round the loop. Go!")),
			Scene(TEXT("SUNSET STRIP"), 111500, TEXT("111500:rrr 113800:mms 115000:h 116000:c 117500:tr 119500:wc 120800:smm"), TEXT("Sunset Strip! The whole town is watching. Show off!"), TEXT("Ride the wall, roll it, loop it!")),
			Scene(TEXT("THE FINALE"), 122500, TEXT("122500:B"), TEXT("There he is. The Juggernaut. Take him DOWN!"), TEXT("Final shot. Everything you've got on the Juggernaut!")),
		};
		V.Rating[1] = 90000; V.Rating[2] = 160000; V.Rating[3] = 230000;
		V.Boss = EFTBoss::Juggernaut; V.BossName = TEXT("THE JUGGERNAUT"); V.BossHp = 120.f; V.BossEscape = 70.f;
		// 80s night on the coast: violet asphalt, white lines, hot pink and cyan neon
		V.RoadA = Hex(TEXT("2a2742")); V.RoadB = Hex(TEXT("26233c")); V.Line = Hex(TEXT("fffaf0"));
		V.KerbA = Hex(TEXT("ff4fb8")); V.KerbB = Hex(TEXT("fffaf0")); V.Rail = Hex(TEXT("2ee6ff")); V.Neon2 = Hex(TEXT("ff4fb8"));
		V.Sky0 = Hex(TEXT("0b0826")); V.Sky1 = Hex(TEXT("3a1a6e")); V.Sky2 = Hex(TEXT("ff4f9a")); V.Fog = Hex(TEXT("5a2a7a"));
		V.Gantries = { TEXT("BAYSHORE"), TEXT("PIER 2 MI"), TEXT("SUNSET STRIP"), TEXT("ROUTE 9 NORTH"), TEXT("MARINA EXIT 4"), TEXT("NEON MILE"), TEXT("LIGHTHOUSE PT"), TEXT("CAUSEWAY") };
		V.Glow = FLinearColor(1.f, 0.31f, 0.72f); V.Lamp = FLinearColor(1.f, 0.75f, 0.55f); V.LampCd = 7000.f;
	}
	// ---------------------------------------------------------------- L2 VOLTA CHEMICAL WORKS
	{
		FFTLevelDef& V = L[1];
		V.Index = 1; V.Zone = TEXT("CHEMICAL PLANT"); V.Title = TEXT("THE FORMULA");
		V.Place = TEXT("VOLTA CHEMICAL WORKS · 23:00 HRS"); V.Map = TEXT("/Game/Maps/ChemicalWorks/L_ChemicalWorks");
		V.Opening = ParseSections(TEXT(
			"1000|1100:.9:.4|1100:-.9:-.4|1100:1.1:.5:hill:.06:1|1100:-1.1:-.5|600|1600:pipe:1:1|"
			"400|1000:.6:.25|1200:-.8:-.35:rise:.18|1200:.9:.4|500|2400:pipe:-1:2|500|"
			"1200:-1:-.45:fall:.15|1200:1:.45|1800:hill:.07:2|2600:1.7:1.0:rise:.22|600|"
			"2100:loop|400|1300:twist:1|2600:-1.7:-1.0:fall:.2|400|1000:.8:.35|1000:-.8:-.35|"
			"900:rise:.22|1700:jump:.32:.40|500|1800:pipe:1:1|500|1300:1.2:.6:hill:.06:1|1300:-1.2:-.6|"
			"800|2400:pipe:-1:2|400|2100:loop|300|1800:pipe:1:1|1200:.9:.4:fall:.15|1000:-.6:-.25|"
			"1100:1:.45|1100:-1:-.45|1100:1:.45:hill:.08:1|1100:-1:-.45|1000:rise:.27|400|2000:jump:.36:.45|"
			"600|1600:1.6:.95|1000:-.5:-.2|2800:pipe:1:3|400|1300:twist:-1|500|2100:loop|"
			"900:.8:.35|1000:-.8:-.35"));
		V.Arena = ParseSections(TEXT("4000:.35:.15|4000:-.35:-.15|4000:.3:.12:hill:.035:1|4000:-.3:-.12|4000:.35:.15|4000:-.35:-.15|4000:.25:.1:hill:.03:1|4000:-.25:-.1|4000"));
		V.Circuit = ParseSections(TEXT("1200:1:.45|1200:-1:-.45:hill:.06:1|600|1800:pipe:1:1|500|2100:loop|400|1400:1.5:.9|1000:-.6:-.25"));
		V.FinaleD = 69000.f;
		V.Scenes = {
			Scene(TEXT("THE FACTORY GATE"), 0, TEXT("1500:mm 3200:r 4600:s 7800:c"), TEXT("Scene two. The chemical plant. Get the formula and get OUT!"), TEXT("Through the gate and into the tubes!")),
			Scene(TEXT("THE TUBES"), 9000, TEXT("9000:mmr 11000:s 11500:h 15000:rr 16500:cw 17500:sm"), TEXT("The tubes! The road goes all the way round. Keep driving!"), TEXT("Twice round the pipe. Don't lift!")),
			Scene(TEXT("SPIN CYCLE"), 19000, TEXT("19000:rr 21000:mm 22500:h 25500:sm 27500:c"), TEXT("Up the helix, round the loop, down again! Spin it!"), TEXT("The spin cycle. Ride the bank!")),
			Scene(TEXT("THE REACTOR"), 29000, TEXT("29000:t 30500:p 32000:h 33500:ss 35300:wc 36500:rmm"), TEXT("Armored tanker on the line! Crack it, then fly the pond!"), TEXT("Missiles on the truck, then the tube!")),
			Scene(TEXT("PIPE DREAMS"), 39000, TEXT("39000:mmm 43000:sr 44500:h 46500:c 47500:t"), TEXT("Two pipes and a loop. The crew built this for you!"), TEXT("Round and round. Keep shooting upside down!")),
			Scene(TEXT("MEGA VAT"), 49000, TEXT("49000:rrr 51000:sm 53000:pc 56500:w 57400:ss"), TEXT("Esses round the vats, then jump the big one. Don't fall in!"), TEXT("Over the Mega Vat. Turbo off the lip!")),
			Scene(TEXT("THE TOXIC RUN"), 59000, TEXT("59000:tmm 61000:h 64000:rs 66000:cw 67500:mmr"), TEXT("Three times round the big pipe! Make me dizzy!"), TEXT("The toxic run. Hold it together!")),
			Scene(TEXT("THE FINALE"), 69000, TEXT("69000:B"), TEXT("The Alchemist's tanker! Blow it before he gets away!"), TEXT("Final shot. Everything on the tanker!")),
		};
		V.Rating[1] = 50000; V.Rating[2] = 90000; V.Rating[3] = 130000;
		V.Boss = EFTBoss::Alchemist; V.BossName = TEXT("THE ALCHEMIST"); V.BossHp = 120.f; V.BossEscape = 70.f;
		V.RoadA = Hex(TEXT("1f2550")); V.RoadB = Hex(TEXT("1b2148")); V.Line = Hex(TEXT("ffd23f"));
		V.KerbA = Hex(TEXT("ffd23f")); V.KerbB = Hex(TEXT("1c1f4a")); V.Rail = Hex(TEXT("ff9e3d")); V.Neon2 = Hex(TEXT("5ff0ff"));
		V.Sky0 = Hex(TEXT("05041a")); V.Sky1 = Hex(TEXT("2a2470")); V.Sky2 = Hex(TEXT("c25bd6")); V.Fog = Hex(TEXT("3a2476"));
		V.Gantries = { TEXT("VOLTA CHEMICAL WORKS"), TEXT("DANGER HIGH VOLTAGE"), TEXT("REACTOR"), TEXT("NO SMOKING"), TEXT("TANK FARM B"), TEXT("AUTHORISED ONLY") };
		V.Glow = FLinearColor(0.37f, 0.94f, 1.f); V.Lamp = FLinearColor(1.f, 0.62f, 0.25f); V.LampCd = 7000.f;
	}
	// ---------------------------------------------------------------- L3 NEVADA
	{
		FFTLevelDef& V = L[2];
		V.Index = 2; V.Zone = TEXT("NEVADA DESERT"); V.Title = TEXT("ALL IN");
		V.Place = TEXT("HIGHWAY 93 TO LAS VEGAS · 21:30 HRS"); V.Map = TEXT("/Game/Maps/Nevada/L_Nevada");
		V.Opening = ParseSections(TEXT(
			"1500|2400:hill:.05:3|1600:.35:.12|1600:-.35:-.12|1400:hill:.06:1|1200|"
			"900:rise:.2|1700:jump:.32:.40|600|2000:hill:.05:2|1400:.6:.25|1400:-.6:-.25|1800|"
			"2400:hill:.04:2|500|2100:loop|400|1500:.45:.18:hill:.05:1|1500:-.45:-.18|2600:hill:.06:3|"
			"1000:.8:.35|1000:-.8:-.35|900:rise:.22|1700:jump:.32:.40|600|1800:hill:.05:2|"
			"1700:1.6:.95|600|1300:twist:1|600|1700:-1.6:-.95|1000:fall:.12|1600:hill:.05:2|"
			"2000|1500:.5:.2|1500:-.5:-.2|1000:rise:.27|400|2000:jump:.36:.45|600|1500:hill:.06:2|"
			"1200:.9:.4|1200:-.9:-.4|500|2100:loop|300|1300:twist:-1|800:fall:.15|2100:hill:.05:3|"
			"1500:.4:.15|1500:-.4:-.15|800:rise:.2|1700:jump:.32:.40|500|2100:loop|900|1500:hill:.04:2|1500"));
		V.Arena = ParseSections(TEXT("5000|4000:.2:.08:hill:.03:1|4000:-.2:-.08|5000:hill:.03:2|4000:.25:.1|4000:-.25:-.1|6000|4000:.2:.08|4000:-.2:-.08"));
		V.Circuit = ParseSections(TEXT("3000:hill:.04:2|2000:.3:.12|2000:-.3:-.12|3000"));
		V.FinaleD = 78000.f;
		V.Scenes = {
			Scene(TEXT("THE STATE LINE"), 0, TEXT("1500:mm 3200:r 4800:s 6500:c 7300:rm"), TEXT("Scene three. The open road to Vegas. Floor it!"), TEXT("Nevada, baby. Make it look like a postcard!")),
			Scene(TEXT("THE DRY WASH"), 8500, TEXT("8500:mm 10500:s 13000:h 14500:rr 16000:cw"), TEXT("The bridge is out over the wash! Turbo and FLY!"), TEXT("Up the ramp and over the wash!")),
			Scene(TEXT("SNAKE EYES"), 17700, TEXT("17700:gg 19500:ggg 21500:s 23000:gg 25000:w 26200:gr"), TEXT("The Snake Eyes! Bikers on your doors. Slam them off the road!"), TEXT("Ram those choppers! Don't let them swing!")),
			Scene(TEXT("JOSHUA FLATS"), 27900, TEXT("27900:ss 29500:gg 31000:c 32500:t 35800:ggm 37000:w"), TEXT("Joshua Flats! Gunships in the trees. Keep it moving!"), TEXT("Through the trees and over the gap!")),
			Scene(TEXT("RED ROCK PASS"), 37500, TEXT("37500:rr 39000:gg 40500:h 42500:sg 44500:cw"), TEXT("Red Rock Pass! Ride the canyon wall and roll it!"), TEXT("Wall, corkscrew, wall. Hold it together!")),
			Scene(TEXT("LAST CHANCE GAS"), 46000, TEXT("46000:h 47500:ggg 49000:p 50200:t 55000:sr 56000:c"), TEXT("Last chance gas! Chopper over the pumps, then the big wash!"), TEXT("Blow the chopper, then fly the wash!")),
			Scene(TEXT("JACKPOT JUNCTION"), 56500, TEXT("56500:rrr 58000:gg 60000:h 62000:sgg 64500:wc 65500:mm"), TEXT("Jackpot Junction! Loop it, roll it, let it ride!"), TEXT("Double or nothing. Loop and corkscrew!")),
			Scene(TEXT("THE CITY LIGHTS"), 66000, TEXT("66000:gggg 68000:tm 71800:s 73500:h 75000:gg 76500:cw"), TEXT("There's Vegas! The whole Strip is lit up for us. Give me a show!"), TEXT("Neon on the paint. Loop it under the lights!")),
			Scene(TEXT("THE FINALE"), 78000, TEXT("78000:B"), TEXT("The Big Dealer's carrier! Every Snake Eyes in Nevada is on that truck. Take it DOWN!"), TEXT("Final shot. Shoot the truck, slam the bikes!")),
		};
		V.Rating[1] = 80000; V.Rating[2] = 150000; V.Rating[3] = 210000;
		V.Boss = EFTBoss::Dealer; V.BossName = TEXT("THE BIG DEALER"); V.BossHp = 200.f; V.BossEscape = 90.f;
		V.RoadA = Hex(TEXT("2c2438")); V.RoadB = Hex(TEXT("282033")); V.Line = Hex(TEXT("fff1d6"));
		V.KerbA = Hex(TEXT("ff7a3d")); V.KerbB = Hex(TEXT("fff1d6")); V.Rail = Hex(TEXT("2ee6ff")); V.Neon2 = Hex(TEXT("ff7a3d"));
		V.Sky0 = Hex(TEXT("100a30")); V.Sky1 = Hex(TEXT("5a2380")); V.Sky2 = Hex(TEXT("ff6a5a")); V.Fog = Hex(TEXT("7a3a6a"));
		V.Gantries = { TEXT("LAS VEGAS 38"), TEXT("US 93 NORTH"), TEXT("LAST GAS 50 MI"), TEXT("JACKPOT JUNCTION"), TEXT("LOOSEST SLOTS IN NEVADA"), TEXT("LAS VEGAS 12"), TEXT("WELCOME TO NEVADA"), TEXT("THE STRIP EXIT 7") };
		V.Glow = FLinearColor(1.f, 0.48f, 0.24f); V.Lamp = FLinearColor(1.f, 0.7f, 0.5f); V.LampCd = 5000.f;
		V.Backdrop = TEXT("/Game/Maps/Nevada/Props/SM_Vegas"); V.BackdropScale = 4.f;
	}
	// ---------------------------------------------------------------- L4 MOUNT KALDERA (2.5)
	{
		FFTLevelDef& V = L[3];
		V.Index = 3; V.Zone = TEXT("VOLCANO"); V.Title = TEXT("BAPTISM OF FIRE");
		V.Place = TEXT("MOUNT KALDERA PASS · 18:40 HRS"); V.Map = TEXT("/Game/Maps/Volcano/L_Volcano");
		V.Hint = TEXT("Jump the lava flows. Dodge the fire hose.");
		V.Opening = ParseSections(TEXT(
			"1400|1500:.4:.15:hill:.05:2|1200:-.6:-.25|600:rise:.12|1100:jump:.24:.30|500|1400:.9:.4|1200:-.9:-.4:hill:.08:1|"
			"900:rise:.2|1700:jump:.32:.40|500|1600:1.6:.95|600|1300:twist:1|1000:-.6:-.25:fall:.15|600:rise:.12|1100:jump:.24:.30|"
			"500|2100:loop|400|1400:.8:.35:hill:.07:2|1400:-.8:-.35|600:rise:.12|1100:jump:.24:.30|600:rise:.12|1100:jump:.24:.30|"
			"500|1700:-1.6:-.95|600|1000:rise:.27|400|2000:jump:.36:.45|600|1500:.6:.25:hill:.06:2|1300:twist:-1|800:fall:.15|"
			"1200:1:.45|1200:-1:-.45|600:rise:.12|1100:jump:.24:.30|500|2100:loop|500|1500:-.5:-.2:hill:.06:1|900:rise:.2|1700:jump:.32:.40|"
			"600|1600:1.5:.9|1300:twist:1|1000:-.8:-.35|600:rise:.12|1100:jump:.24:.30|600:rise:.12|1100:jump:.24:.30|500|2100:loop|900:.3:.1|1500"));
		V.Arena = ParseSections(TEXT("5000|4000:.25:.1:hill:.03:1|4000:-.25:-.1|5000|4000:.3:.12|4000:-.3:-.12|6000|4000:.2:.08|4000:-.2:-.08"));
		V.Circuit = ParseSections(TEXT("1500:.6:.25|600:rise:.12|1100:jump:.24:.30|600|1500:-.6:-.25|2000"));
		V.FinaleD = 60800.f;
		V.Scenes = {
			Scene(TEXT("THE ASH ROAD"), 0, TEXT("1500:mm 3200:r 6500:s"), TEXT("Scene four. The volcano is blowing its top. Drive right through it!"), TEXT("Through the ash and over the lava. Keep it floored!")),
			Scene(TEXT("LAVA JUMPS"), 8500, TEXT("8500:mm 11500:rs 13600:c 14000:h"), TEXT("Lava across the road! Hit the lip fast and FLY over it!"), TEXT("Jump the lava. Don't you dare land short!")),
			Scene(TEXT("THE CALDERA"), 18200, TEXT("18200:rr 21200:mm 22600:t 23200:w"), TEXT("Loop it on the rim of the crater! Three lava jumps after!"), TEXT("Round the rim and over the lava!")),
			Scene(TEXT("FIRE AND BRIMSTONE"), 27400, TEXT("27400:ss 29600:h 33600:rm 34200:wc"), TEXT("Lightning in the ash cloud! Keep moving, keep shooting!"), TEXT("Gunships in the lightning. Light them up!")),
			Scene(TEXT("THE MAGMA RUN"), 37800, TEXT("37800:mmr 39000:s 41900:tm 44500:c 45000:h"), TEXT("A lava jump, a loop and the big one. Give me fire on film!"), TEXT("The magma run. Fly it, loop it, fly it again!")),
			Scene(TEXT("OBSIDIAN PASS"), 49100, TEXT("49100:rrr 52600:sm 57000:wc"), TEXT("Black glass canyon! Ride the wall and corkscrew out!"), TEXT("Wall, corkscrew, two jumps and a loop. Go!")),
			Scene(TEXT("THE FINALE"), 60800, TEXT("60800:B"), TEXT("Engine 13! He's hosing the road with fire. Take him DOWN!"), TEXT("Final shot. Dodge the flames and crack his tank!")),
		};
		V.Rating[1] = 80000; V.Rating[2] = 145000; V.Rating[3] = 210000;
		V.Boss = EFTBoss::FireTruck; V.BossName = TEXT("ENGINE 13"); V.BossHp = 150.f; V.BossEscape = 80.f; V.BossBullet = 2;
		V.RoadA = Hex(TEXT("2c2230")); V.RoadB = Hex(TEXT("28202c")); V.Line = Hex(TEXT("ffd23f"));
		V.KerbA = Hex(TEXT("ff5a1f")); V.KerbB = Hex(TEXT("2c2230")); V.Rail = Hex(TEXT("ff7a2a")); V.Neon2 = Hex(TEXT("ffd23f"));
		V.Sky0 = Hex(TEXT("120608")); V.Sky1 = Hex(TEXT("4a1410")); V.Sky2 = Hex(TEXT("ff5a1f")); V.Fog = Hex(TEXT("5a2010"));
		V.Glow = FLinearColor(1.f, 0.42f, 0.1f); V.Lamp = FLinearColor(1.f, 0.5f, 0.2f); V.LampCd = 6000.f;
		V.Gantries = { TEXT("MOUNT KALDERA"), TEXT("DANGER LAVA"), TEXT("EVACUATION ROUTE"), TEXT("OBSIDIAN PASS"), TEXT("NO STOPPING") };
		V.bLava = true; V.LavaBands = { FVector2D(-5200.f, -3800.f), FVector2D(4600.f, 6100.f) }; V.bLightning = true;
		V.Backdrop = TEXT("/Game/Maps/Volcano/Props/SM_Volcano"); V.BackdropScale = 4.f;
	}
	// ---------------------------------------------------------------- L5 COASTER COVE (2.5)
	{
		FFTLevelDef& V = L[4];
		V.Index = 4; V.Zone = TEXT("THEME PARK"); V.Title = TEXT("THE LAST RIDE");
		V.Place = TEXT("COASTER COVE AMUSEMENT PARK · 21:00 HRS"); V.Map = TEXT("/Game/Maps/ThemePark/L_ThemePark");
		V.Hint = TEXT("Tighter and tighter. Then ride the coaster rails.");
		V.Opening = ParseSections(TEXT(
			"1400|1300:.6:.25|1300:-.6:-.25|1200:.9:.4:hill:.05:1|1200:-.9:-.4|600|1400:1.4:.8|500|1300:twist:1|1000:-1:-.45|"
			"1000:1.8:.9|900:-1.8:-.9|500|900:rise:.2|1700:jump:.32:.40|600|1100:2.1:1.0|1000:-2.1:-1.0|500|2100:loop|400|"
			"900:rise:.15|600|2400:coaster:0:.18|2600:coaster:1:.12|1800:coaster:0:.2|2600:coaster:2:0|1600:coaster:0:.15|2800:coaster:3:.1|"
			"1800:coaster:0:.2|2400:coaster:1:.1|1200:coaster:0:.1|600|800:fall:.15|"
			"1000:2.2:1.0|800:-2.4:-1.0|800:2.4:1.0|500|1300:twist:-1|900:-2:-.9|900:2:.9|500|2100:loop|500|1000:-2.2:-1.0|1000:2.2:1.0|"
			"1000:rise:.27|400|2000:jump:.36:.45|600|900:-1.8:-.8|900:1.8:.8|1500"));
		V.Arena = ParseSections(TEXT("5000|4000:.35:.15|4000:-.35:-.15|5000:hill:.03:1|4000:.3:.12|4000:-.3:-.12|6000|4000:.25:.1|4000:-.25:-.1"));
		V.Circuit = ParseSections(TEXT("1000:1.8:.9|1000:-1.8:-.9|600|2100:loop|600|1200:1.2:.5|1200:-1.2:-.5|1500"));
		V.FinaleD = 61400.f;
		V.Scenes = {
			Scene(TEXT("OPENING NIGHT"), 0, TEXT("1500:mm 3200:c 5200:r 7000:s"), TEXT("Scene five. The park's open late and the clowns are armed!"), TEXT("Opening night. Make it a show!")),
			Scene(TEXT("THE MIDWAY"), 8400, TEXT("8400:rr 11200:mm 13100:s 15000:w"), TEXT("The turns get tighter from here. Hug the inside and drift!"), TEXT("Drift the midway. Tighter!")),
			Scene(TEXT("HAIRPIN ALLEY"), 16200, TEXT("16200:rm 18900:sm 20400:h"), TEXT("Hairpins and a loop! Drift it or wear it!"), TEXT("Hairpins. Drift them all!")),
			Scene(TEXT("THE COASTER"), 22800, TEXT(""), TEXT("THE COASTER! Foot down. Hop across when the rails meet!"), TEXT("Ride the rails. Hop when they're side by side!")),
			Scene(TEXT("WILD MOUSE"), 42600, TEXT("42600:mmm 44000:r 46600:sm 49800:h"), TEXT("Back on the road and it's all hairpins! Steer, Kane, STEER!"), TEXT("Wild mouse. Every corner is a hairpin!")),
			Scene(TEXT("THE FUNHOUSE"), 52800, TEXT("52800:rr 54300:s 56300:wc 58700:mm"), TEXT("Crazy turns, a big jump, then the clown himself!"), TEXT("The funhouse. Jump it and get ready!")),
			Scene(TEXT("THE FINALE"), 61400, TEXT("61400:B"), TEXT("Mr. Chuckles! His balloons are full of bombs. Watch the shadows!"), TEXT("Final shot. Dodge the bombs, pop the clown!")),
		};
		V.Rating[1] = 75000; V.Rating[2] = 140000; V.Rating[3] = 200000;
		V.Boss = EFTBoss::Clown; V.BossName = TEXT("MR. CHUCKLES"); V.BossHp = 140.f; V.BossEscape = 80.f; V.BossBullet = 0;
		V.RoadA = Hex(TEXT("2a2546")); V.RoadB = Hex(TEXT("262140")); V.Line = Hex(TEXT("fff1d6"));
		V.KerbA = Hex(TEXT("ff4fb8")); V.KerbB = Hex(TEXT("ffd23f")); V.Rail = Hex(TEXT("ffd23f")); V.Neon2 = Hex(TEXT("2ee6ff"));
		V.Sky0 = Hex(TEXT("0b0826")); V.Sky1 = Hex(TEXT("3a1a6e")); V.Sky2 = Hex(TEXT("ff4f9a")); V.Fog = Hex(TEXT("4a2a6a"));
		V.Glow = FLinearColor(1.f, 0.82f, 0.25f); V.Lamp = FLinearColor(1.f, 0.8f, 0.5f); V.LampCd = 7000.f;
		V.Gantries = { TEXT("COASTER COVE"), TEXT("YOU MUST BE THIS TALL"), TEXT("THE MIDWAY"), TEXT("FUNHOUSE"), TEXT("NO RE-ENTRY") };
		V.Backdrop = TEXT("/Game/Maps/ThemePark/Props/SM_ParkSkyline"); V.BackdropScale = 4.f;
	}
	// ---------------------------------------------------------------- L6 FROSTBITE PEAK (2.5)
	{
		FFTLevelDef& V = L[5];
		V.Index = 5; V.Zone = TEXT("ICE MOUNTAIN"); V.Title = TEXT("WHITE OUT");
		V.Place = TEXT("FROSTBITE PEAK SKI RUN · 07:30 HRS"); V.Map = TEXT("/Game/Maps/Glacier/L_Glacier");
		V.Hint = TEXT("Downhill all the way. Ride the half pipe walls.");
		V.Opening = ParseSections(TEXT(
			"1500:.5:.2|1300:-.7:-.3|1300:.9:.4:hill:.05:2|1200:-1.1:-.5|600|1400:1.3:.6|1400:-1.3:-.6|500|3000:half:1|500|1200:1:.45|1200:-1:-.45|"
			"900:rise:.15|1700:jump:.3:.5|600|1300:twist:1|1000:1.6:.9|1000:-1.6:-.9|600|3200:half:1|600|1100:1.2:.5|1100:-1.2:-.5|1100:1.2:.5:hill:.06:2|500|2100:loop|500|"
			"1300:-1.5:-.8|1300:1.5:.8|800:rise:.15|1700:jump:.3:.5|600|1300:twist:-1|900:-.9:-.4|900:.9:.4|600|3600:half:1|600|1200:-1.4:-.6|1200:1.4:.6|1200:-1.4:-.6|500|2100:loop|400|"
			"1300:twist:1|800|1600:1.6:.95|1600:-1.6:-.95|900:rise:.15|1700:jump:.3:.5|600|3000:half:1|800|1500"));
		V.Arena = ParseSections(TEXT("5000:.3:.12|4000:-.35:-.15|4000:.35:.15|5000:-.25:-.1|4000:.3:.12|4000:-.3:-.12|6000|4000:.25:.1|4000:-.25:-.1"));
		V.Circuit = ParseSections(TEXT("1300:1.2:.5|1300:-1.2:-.5|600|3000:half:1|600|1500"));
		V.FinaleD = 65200.f;
		V.Scenes = {
			Scene(TEXT("FIRST TRACKS"), 0, TEXT("1500:mm 3200:r 5300:s 7300:c"), TEXT("Scene six. Top of the mountain, and it's downhill all the way!"), TEXT("First tracks. Carve it!")),
			Scene(TEXT("THE HALF PIPE"), 8700, TEXT("8700:mm 12700:rs 14400:w"), TEXT("Half pipe! Ride the walls like a snowboarder. Higher!"), TEXT("Up the walls! Higher!")),
			Scene(TEXT("AVALANCHE CHUTE"), 17700, TEXT("17700:rr 19600:sm 21000:h"), TEXT("Corkscrew down the chute! Keep the speed up!"), TEXT("Corkscrew, then the big pipe!")),
			Scene(TEXT("THE SUPER PIPE"), 21600, TEXT("21600:s 25400:mm 26000:wc"), TEXT("A bigger pipe! Up the wall, top of the world!"), TEXT("The super pipe. Ride it high!")),
			Scene(TEXT("BLACK DIAMOND"), 29300, TEXT("29300:rrr 31900:t 33700:p 35000:h 37500:cw"), TEXT("Black diamond run! Loop, jump, roll. Don't fall!"), TEXT("Black diamond. Hold it together!")),
			Scene(TEXT("THE WHITEOUT"), 41200, TEXT("41200:mm 45400:sr 47200:h 49600:wc 52200:mm 53900:rs 57000:c"), TEXT("Whiteout! Every gate on the mountain. Hold your line!"), TEXT("Whiteout. Gates, pipe, loop, jump!")),
			Scene(TEXT("THE FINALE"), 65200, TEXT("65200:B"), TEXT("THE ABOMINABLE! He's real and he's ANGRY. Dodge the snowballs!"), TEXT("Final shot. Dodge the snow and take the yeti down!")),
		};
		V.Rating[1] = 85000; V.Rating[2] = 150000; V.Rating[3] = 215000;
		V.Boss = EFTBoss::Yeti; V.BossName = TEXT("THE ABOMINABLE"); V.BossHp = 150.f; V.BossEscape = 80.f; V.BossBullet = 0;
		V.RoadA = Hex(TEXT("3a4a6a")); V.RoadB = Hex(TEXT("364563")); V.Line = Hex(TEXT("ffffff"));
		V.KerbA = Hex(TEXT("ff3355")); V.KerbB = Hex(TEXT("ffffff")); V.Rail = Hex(TEXT("5ff0ff")); V.Neon2 = Hex(TEXT("ff3355"));
		V.Sky0 = Hex(TEXT("3f8fdf")); V.Sky1 = Hex(TEXT("bfe3ff")); V.Sky2 = Hex(TEXT("fff4f0")); V.Fog = Hex(TEXT("dbeeff"));
		V.Glow = FLinearColor(0.37f, 0.94f, 1.f); V.Lamp = FLinearColor(1.f, 0.95f, 0.9f); V.LampCd = 4000.f;
		V.Gantries = { TEXT("FROSTBITE PEAK"), TEXT("EXPERTS ONLY"), TEXT("AVALANCHE ZONE"), TEXT("SUPER PIPE"), TEXT("BLACK DIAMOND") };
		V.Slope = -0.07f;
		V.Backdrop = TEXT("/Game/Maps/Glacier/Props/SM_Peak"); V.BackdropScale = 5.f;
	}
	// ---------------------------------------------------------------- L7 THE DEEP (2.5)
	{
		FFTLevelDef& V = L[6];
		V.Index = 6; V.Zone = TEXT("UNDER THE SEA"); V.Title = TEXT("SINK OR SWIM");
		V.Place = TEXT("SEABED ROUTE 7 TO NEREIDA · 300 FATHOMS"); V.Map = TEXT("/Game/Maps/Abyss/L_Abyss");
		V.Hint = TEXT("The road to the sunken city. Glass tunnels all the way.");
		V.Opening = ParseSections(TEXT(
			"1500|1500:.5:.2:hill:.06:2|1300:-.6:-.25|600|1800:pipe:1:1|500|1300:.9:.4|1300:-.9:-.4|1500:hill:.08:2|600|2400:pipe:-1:2|500|"
			"1200:1.1:.5|1200:-1.1:-.5|900:rise:.2|1700:jump:.32:.40|600|1300:twist:1|1000:-.6:-.25:fall:.15|500|2100:loop|500|1800:pipe:1:1|500|"
			"1400:1.5:.9|1400:-1.5:-.9|1600:hill:.07:2|600|2800:pipe:1:3|500|1200:.8:.35|1200:-.8:-.35|900:rise:.2|1700:jump:.32:.40|600|1300:twist:-1|"
			"1000:1:.45|1000:-1:-.45|600|2400:pipe:-1:2|500|2100:loop|500|1500:.6:.25:hill:.06:1|1500:-.6:-.25|600|1800:pipe:1:1|800|1500"));
		V.Arena = ParseSections(TEXT("5000|4000:.25:.1|4000:-.25:-.1|5000|4000:.3:.12|4000:-.3:-.12|6000|4000:.2:.08|4000:-.2:-.08"));
		V.Circuit = ParseSections(TEXT("1200:1:.45|1200:-1:-.45|600|1800:pipe:1:1|600|1500"));
		V.FinaleD = 57900.f;
		V.Scenes = {
			Scene(TEXT("THE DESCENT"), 0, TEXT("1500:mm 3000:r 4300:s 6700:c"), TEXT("Scene seven. Three hundred fathoms down, and the road keeps going!"), TEXT("The descent. Lights on, foot down!")),
			Scene(TEXT("THE GLASS TUNNEL"), 7200, TEXT("7200:mm 9800:rs 11300:w 14300:sm"), TEXT("Into the glass tunnel! The whole ocean is on the other side!"), TEXT("Round the tunnel. Don't crack the glass!")),
			Scene(TEXT("THE KELP FOREST"), 14800, TEXT("14800:rr 16000:sm 19800:t 22700:c"), TEXT("Kelp on both sides and a corkscrew dead ahead!"), TEXT("The kelp forest. Jump, roll, loop!")),
			Scene(TEXT("THE TRENCH"), 25300, TEXT("25300:mmm 27600:s 28100:rr 30900:wc"), TEXT("The trench! Down into the dark. Guns up!"), TEXT("The trench. Three times round the tube!")),
			Scene(TEXT("THE CORAL REEF"), 35900, TEXT("35900:ss 37600:mr 41400:t 43300:wc"), TEXT("Coral everywhere! Don't scratch my star car!"), TEXT("The reef. Jump it and roll it!")),
			Scene(TEXT("THE CITY GATES"), 45300, TEXT("45300:rrm 48300:sm 50900:mm 52900:wc 54400:rs"), TEXT("There it is! The sunken city of Nereida! Lights, camera, ACTION!"), TEXT("The city gates. Loop it into town!")),
			Scene(TEXT("THE FINALE"), 57900, TEXT("57900:B"), TEXT("THE GREAT WHITE! A shark on wheels! Dodge the fish!"), TEXT("Final shot. Dodge the fish and harpoon the shark!")),
		};
		V.Rating[1] = 75000; V.Rating[2] = 140000; V.Rating[3] = 200000;
		V.Boss = EFTBoss::Shark; V.BossName = TEXT("THE GREAT WHITE"); V.BossHp = 150.f; V.BossEscape = 80.f; V.BossBullet = 1;
		V.RoadA = Hex(TEXT("1e2f50")); V.RoadB = Hex(TEXT("1b2a48")); V.Line = Hex(TEXT("9ff0ff"));
		V.KerbA = Hex(TEXT("2ee6ff")); V.KerbB = Hex(TEXT("1e2f50")); V.Rail = Hex(TEXT("2ee6ff")); V.Neon2 = Hex(TEXT("ff7ad9"));
		V.Sky0 = Hex(TEXT("020a1e")); V.Sky1 = Hex(TEXT("06305a")); V.Sky2 = Hex(TEXT("2a9ac0")); V.Fog = Hex(TEXT("0a3a5a"));
		V.Glow = FLinearColor(0.2f, 0.8f, 1.f); V.Lamp = FLinearColor(0.5f, 0.9f, 1.f); V.LampCd = 6000.f;
		V.Gantries = { TEXT("NEREIDA 20 LEAGUES"), TEXT("SEABED ROUTE 7"), TEXT("THE TRENCH"), TEXT("CORAL REEF"), TEXT("WELCOME TO NEREIDA") };
		V.Backdrop = TEXT("/Game/Maps/Abyss/Props/SM_City"); V.BackdropScale = 4.f;
	}
	// ---------------------------------------------------------------- L8 MIDTOWN (2.5)
	{
		FFTLevelDef& V = L[7];
		V.Index = 7; V.Zone = TEXT("CITY CENTER"); V.Title = TEXT("HOSTILE TAKEOVER");
		V.Place = TEXT("MIDTOWN · FIFTH AND MAIN · 23:59 HRS"); V.Map = TEXT("/Game/Maps/Metro/L_Metro");
		V.Hint = TEXT("Tight corners, straight through the towers. Then the Chairman.");
		V.Opening = ParseSections(TEXT(
			"1400|1000:1.2:.5|1000:-1.2:-.5|600|1600:.3:.1:bldg:52|600|900:2.4:1.0|900:-2.4:-1.0|500|1400:bldg:70|500|1300:twist:1|1000:2.6:1.0|"
			"800|1800:-.3:-.1:bldg:60|800|900:-2.8:-1.0|900:2.8:1.0|500|900:rise:.2|1700:jump:.32:.40|600|1200:bldg:45|600|2100:loop|500|1000:-2.6:-1.0|"
			"1000:2.6:1.0|600|2000:.25:.1:bldg:85|600|900:3:1.0|700|900:-3:-1.0|500|1300:twist:-1|800:2.2:.9|800:-2.2:-.9|600|1600:bldg:58|600|1000:rise:.27|400|2000:jump:.36:.45|"
			"600|1000:2.8:1.0|1000:-2.8:-1.0|600|1800:-.3:-.1:bldg:66|600|2100:loop|600|900:-2.4:-1.0|900:2.4:1.0|1400:bldg:50|800|1500"));
		V.Arena = ParseSections(TEXT("5000|4000:.3:.12|4000:-.3:-.12|3000:bldg:60|5000|4000:.35:.15|4000:-.35:-.15|6000|4000:.25:.1|4000:-.25:-.1"));
		V.Circuit = ParseSections(TEXT("900:2.4:1.0|900:-2.4:-1.0|600|1600:bldg:55|600|1500"));
		V.FinaleD = 55900.f;
		V.Scenes = {
			Scene(TEXT("RUSH HOUR"), 0, TEXT("1400:mm 2400:r 3400:s 5600:c"), TEXT("The last scene. Midtown at midnight. Straight through the towers!"), TEXT("Rush hour. Through the lobby, go!")),
			Scene(TEXT("THE LOBBY"), 8000, TEXT("8000:rr 9900:sm 12700:h"), TEXT("Right through the building! Mind the doorman!"), TEXT("Through the lobby and roll it!")),
			Scene(TEXT("ROOFTOP ALLEY"), 15300, TEXT("15300:mmm 17900:s 21000:wc"), TEXT("Hard left, hard right! This town is all corners!"), TEXT("Corners! Drift them!")),
			Scene(TEXT("THE PLAZA"), 22800, TEXT("22800:rr 25500:t 28000:sm 30600:h"), TEXT("Loop it in the plaza, then straight through the tower!"), TEXT("The plaza. Loop, then the tower!")),
			Scene(TEXT("WALL STREET"), 33700, TEXT("33700:rrr 36300:s 39300:wc 40900:p 43300:mm"), TEXT("The money district! Every corner's a hairpin!"), TEXT("Wall Street. Hairpins and a big jump!")),
			Scene(TEXT("PENTHOUSE ROW"), 45900, TEXT("45900:ss 48300:h 51000:rrm 52500:wc 54800:mm"), TEXT("Last stretch! Through the towers, round the loop!"), TEXT("Penthouse row. Make it count!")),
			Scene(TEXT("THE FINALE"), 55900, TEXT("55900:B"), TEXT("THE CHAIRMAN! His limo's a gunship. Dodge the lead and take him DOWN!"), TEXT("Final shot. Thread the bullets and finish the Chairman!")),
		};
		V.Rating[1] = 90000; V.Rating[2] = 165000; V.Rating[3] = 240000;
		V.Boss = EFTBoss::Limo; V.BossName = TEXT("THE CHAIRMAN"); V.BossHp = 220.f; V.BossEscape = 95.f; V.BossBullet = 3;
		V.RoadA = Hex(TEXT("26243a")); V.RoadB = Hex(TEXT("222035")); V.Line = Hex(TEXT("ffd23f"));
		V.KerbA = Hex(TEXT("ffd23f")); V.KerbB = Hex(TEXT("26243a")); V.Rail = Hex(TEXT("ff4fb8")); V.Neon2 = Hex(TEXT("2ee6ff"));
		V.Sky0 = Hex(TEXT("080614")); V.Sky1 = Hex(TEXT("2a1f4a")); V.Sky2 = Hex(TEXT("ff6a3d")); V.Fog = Hex(TEXT("3a2a4a"));
		V.Glow = FLinearColor(1.f, 0.82f, 0.25f); V.Lamp = FLinearColor(1.f, 0.85f, 0.6f); V.LampCd = 6000.f;
		V.Gantries = { TEXT("FIFTH AND MAIN"), TEXT("MIDTOWN TUNNEL"), TEXT("WALL STREET"), TEXT("CHAIRMAN TOWER"), TEXT("ONE WAY") };
		V.Backdrop = TEXT("/Game/Maps/Metro/Props/SM_Spire"); V.BackdropScale = 4.f;
	}
	return L;
}

const FFTLevelDef& FTData::Level(int32 Index)
{
	static TArray<FFTLevelDef> Levels = BuildLevels();
	return Levels[FMath::Clamp(Index, 0, Levels.Num() - 1)];
}

int32 FTData::NumLevels() { return 8; }

static TMap<FName, TArray<FString>> BuildLines()
{
	TMap<FName, TArray<FString>> M;
	auto A = [&M](const TCHAR* K, std::initializer_list<const TCHAR*> V) { TArray<FString>& T = M.FindOrAdd(K); for (const TCHAR* S : V) T.Add(S); };
	A(TEXT("kill"), { TEXT("Yes! That's the shot!"), TEXT("Boom! Beautiful!"), TEXT("Gorgeous wreck! Keep rolling!"), TEXT("That's going in the trailer!"), TEXT("Print it! Next!") });
	A(TEXT("bigKill"), { TEXT("THAT'S the money shot!"), TEXT("The truck! The studio's going to love this!"), TEXT("Look at that fireball! Gorgeous!") });
	A(TEXT("heliDown"), { TEXT("You shot down a CHOPPER?! Somebody call the studio!"), TEXT("The chopper! Look at it burn!") });
	A(TEXT("near"), { TEXT("Closer! I want paint on paint!"), TEXT("Ooh, inches! Again!"), TEXT("That's the danger I want!"), TEXT("Close shave! Love it!") });
	A(TEXT("rail"), { TEXT("Sparks! YES! More sparks!"), TEXT("Grind that rail! Gorgeous!"), TEXT("Look at those sparks fly!") });
	A(TEXT("collateral"), { TEXT("Not the civilians! Insurance is killing me!"), TEXT("Who hired that extra?! Careful!"), TEXT("Civilians cost money, Kane!") });
	A(TEXT("hit"), { TEXT("You're taking fire! Move!"), TEXT("Get out of his line!"), TEXT("Don't let them scratch my star car!"), TEXT("Shake him off!") });
	A(TEXT("lowHp"), { TEXT("The car's on fire! Find a wrench!"), TEXT("That's our only car! Get repairs!") });
	A(TEXT("pickupMissiles"), { TEXT("Props! More missiles!"), TEXT("Missiles loaded. Use them!") });
	A(TEXT("pickupRepair"), { TEXT("Crew patched you up. Go!"), TEXT("Fresh armor. Back in it!") });
	A(TEXT("rev"), { TEXT("He's lining you up! Get out of his lane!"), TEXT("Headlights! He's about to charge. Move!") });
	A(TEXT("turbo"), { TEXT("Punch it!"), TEXT("TURBO! Yes!"), TEXT("Speed lines! Beautiful!") });
	A(TEXT("empty"), { TEXT("No missiles! Grab an orange crate!"), TEXT("You're dry! Find the missile crates!") });
	A(TEXT("gapLand"), { TEXT("And DOWN! Stick it!"), TEXT("Landed it! Print that!"), TEXT("The road came back! Beautiful!") });
	A(TEXT("boost"), { TEXT("Boost! Go, go, go!"), TEXT("Slide and BOOST! That's driving!"), TEXT("Out of the drift and GONE!") });
	A(TEXT("aheadLoop"), { TEXT("LOOP ahead! Keep your foot down all the way round!"), TEXT("Here comes the loop! Don't lift!") });
	A(TEXT("aheadJump"), { TEXT("The road ENDS up there! Hit the lip fast and FLY!"), TEXT("Gap ahead! Turbo off the lip. Big air!") });
	A(TEXT("aheadTwist"), { TEXT("Corkscrew coming! Hold on to your lunch!"), TEXT("The road rolls over up there. Keep shooting!") });
	A(TEXT("aheadWall"), { TEXT("Banked wall ahead! Ride it up high!"), TEXT("Big banked turn! Get up on the wall!") });
	A(TEXT("aheadPipe"), { TEXT("Pipe ahead! The road goes all the way round inside it!"), TEXT("Into the tube! Keep it floored all the way round!") });
	A(TEXT("loopTop"), { TEXT("UPSIDE DOWN! The camera loves it!"), TEXT("Over the top! Keep rolling!") });
	A(TEXT("twistTop"), { TEXT("Upside down and still driving! Gorgeous!"), TEXT("Look at that sky under you!") });
	A(TEXT("pipeTop"), { TEXT("On the CEILING! Keep driving!"), TEXT("Upside down in the pipe! Gorgeous!") });
	A(TEXT("wallIn"), { TEXT("Up on the bank! Higher! Higher!"), TEXT("Ride the wall! Get up there!") });
	A(TEXT("zapWarn"), { TEXT("Lightning in your lane! MOVE!"), TEXT("He's charging the coil! Change lanes!"), TEXT("That lane's going live! Get out!") });
	A(TEXT("zoneWarn"), { TEXT("He's locked on! Keep moving!"), TEXT("Targets on the road! Don't sit still!"), TEXT("Lightning's coming down! Move it!") });
	A(TEXT("ringWarn"), { TEXT("Shock wave! Find the green gap!"), TEXT("Here comes the wave! Get to the gap!"), TEXT("He's discharging! Gap, gap, GAP!") });
	A(TEXT("shock"), { TEXT("You're getting fried! Out of the light!"), TEXT("Zapped! Read the warnings, Kane!"), TEXT("Sparks on the star car! Dodge it!") });
	A(TEXT("dodge"), { TEXT("Through the gap! Beautiful!"), TEXT("Clean dodge! The crew is screaming!"), TEXT("Missed you by a mile! Gorgeous!") });
	A(TEXT("heliIn"), { TEXT("Chopper! They're dropping men on you!"), TEXT("Here comes the chopper! Swerve, don't let them land!"), TEXT("Bird overhead! Shoot it or shake them!") });
	A(TEXT("latch"), { TEXT("He's on the car! Slam him into the wall!"), TEXT("Passenger! Scrape him off on the barrier!"), TEXT("Hitchhiker! Into the wall with him!") });
	A(TEXT("latch2"), { TEXT("Two of them! Wall, wall, WALL!"), TEXT("Both doors! Loop it or grind them off!") });
	A(TEXT("henchHang"), { TEXT("He's still on! Ram that side into the wall!"), TEXT("Get him off my star car! The wall, Kane!"), TEXT("Go upside down and he's gone!") });
	A(TEXT("dropDodged"), { TEXT("Missed you! He kissed the road!"), TEXT("Ha! He landed on nothing!") });
	A(TEXT("partOff"), { TEXT("There goes a piece of the car!"), TEXT("You're shedding parts! Careful!"), TEXT("The car's falling apart! Find a wrench!") });
	A(TEXT("chainWarn"), { TEXT("Chain! He's swinging a chain! Get away from him!"), TEXT("Biker on your door! Slam him or move!"), TEXT("He's winding up! Ram him off the bike!") });
	A(TEXT("chainDodged"), { TEXT("Missed you! Now hit him back!"), TEXT("Ducked it! Gorgeous!") });
	A(TEXT("chainHit"), { TEXT("He chained the star car! Hit him back!"), TEXT("Ow! Ram that chopper!") });
	A(TEXT("hogJump"), { TEXT("They're JUMPING off the truck! Watch the marks!"), TEXT("Flying bikers! Get out from under them!") });
	A(TEXT("brakeWarn"), { TEXT("Brake lights! He's brake-checking you! Back off!"), TEXT("He's stopping! Don't kiss that ramp!") });
	A(TEXT("retake"), { TEXT("Take {k}. Same scene, more conviction!"), TEXT("Take {k}. Stay on the road this time!"), TEXT("Take {k}. Fresh paint on the car. Don't waste it!"), TEXT("Take {k}. From the top. Eyes up!") });
	A(TEXT("cut"), { TEXT("CUT! CUT! Again, from the top!"), TEXT("CUT! We lost the car. Again!"), TEXT("CUT! Reset the scene. Again!") });
	A(TEXT("takeOut"), { TEXT("Take him out, now!"), TEXT("Shoot! What are you waiting for?"), TEXT("Light him up!"), TEXT("He's right there! Fire!") });
	A(TEXT("keepFiring"), { TEXT("Good! Keep pouring it on!"), TEXT("Hold that fire! Don't let up!"), TEXT("More bullets! More!") });
	A(TEXT("faster"), { TEXT("Faster! This is a chase, not a commute!"), TEXT("Speed! I need speed!"), TEXT("Don't brake! Faster!") });
	A(TEXT("useTurbo"), { TEXT("Faster! Hit the turbo!"), TEXT("Turbo's full! Use it, give me speed!") });
	A(TEXT("sparks"), { TEXT("Faster, give me sparks! Kiss that rail!"), TEXT("Scrape the barrier! I want sparks!") });
	A(TEXT("weave"), { TEXT("Get closer to that traffic! Paint on paint!"), TEXT("Weave! Make it look dangerous!"), TEXT("Thread the cars! Closer!") });
	A(TEXT("drift"), { TEXT("Throw it sideways into the bend! Drift it!"), TEXT("Hold the drift through the turn! Sparks!") });
	A(TEXT("calm"), { TEXT("Good. Keep it rolling."), TEXT("Hold that line. Camera loves you."), TEXT("Stay on it. Don't lose the light."), TEXT("Nice. Nice. Keep going.") });
	A(TEXT("missiles"), { TEXT("Missiles! Crack that armor!"), TEXT("Use the missiles! Now!") });
	A(TEXT("lowHpAmb"), { TEXT("The car's smoking. Careful, that's our only one!"), TEXT("Keep it together. We need that car!") });
	A(TEXT("inverted"), { TEXT("Shoot upside down! Make it look easy!"), TEXT("Hang on up there!") });
	A(TEXT("escape"), { TEXT("He's getting away! Faster! Take him out, NOW!") });
	// 2.5
	A(TEXT("aheadHalf"), { TEXT("Half pipe ahead! Steer up the walls, ride it high!"), TEXT("Here comes the pipe! Carve the walls like a snowboarder!") });
	A(TEXT("aheadCoaster"), { TEXT("The coaster! Just hold the gas. Hop across when the rails meet!"), TEXT("Rails ahead! Steer hard to hop to the other track!") });
	A(TEXT("aheadBldg"), { TEXT("Straight through that building! Through the lobby, GO!"), TEXT("The road goes through the tower! Don't stop!") });
	A(TEXT("aheadLava"), { TEXT("Lava across the road! Hit the lip fast!"), TEXT("Lava flow ahead! Fly over it!") });
	A(TEXT("railHop"), { TEXT("Track to track! Gorgeous!"), TEXT("Hopped the rails! The crew is screaming!") });
	A(TEXT("railsSplit"), { TEXT("The rails split! Hang on!"), TEXT("Here they go! Hold on to your lunch!") });
	A(TEXT("halfPipe"), { TEXT("Higher! Ride that wall!"), TEXT("Up the pipe! The camera loves it!") });
	A(TEXT("bldgIn"), { TEXT("Through the lobby! Somebody get the doorman!"), TEXT("Right through the building! Print it!") });
	A(TEXT("lobWarn"), { TEXT("Incoming! Watch the marks on the road!"), TEXT("Something's coming down! Get out of the ring!"), TEXT("Look at the road, Kane! MOVE!") });
	A(TEXT("flameWarn"), { TEXT("He's aiming the hose! Get out of that lane!"), TEXT("Fire in your lane! MOVE!"), TEXT("Flames! Change lanes!") });
	A(TEXT("bulletWarn"), { TEXT("Here comes the lead! Find the gap!"), TEXT("He's opening up! Weave, Kane, weave!"), TEXT("Bullets! Thread them!") });
	A(TEXT("thunder"), { TEXT("Lightning! The sky's on fire!"), TEXT("Did you see that bolt?! Keep driving!") });
	return M;
}

const TArray<FString>& FTData::Lines(const FName& Key)
{
	static TMap<FName, TArray<FString>> M = BuildLines();
	static TArray<FString> Empty;
	const TArray<FString>* F = M.Find(Key);
	return F ? *F : Empty;
}
