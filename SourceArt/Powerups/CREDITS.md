# Power-up concept sources

- Art direction and brief: Justin, project technical and environment artist.
- Existing pickup and KESTREL models: Full Throttle project art, identified by the repository asset inventory as Matt's port. Used with Justin's authorization as visual references; this document does not assert a new license for existing project content.
- Bayshore world/sky and Chemical Works gameplay: Full Throttle project captures made locally during this task, 2026-11-14. Existing Bayshore sky work is preserved in its original checkout.
- Concept illustration `concepts/powerups-directions-v01.png`: generated for this project using the built-in image_gen imagegen tool. Exact backend model identifier is not exposed. Prompt and source provenance are retained alongside the image. Concept only; not imported into runtime Content.
- No external stock art, real brands/logos, copied third-party prop designs, external font files, or third-party asset packages were introduced. Text in the concept sheet is generated raster lettering. The review page uses the viewer's system fonts without bundling font files.

Reference copies and hashes are listed in `provenance-v01.json`. No external paid provider/API invocation was used.

Runtime assets: original procedural forms and wrench profile authored for this feature in ft_powerups.py. The build uses the project's existing Geometry Script helpers and Epic's bundled Niagara DirectionalBurstLightweight template; the template is adapted through the project's existing FTEditorTools reflection helper. No new external asset license is asserted or introduced. Procedural source was authored with Codex; the exact coding backend model identifier is not exposed by this session.

Golden-hour compatibility images use Claude's existing project BayshoreSky and billboard content at `f99e5de`, temporarily integrated with the power-up feature for capture. Those assets and Claude's newer in-progress building changes are not included in this power-up contribution. Presentation derivatives use ffmpeg for labels, grids, grayscale and compression; raw viewport PNGs are retained unchanged outside Git. Native distance WebPs use lossless encoding. Capture provenance and derivative source hashes are in `evidence/manifest.json`.
