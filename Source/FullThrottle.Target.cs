using UnrealBuildTool;

public class FullThrottleTarget : TargetRules
{
    public FullThrottleTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("FullThrottle");
    }
}
