import unreal
print("BSKY thumb classes:", [n for n in dir(unreal) if "humbnail" in n])
for n in [n for n in dir(unreal) if "humbnail" in n]:
    c = getattr(unreal, n)
    print("BSKY", n, [m for m in dir(c) if not m.startswith("_") and ("thumb" in m.lower() or "render" in m.lower() or "export" in m.lower() or "image" in m.lower())][:20])
print("BSKY StaticMesh tri fns:", [m for m in dir(unreal.StaticMesh) if "tri" in m.lower() or "num" in m.lower()][:20])
print("BSKY Texture2D size fns:", [m for m in dir(unreal.Texture2D) if "size" in m.lower()][:20])
print("BSKY EAL thumb:", [m for m in dir(unreal.EditorAssetLibrary) if "thumb" in m.lower()])
print("BSKY AssetTools thumb:", [m for m in dir(unreal.AssetTools) if "thumb" in m.lower()])
