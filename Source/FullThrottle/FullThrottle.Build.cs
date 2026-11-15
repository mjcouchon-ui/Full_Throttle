using UnrealBuildTool;

public class FullThrottle : ModuleRules
{
    public FullThrottle(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bUseUnity = false; // FTGameMode.cpp uses its own namespace freely
        // the nights (the beat-em-up, merged from beat-em-up-unreal): its files include each other by bare name
        PublicIncludePaths.Add(ModuleDirectory + "/Brawl");
        PublicIncludePaths.Add(ModuleDirectory);   // so Brawl/ finds FTNightFlow.h (the hand-off to the races)

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
            "ChaosVehicles", "ChaosVehiclesCore", "PhysicsCore", "Chaos",
            "ProceduralMeshComponent", "Niagara", "GeometryCollectionEngine", "FieldSystemEngine",
            "Slate", "SlateCore", "ApplicationCore", "Json", "AudioMixer", "RHI", "RenderCore"
        });
    }
}
