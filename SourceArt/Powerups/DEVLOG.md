# Power-ups development log

## 2026-11-14 — Concept review v01

- Justin approved using existing game art and the supplied power-up brief as the reference set in chat.
- Prepared three directions for each of the two existing pickups: field cases, exposed hardware, and stunt gadgets. Written target and tradeoffs are in `ART_DIRECTION.md`.
- Generated one 1536 x 1024 concept sheet with the built-in image tool, using five actual project images as references. The tool does not expose its backend model identifier. Exact prompt, rejected preflight attempt, output, and sources are recorded in `PROMPTS.md` and `provenance-v01.json`.
- Copied the selected sheet and source references into this feature branch for durable review. `REVIEW.html` shows the full concept sheet and the exact reference images side by side below it.
- Recommended M2 + R2 for their distinct pointed-missile and open-wrench silhouettes. Owner selection is pending; no meshes, materials, gameplay code, shared settings, or runtime effects were changed.
- Branch `powerups` was created from fetched main at `16d493f`. The existing `level1-sky` checkout and uncommitted Bayshore work remain separate. Nothing has been pushed.
- Exploratory captures from the pre-existing Development build: 107 frames across Bayshore, Chemical Works, and Nevada. A fresh branch build and measured baseline remain necessary before implementation comparisons. No GPU budget claim is made.

## 2026-11-14 — Approved implementation

- Justin selected R1 + M2 in chat. The concept review and direction document now record that selection.
- Built procedural chamfered geometry, preserving SM_Crate and SM_Wrench paths and updating assets in place: 1,484 / 1,376 triangles, four material sections each, Nanite explicitly disabled. Dedicated pickup material instances leave shared palette colors and cel materials unchanged.
- Added visual-only shallow bob/yaw and a brief eight-chip collection burst, disabled on Low. Effects use deterministic Niagara seeds to avoid consuming gameplay RNG.
- First mesh build completed successfully; repeat export compiled existing materials, then exposed a commandlet asset-registry lookup issue with the Niagara template. Changed duplication to use the loaded template object. Subsequent validation will record the result.
- Clean latest-main Development package succeeded. Full-resolution baseline captures are running across all eight levels at Epic and Low with fixed simulation steps. Screenshot runs are not performance measurements.
- Corrected repeat generation passed with zero errors. Unreal's template object is loaded directly before duplication. Fixed seed 88 is explicitly stored on both Niagara systems.
- Independent review found stale vehicle material overrides on pooled pickup proxies; Setup now clears them for pickups. Emissive strips were moved flush onto supporting faces.
- Development Editor C++ build passed. The non-exported NiagaraWorldManager pool helper was replaced by the engine's existing PostLoad/first-world-tick pool priming. Warm components still request PSO precaching.
- A commandlet SceneCapture studio attempt produced empty images and was discarded as evidence. Use the real packaged viewport inspection mode for form/turntable/cel validation; the unsuccessful script was removed.

## 2026-11-14 — Visual validation and integration

- Justin requested: “Finish visuals; leave timings pending.” No idle-GPU or cold first-use timing approval is claimed.
- Actual viewport captures exposed the original 250 cd light bleaching raised details. Reduced it to 2 cd, raised it to Z220, and disabled it on Low. Independent close day/night and Low review passed after that correction.
- Close grayscale inspection separates the missile columns from the toolbox/handle/wrench. At native 100 m, the established orange/green identities remain distinguishable in the reviewed conditions; geometry alone does not reliably communicate function. Do not use enlarged crops as native-size recognition evidence.
- Collection chips remain brief and leave the forward view clear in sampled day/night captures. Low suppresses the effect.
- Completed baseline and feature 2.7 natural gameplay captures: all eight levels at Epic and Low, 114 native 3440 x 1440 frames per run, 3,648 PNGs total. Every non-benchmark LogFT event matches baseline in all 16 paired runs, including recorded collections. Raw captures remain outside Git; the review contains labeled, hashed derivatives.
- Main advanced to 2.8 while work was underway. Merged 89eb95d into powerups, preserving its themed enemies, Yeti and lighting/performance changes. Development game and Editor compilation passed. The final package and 2.8 inspection matrix are being completed separately from the 2.7 comparisons.
- The inspection camera can face toward/away the actual rendered directional light. Hidden/disabled lights are excluded after independent code review. Its staged approach begins at exactly 100 m, advances at 54 m/s, and demonstrates the visual burst without entering the gameplay entity list.
- The connected GitHub plugin could not access this repository. The local GitHub CLI can access it and will be used for the authorized feature PR.

## 2026-11-14 — Final evidence

- Final 2.8 Windows Development package passed; cook reported zero errors and zero warnings. All 16 staged level/quality captures exited successfully with 25 frames each (400 native PNGs). Independent art review covered all 16 native distance frames and all 16 contact sheets. No new close-form/rendering blocker was found. Geometry-only function recognition at 100 m remains an unmet distance-readability condition.
- Added portable lossless 3440 x 1440 WebP distance frames. A decoded RGBA hash comparison of Bayshore Epic PNG and WebP matched exactly (`f0b8d5772c9bdff77720295151a0f813`), verifying no pixel changes for that sample.
- Created temporary local integration `8cbdacb` from feature `3d02846` plus Claude's committed Level 1 sky/billboard branch `f99e5de`, and built its Editor module successfully for uncooked visual compatibility checks. Claude's separate in-progress `level1-buildings` checkout was inspected read-only and is not included. The temporary integration is not the powerups PR branch.
- Independent capture-tool review fixed relative project/output resolution and unverified commit labels. Uncooked manifests now record actual Git revision/parents, tracked working changes and module hash; the assembler rejects missing or mixed provenance. Early uncooked screenshots precede Level Instance streaming, so golden-hour evidence uses frame 017 after the sky and rendered sun are present.
- All six golden-hour compatibility runs exited 0 with 21 frames each: toward/away sun plus exact 100 m at Epic and Low. Independent review found no further pickup art blocker beyond distance recognition; Low toward-sun trim/road highlight differences are documented. Direct sun-disc overlap was not demonstrated.
- Returned to the powerups feature branch and restored its six backed-up Editor binary/target files with matching hashes. Only the feature branch will be published. A merge-tree check against current main 2.9 (`f039571`) succeeds; the actual build/package evidence remains labeled 2.8.
