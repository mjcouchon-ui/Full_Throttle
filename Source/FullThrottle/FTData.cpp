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
	L.SetNum(3);

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
	}
	return L;
}

const FFTLevelDef& FTData::Level(int32 Index)
{
	static TArray<FFTLevelDef> Levels = BuildLevels();
	return Levels[FMath::Clamp(Index, 0, Levels.Num() - 1)];
}

int32 FTData::NumLevels() { return 3; }

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
	return M;
}

const TArray<FString>& FTData::Lines(const FName& Key)
{
	static TMap<FName, TArray<FString>> M = BuildLines();
	static TArray<FString> Empty;
	const TArray<FString>* F = M.Find(Key);
	return F ? *F : Empty;
}
