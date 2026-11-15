"""Probe (no changes): UE 5.9 APIs for the HDR / volumetric-lighting pass. Material expression classes for the sky
atmosphere, light-shaft and volumetric properties on the directional light, height-fog properties, texture compression
enums, HDR console variables."""
import re
import unreal


def log(*a):
    print("BSKY", *[str(x) for x in a], flush=True)


for n in ("MaterialExpressionSkyAtmosphereViewLuminance", "MaterialExpressionSkyAtmosphereLightIlluminance",
          "MaterialExpressionSkyAtmosphereLightDiskLuminance", "MaterialExpressionSkyAtmosphereAerialPerspective",
          "MaterialExpressionSkyAtmosphereDistantLightScatteredLuminance", "MaterialExpressionSkyAtmosphereLightDirection",
          "MaterialExpressionSkyLightEnvMapSample", "SkyAtmosphereComponent", "VolumetricCloudComponent"):
    log("class", n, hasattr(unreal, n))


def props(cls, pat):
    doc = cls.__doc__ or ""
    names = re.findall(r"- ``(\w+)`` \(([^)]*)\)", doc)
    return [(a, b) for a, b in names if re.search(pat, a, re.I)]


log("dirlight", props(unreal.DirectionalLightComponent, "shaft|bloom|occlusion|volumetric|atmosphere|cast|scatter|source_angle|sun"))
log("lightcomp", props(unreal.LightComponent, "volumetric|scatter|indirect|light_function|lighting_channels"))
log("fog", props(unreal.ExponentialHeightFogComponent, "volumetric|inscatter|directional|cubemap|texture|density|falloff|start|opacity|cutoff|sky|second"))
log("skylight", props(unreal.SkyLightComponent, "real_time|cubemap|capture|source|lower|intensity|volumetric"))
log("skyatmo", props(unreal.SkyAtmosphereComponent, ".")[:60] if hasattr(unreal, "SkyAtmosphereComponent") else None)
log("tc", [e for e in dir(unreal.TextureCompressionSettings) if e.startswith("TC_")])
for cv in ("r.AllowHDR", "r.HDR.EnableHDROutput", "r.HDR.Display.OutputDevice", "r.HDR.Display.ColorGamut", "r.LightShaftQuality",
           "r.VolumetricFog", "r.VolumetricFog.GridPixelSize", "r.VolumetricFog.GridSizeZ", "r.Tonemapper.GrainQuantization",
           "r.SkyAtmosphere", "r.SceneColorFormat"):
    try:
        log("cvar", cv, unreal.SystemLibrary.get_console_variable_int_value(cv), unreal.SystemLibrary.get_console_variable_float_value(cv))
    except Exception as e:  # noqa
        log("cvar", cv, "ERR", e)
