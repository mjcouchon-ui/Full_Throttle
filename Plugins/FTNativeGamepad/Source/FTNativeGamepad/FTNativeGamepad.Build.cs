using UnrealBuildTool;

public class FTNativeGamepad : ModuleRules
{
    public FTNativeGamepad(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PrivateDependencyModuleNames.AddRange(new[] { "Core", "CoreUObject", "Engine", "ApplicationCore", "InputCore", "InputDevice", "GameInputBase" });
    }
}
