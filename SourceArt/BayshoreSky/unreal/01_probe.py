import unreal
L = lambda *a: print("BSKY", *a)
for n in ["CollectionScalarParameter","CollectionVectorParameter","CustomInput","CustomOutput","CustomMaterialOutputType","MaterialExpressionPerInstanceCustomData","MaterialExpressionVertexInterpolator","MaterialExpressionCustom","DataAssetFactory","BlueprintGraphEditor","TextureCompressionSettings","TextureGroup"]:
    L("has", n, hasattr(unreal, n))
L("MP", [m for m in dir(unreal.MaterialProperty) if m.startswith("MP_")])
L("TC", [m for m in dir(unreal.TextureCompressionSettings) if m.startswith("TC_")])
L("TG", [m for m in dir(unreal.TextureGroup) if "SKY" in m or "WORLD" in m or "EFFECTS" in m])
L("FIT", [m for m in dir(unreal.FunctionInputType) if m.startswith("FUNCTION")])
L("CMOT", [m for m in dir(unreal.CustomMaterialOutputType) if m.startswith("CMOT")])
L("Usage", [m for m in dir(unreal.MaterialUsage) if "INSTANCED" in m or "STATIC" in m or "NIAGARA" in m])
# compile test
at = unreal.AssetToolsHelpers.get_asset_tools()
MEL = unreal.MaterialEditingLibrary
p = "/BayshoreSky/Materials/M_ProbeTemp"
if unreal.EditorAssetLibrary.does_asset_exist(p): unreal.EditorAssetLibrary.delete_asset(p)
m = at.create_asset("M_ProbeTemp", "/BayshoreSky/Materials", unreal.Material, unreal.MaterialFactoryNew())
m.set_editor_property("ShadingModel", unreal.MaterialShadingModel.MSM_UNLIT)
c = MEL.create_material_expression(m, unreal.MaterialExpressionCustom, -300, 0)
ci = unreal.CustomInput(); ci.set_editor_property("InputName", "A")
c.set_editor_property("Inputs", [ci])
c.set_editor_property("Code", "return float3(A, View.GameTime * 0, 1);")
k = MEL.create_material_expression(m, unreal.MaterialExpressionConstant, -500, 0)
L("connect", MEL.connect_material_expressions(k, "", c, "A"), MEL.connect_material_property(c, "", unreal.MaterialProperty.MP_EMISSIVE_COLOR))
try:
    errs = MEL.recompile_material(m); L("errors", list(errs))
    s = MEL.get_statistics(m); L("stats PS", s.get_editor_property("NumPixelShaderInstructions"))
except Exception as e:
    L("compile exception", e)
unreal.EditorAssetLibrary.delete_loaded_asset(m)
