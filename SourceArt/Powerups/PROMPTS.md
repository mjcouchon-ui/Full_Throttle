# Power-up concept generation log

## V01 — 2026-11-14

- Owner approval: Justin authorized existing game art and the supplied brief as the reference set. Modeling still requires concept approval.
- Generator: built-in image_gen imagegen; exact backend model identifier is not exposed by the tool. No external paid provider or API invocation.
- Purpose: one approval sheet with three directions per pickup. Not an engine render, runtime asset, or evidence of performance.
- Inputs, in order:
1. `K:\Full Throttle\Level1_Asset_Sheet\_work\thumbs\Game__Characters__Pickups__SM_Crate.png`
2. `K:\Full Throttle\Level1_Asset_Sheet\_work\thumbs\Game__Characters__Pickups__SM_Wrench.png`
3. `K:\Full Throttle\Level1_Asset_Sheet\_work\thumbs\Game__Characters__Kestrel__SM_Kestrel_Body.png`
4. `K:\Full Throttle\PowerupsReview\baseline-local-20261114\shots\ft_L0_030.png`
5. `K:\Full Throttle\PowerupsReview\baseline-local-20261114\shots\ft_L1_052.png`

## Exact generation prompt

```text
Use case: stylized-concept.
Asset type: art-director approval sheet, one coherent landscape board with six original collectible designs, exactly 3 columns by 2 rows, for the game FULL THROTTLE.
Primary request: dramatically redesign the existing missile and repair pickups, grounded in the supplied game's actual art, for high-speed arcade driving. Produce an attractive, clean, production-useful concept sheet. This is 2D concept art, not an actual engine render or a claim of tested performance.

Input references: Image 1 is the existing orange missile crate: preserve its orange/yellow reward identity, replace its plain cube shape. Image 2 is the existing green repair cube: preserve green repair identity and a clearly open-jawed wrench symbol, replace the cube silhouette. Image 3 is the existing KESTREL body: reference chunky angular construction, navy separations, saturated accents, and broad planar highlights. Image 4 is actual Bayshore gameplay: reference the pastel world, ink contour weight, violet road, neon accent restraint, and graphic cel shading. Image 5 is actual night gameplay: reference the dark navy/violet world; improve the pickup's washed-out white face by keeping glow small and localized. The source images are visual references for original designs, not backgrounds to copy into every cell.

Composition: 3:2 landscape sheet, plain warm very pale grey-lavender background, generous margins, subtle dividers. Top title exactly "FULL THROTTLE / POWER-UPS". Subtitle exactly "CONCEPT DIRECTIONS - FOR APPROVAL". Three labeled column headers: "A / FIELD CASES", "B / EXPOSED HARDWARE", "C / STUNT GADGETS". Top row contains ONLY orange missile supply designs labeled "M1", "M2", "M3". Bottom row contains ONLY green repair designs labeled "R1", "R2", "R3". Six large evenly spaced three-quarter views with consistent viewpoint, camera height, lighting and visual scale. Each cell includes a small solid dark silhouette of exactly the same design directly beneath the colored main view, to compare shape without color. No additional paragraphs, UI, arrows, dimensions, watermarks, or brand marks. Ensure all six objects are fully visible and never overlap cell boundaries.

Designs:
M1: orange armored ammunition field case, low and wide hexagonal/chamfered body, chunky dark navy protective corners and two big latches, a shallow top recess revealing TWO large ivory-grey missile noses with warm orange caps inside protective rails, small amber raised rocket emblem on broad dark front inset. Deliberate substantial panel depth. It should look like a tangible 1980s stunt-film prop, with a legible stepped silhouette.
R1: green mechanic's toolbox, wider at the base with pitched shoulder surfaces and a large integral open arched carrying handle; one oversized ivory open-jawed wrench mounted diagonally across a dark inset front, chunky corner pads and a pair of strong latches. Icon must stay recognizable as a wrench. Broad green paint stays visibly green even near the luminous icon.
M2: open orange ammunition carrier holding THREE short chunky faceted missiles upright, clearly exposed pointed noses and broad triangular fins, secured by two bold orange clamps and dark navy structural rails. Distinct three-pronged upper silhouette with real negative space. Compact and balanced, no launch flame.
R2: upright oversized ivory open-jawed wrench seated in a squat green service-jack cradle, a single dark navy brace and broad green stabilizing feet. The wrench jaw and long solid shank define a tall unmistakable tool silhouette; one thick green grip band and a small mint light at the cradle. It is one unified collectible, not loose components. No medical cross.
M3: compact orange wedge-shaped spy equipment cassette with ONE chunky diagonal rocket physically nested through an open top channel, two large guard fins framing its sides, bold chamfered nose, contrasting dark navy base, a small amber light strip. Distinct forward-swept diagonal silhouette, integrated hardware, no fragile aerials.
R3: green folding tool caddy: squat broad two-piece clamshell base, opened only enough to present a large ivory spanner like an angled carry bar across its top, angular green end cheeks and a chunky dark navy hinge. Distinct low arch with conspicuous open negative space under the spanner, different from the taller R1 handle and R2 upright wrench. No wheels.

Style: the game's semi-cel shaded, low-poly 1980s spy-car action aesthetic. Bold simple volumes, chamfered edges, sparse substantial details, per-face planar shading with 2-3 tonal steps, slight gentle variation within planes, thin dark purple/navy silhouette ink. Keep interior linework sparse. Dry painted enamel, dark rubberized guards, broad restrained metal highlight. Three or four dominant material color families per object. Orange base approximately #ff8a1e; green base approximately #2fbf71; outline/structural dark #1b1033; ivory accents. Neon is a small concentrated icon or seam accent, does not cover whole panels. Colored bodies retain saturation and substantial dark mass. Design forms plausibly reproducible using basic mesh primitives and extruded profiles with a roughly 5000-triangle ceiling. Hero readability comes from shape and proportion, not tiny details. Lighting warm upper-left with simple cool shadows consistent across the whole board.

Avoid: photorealism, grime textures, dense greebles, military realism, modern tactical equipment, fantasy gems, treasure chests, circular energy portals, magical sparkles, glitter, particle showers, holographic HUD rings, translucent glowing cubes, generic mobile-game prizes, endless strips of LEDs, complex tubing, legible real logos or branding, copied IP, mesh wireframes, excessive bloom, white washed-out glowing panels. Do not add particles, floating icons or beacons on this sheet: the physical form must carry the design.
```

## Output

Saved output: `concepts/powerups-directions-v01.png`. The built-in image tool returned this sheet on 2026-11-14. Exact file hashes, original output location, and copied reference sources are recorded in `provenance-v01.json`.

Visual inspection: all six requested designs are present with correct M1/M2/M3 and R1/R2/R3 labels. Missile concepts retain orange paint; repair concepts retain green paint and recognizable wrench forms. The board uses sparse fittings, chunky planar forms, navy separation, and focused glow. No external brand, particle field, or white glowing box is present. The solid silhouettes favor M2 and R2 for distinctness. This is a qualitative illustration review, not a measured grayscale or gameplay test. M3 reads as a launcher and R3 as a repair stand; these associations should be considered during selection. The sheet's smooth illustration highlights must be translated to the project's existing cel treatment during implementation.


Initial invocation was rejected before generation because the tool accepts at most five reference images. The unused missile thumbnail was removed; the exact submitted prompt and reference list above reflect the corrected invocation.
