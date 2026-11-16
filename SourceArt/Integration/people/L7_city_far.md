# L7 City Center, far life and creatures (extra worker, focus "far")

Generator: Content/Python/ft_people_city_far.py. Table: the "far" block of Source/FullThrottle/FTPeopleL7.inl.

Six meshes in /Game/People/CityCenter/, all built and all placed by the table:
SM_FarTowerParty, SM_FarTowerLoft, SM_FarConcert, SM_FarSkyRun_A, SM_FarGeese, SM_FarGliders.

The one closing check (15 Nov, level started once, no frames looked at):
- compiles; the level comes up;
- FTTRACK level=7 frames=12717 route=5faaeb1d solid=25fa2576 solidTriangles=957884, identical to the baseline;
- FTDRESS level=7 pieces placed=7421 of 7448 tried (dressing and people together);
- all six meshes are loaded by the level. The placement list only names meshes with something moved or left out:
  SM_FarTowerLoft -8/~5/50, SM_FarSkyRun_A -1/~0/20; the other four had nothing moved and nothing left out, so the
  list does not print their counts.

Not done: no review of the last table (gliders kept out of loops, corkscrews, jumps) in frames; no contact sheet.
