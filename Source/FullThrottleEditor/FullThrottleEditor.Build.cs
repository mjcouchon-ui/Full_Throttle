using UnrealBuildTool;

// Editor-only tools for building FULL THROTTLE's assets through the Unreal MCP connector:
// Niagara systems from the engine's lightweight templates (set by reflection), and Chaos
// geometry collections fractured from static meshes.
public class FullThrottleEditor : ModuleRules
{
    public FullThrottleEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });
        PrivateDependencyModuleNames.AddRange(new string[] {
            "UnrealEd", "AssetTools", "AssetRegistry", "Niagara", "NiagaraCore",
            "GeometryCollectionEngine", "Chaos", "FractureEngine", "PlanarCut", "DataflowCore", "FullThrottle"
        });
    }
}
