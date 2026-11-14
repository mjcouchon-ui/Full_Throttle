using UnrealBuildTool;

public class FullThrottleEditorTarget : TargetRules
{
    public FullThrottleEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.AddRange(new string[] { "FullThrottle", "FullThrottleEditor" });
    }
}
