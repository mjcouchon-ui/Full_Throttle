# Bayshore billboards: Set 02

Four new fictional advertisements for Full Throttle, Level 1: Bayshore.

| Business | Headline | Runtime texture |
| --- | --- | --- |
| Billable Bill | HURT AT WORK? CONGRATULATIONS. | T_BB_BillableBill_BC.png |
| Last Dime Casino | LOSE THE HOUSE. KEEP THE MUG. | T_BB_LastDime_BC.png |
| Captain Liability's | YESTERDAY'S CATCH. TOMORROW'S LAWSUIT. | T_BB_CaptainLiability_BC.png |
| Alibi Inn | WE CHARGE BY THE LIE. | T_BB_AlibiInn_BC.png |

## Resolution and provenance

The original nine-concept sheet was 1774 x 887 for the entire sheet, approximately 590 x 295 per sign. These four designs were generated individually at 1774 x 887 each with the built-in image generator. Each has roughly nine times the source pixels of one panel in the original sheet.

The production PNGs in `textures_2k/` are 2048 x 1024, RGB, with an embedded sRGB profile and no alpha. They are modest Lanczos resizes of the generated originals to power-of-two dimensions, not native 2K generation and not AI-enhanced detail. No 4K files are needed for the intended drive-by use. Original generated PNGs and the exact prompts are retained.

Generated illustrations and exact visible headlines were reviewed. PNG dimensions, RGB mode, profile, hashes, and archive integrity were checked. No runtime performance, cooked texture format, in-level readability, or mesh import validation is claimed: the editor bridge was unavailable.

## Unreal import

**Use `import ft_billboards; ft_billboards.build()` (`Content/Python/ft_billboards.py`) instead.** It builds both sets into `/Game/Maps/Bayshore/Billboards/` (textures, `M_BB_Printed`, the instances and the `SM_BB_*` sign meshes) and is what Level 1's track places. The manual steps and the optional importer below write to a different folder (`.../Billboards/Set02/...`), so following them would create duplicate assets; they are kept only as the pack's original hand-off.

Import the four PNGs from `textures_2k/` into a new folder such as `/Game/Maps/Bayshore/Billboards/Set02/Textures`. Import only these texture PNGs, not the preview or source originals.

For the Windows target, start with:

- Compression Settings: BC7 (verify the cooked format for the actual target).
- sRGB: enabled.
- Texture Group: World.
- Mip Gen Settings: FromTextureGroup; preserve the full mip chain.
- Never Stream: disabled, so normal texture streaming can work.
- Maximum Texture Size: 2048.
- LOD Bias: 0 initially.
- X/Y tiling: Clamp.

The optional `unreal/import_billboards.py` imports only the four textures and configures those properties. It creates one shared Default Lit material and four material instances in the new Set02 folder. It does not touch the map or overwrite existing assets. Run it through Unreal's Execute Python Script command after enabling Python Editor Script Plugin. Its syntax was checked locally; it has not been executed in Unreal.

Use the shared material's texture parameter as Base Color, roughness about 0.75, metallic 0, opaque Default Lit. Leave emissive off for normal printed billboards. Avoid UI texture settings, disabled mipmaps, or forcing all signs to remain fully resident. If mip generation softens the lettering too much, test mild mip sharpening at the actual camera distance before changing the whole texture group.

## Fitting the existing sign

The inspected project is `K:/Full Throttle/UE/full-throttle-unreal`. Its `Content/Python/ft_city.py` builds `SM_Billboard` as colored geometry: the face is 1200 x 600 cm, its bottom is Z=560 cm, and its front faces local -X. Applying a texture to the whole existing mesh is not a verified full-face UV mapping.

`meshes/SM_BB_FaceOverlay.obj` is an optional four-vertex, two-triangle face with full 0-1 UVs and a -X normal. It matches those dimensions and the existing sign's pivot, at local X=-20 cm to clear the decorative face geometry. Import with scale 1 and verify the dimensions, orientation, and UV orientation in Unreal before using it. It is not placed in the level by this pack.

The existing signs are spawned by a procedural prop rule in `FTTrack.cpp`. To use overlays on those instances, the face must receive the same instance transforms, or the textured face must be incorporated into a new billboard mesh variant. Placement and variation are a separate integration step. Do not apply one large image across the entire sign's structural material slots.

## Runtime budget and visual check

A 2048 x 1024 BC7 texture occupies about 2.67 MiB with a complete mip chain when fully resident. Four cost roughly 10.67 MiB; the comparable four 4K textures would cost roughly 42.67 MiB. These are block-compression estimates, not a measured GPU profile or the PNG sizes on disk. At distance, streaming can reduce residency further.

Check the headlines from the gameplay chase camera at normal speed and target display resolution. If the nearest visible sign is small on screen, cap it at 1024 x 512 or test LOD Bias 1; that drops its complete BC7 mip chain to about 0.67 MiB. Look for text shimmer, mip transitions, and whether bloom or the cel pass obscures lettering. Keep 2K for the closer signs only if the camera benefits.

## Contents

- `textures_2k/`: four production PNGs, 2048 x 1024 each.
- `originals/`: unmodified generated sources, 1774 x 887 each.
- `prompts/`: exact generation prompts.
- `meshes/`: optional face-overlay OBJ.
- `unreal/`: optional texture/material import script.
- `preview.jpg`: review contact sheet, not a production texture.
- `manifest.json`: provenance, settings, dimensions, SHA-256 hashes, and validation scope.
