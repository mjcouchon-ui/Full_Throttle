using UnrealBuildTool;

public class FullThrottle : ModuleRules
{
    public FullThrottle(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bUseUnity = false; // FTGameMode.cpp uses its own namespace freely

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
            "ChaosVehicles", "ChaosVehiclesCore", "PhysicsCore", "Chaos",
            "ProceduralMeshComponent", "Niagara", "GeometryCollectionEngine", "FieldSystemEngine",
            "Slate", "SlateCore", "ApplicationCore", "Json", "AudioMixer", "RHI", "RenderCore"
        });
    }
}
