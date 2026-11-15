import unreal
print("BSKY engine:", unreal.SystemLibrary.get_engine_version())
print("BSKY project:", unreal.Paths.get_project_file_path())
ar = unreal.AssetRegistryHelpers.get_asset_registry()
print("BSKY plugin mounted:", unreal.EditorAssetLibrary.does_directory_exist("/BayshoreSky"))
for n in ["BlueprintEditorLibrary", "MaterialEditingLibrary", "SubobjectDataSubsystem", "EditorLoadingAndSavingUtils", "InterchangeManager"]:
    print("BSKY has", n, hasattr(unreal, n))
print("BSKY BlueprintEditorLibrary:", [m for m in dir(unreal.BlueprintEditorLibrary) if not m.startswith("_")])
print("BSKY graph-ish classes:", [m for m in dir(unreal) if "BlueprintGraph" in m or "K2Node" in m or "EdGraph" in m][:80])
