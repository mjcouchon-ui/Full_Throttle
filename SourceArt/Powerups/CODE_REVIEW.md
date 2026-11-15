# Independent code and shader review

2026-11-14, independent reviewer agent `review_pickup_code`, read-only diff against origin/main.

## Findings

- P2: pooled actors can retain vehicle Alert material overrides. Fixed by clearing overrides when Setup assigns either pickup kind. Rewards, hitboxes, spawn rules and logical positions are untouched.
- Niagara reset may consume FMath::Rand and alter subsequent gameplay random choices. Fixed in generation: system determinism enabled, seed 88. This applies to pool priming and collection activation.
- Visual note: emissive strips sit outboard of their supporting surfaces; inspect side views before accepting the shape.

## Other conclusions

Hidden static-mesh components still submit PSO requests. Niagara precaches renderer PSOs at asset level and primes pooled components. System roots preserve effect references; collection instances use AutoRelease. Emissive remains unclamped HDR; shared cel and palette materials are unchanged. Low disables collection bursts. Mesh counts satisfy the requested limits.

Packaged first-use hitch testing and idle GPU measurements remain required. This review does not claim a measured performance pass.

## Follow-up review

The same independent reviewer rechecked the Development helper, light changes, pooling, capture script and full source diff. No remaining runtime blocker was found. Mode changes recreate proxies and reset burst/timing state. Pickup lights default to 2 cd at Z220, are disabled on Low, and reset position before pooled reuse. Unchanged light setters early-out. Emissive strips were moved flush to supporting surfaces; the earlier outboard-strip finding is resolved.

Justin explicitly deferred GPU timings while approving completion of visuals. Fixed-step FPS/frame-delta fields are not valid wall-clock measurements; only actual GPU timers may be compared in a future idle-GPU run. The inspection helper is excluded from that run.

Main advanced to 2.8 during this work. Its themed meshes, Yeti changes, moving-mesh Lumen/distance-field exclusions and boss-contact exclusion must remain intact during integration. The original 2.7 capture set is labeled separately from subsequent integration checks.

## Final capture-tool review

Independent review found two issues in the new uncooked visual capture path. The launcher now resolves the project and output paths before changing its working directory. Each uncooked run records the actual source revision, parents, tracked working changes and game-module SHA-256. The evidence assembler reads that provenance instead of assigning constant commit IDs, rejects missing manifests/revisions and rejects mixed revisions for a combined golden-hour sheet. The inspected toward/away manifests show integration `8cbdacb`, parents `3d02846` / `f99e5de`, identical module hashes and working changes limited to documentation/tools/evidence.

The uncooked path accepts screenshots only; performance mode rejects staged inspection actors. GPU and first-use timings remain explicitly deferred. The final 2.8 Development Game and Editor builds and Windows package passed; the package cook reported zero errors and zero warnings.
