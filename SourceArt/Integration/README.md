# Main integration — 15 November 2026

Combines the existing 3.3 gameplay and career-night handoff, the Canvas redesign
and menu-motion update, approved pickups and distance markers, sky/ground fixes,
all eight dressing kits, living-world animation, eruption assets and attack FX.
Dirty source worktrees were preserved as Git snapshot commits before integration.

The newer 3.3 helicopter rig, separated wheels, suspension, Tesla boss and
three-rail coaster remain intact. `refresh_vehicle_art.py` regenerated 55 bodies
and characters with the visual-finish pass on that geometry; the wheel placement
JSON is unchanged. Pickup markers use the current HUD typography. Ground skirts
support all three coaster rails. Attack effects retain the current hit and rail
logic, and share the existing glow-change cache without suppressing wind-up FX.

Validation: the Development Editor target builds successfully. `smoke.txt` loads
all eight levels, drives briefly in each, captures the title/HUD and pause screen,
and exercises pause/resume. It is a startup and integration check, not a complete
playthrough, packaged release test or performance certification.

Known limitation: Unreal reported a non-Nanite virtual-shadow marking queue
overflow in dense Bayshore scenery. It did not stop the run. GPU timing and
performance-budget certification remain pending at Justin's request.
