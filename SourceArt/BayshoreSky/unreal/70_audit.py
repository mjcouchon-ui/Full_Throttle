"""Audit /BayshoreSky: every package, its size on disk, its dependencies outside the plugin, and Data Validation.
Writes DevLog/audit/plugin_assets.csv, engine_dependencies.txt and validation.txt. Run: ue_run.ps1 -Editor -Script 70_audit.py
"""
import sys
import os
import csv
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)) if "__file__" in globals() else r"K:\Full Throttle\SkyScratch\SourceArt\BayshoreSky\unreal")
import unreal
from bsky_ue import log

OUT = r"K:\Full Throttle\SkyScratch\DevLog\audit"
os.makedirs(OUT, exist_ok=True)
CONTENT = r"K:\Full Throttle\SkyScratch\Plugins\BayshoreSky\Content"
ar = unreal.AssetRegistryHelpers.get_asset_registry()
ar.scan_paths_synchronous(["/BayshoreSky"], True)
assets = ar.get_assets_by_path("/BayshoreSky", recursive=True)
opts = unreal.AssetRegistryDependencyOptions(include_soft_package_references=True, include_hard_package_references=True,
                                              include_searchable_names=False, include_soft_management_references=False,
                                              include_hard_management_references=False)
rows, outside = [], {}
for a in assets:
    pkg = str(a.package_name)
    rel = pkg[len("/BayshoreSky/"):]
    f = None
    for ext in (".uasset", ".umap"):
        p = os.path.join(CONTENT, rel.replace("/", os.sep) + ext)
        if os.path.exists(p):
            f = p
    size = os.path.getsize(f) if f else 0
    deps = [str(d) for d in (ar.get_dependencies(pkg, opts) or [])]
    ext_deps = [d for d in deps if not d.startswith("/BayshoreSky")]
    for d in ext_deps:
        outside.setdefault(d, set()).add(pkg)
    rows.append((pkg, str(a.asset_class_path.asset_name), size, ";".join(sorted(ext_deps))))
with open(os.path.join(OUT, "plugin_assets.csv"), "w", newline="", encoding="utf-8") as fh:
    w = csv.writer(fh)
    w.writerow(["package", "class", "bytes_on_disk", "references_outside_plugin"])
    for r in sorted(rows):
        w.writerow(r)
with open(os.path.join(OUT, "engine_dependencies.txt"), "w", encoding="utf-8") as fh:
    for d in sorted(outside):
        fh.write("%s  <-  %s\n" % (d, ", ".join(sorted(outside[d]))))
bad = [d for d in outside if not (d.startswith("/Engine") or d.startswith("/Script"))]
log("audit: %d packages, %d outside references, non-Engine: %s" % (len(rows), len(outside), bad))

# Data Validation
lines = []
try:
    sub = unreal.get_editor_subsystem(unreal.EditorValidatorSubsystem)
    settings = unreal.ValidateAssetsSettings()
    settings.set_editor_property("skip_excluded_directories", True)
    settings.set_editor_property("show_if_no_failures", True)
    settings.set_editor_property("validation_usecase", unreal.DataValidationUsecase.MANUAL)
    ret = sub.validate_assets_with_settings(assets, settings)
    results = ret[1] if isinstance(ret, tuple) else ret
    lines.append("returned %s" % (ret[0] if isinstance(ret, tuple) else ret))
    for k in ("num_checked", "num_valid", "num_invalid", "num_skipped", "num_warnings", "num_unable_to_validate"):
        try:
            lines.append("%s %s" % (k, results.get_editor_property(k)))
        except Exception as e:  # noqa
            lines.append("%s n/a (%s)" % (k, e))
except Exception as e:  # noqa
    lines.append("validation call failed: %s" % e)
open(os.path.join(OUT, "validation.txt"), "w", encoding="utf-8").write("\n".join(lines) + "\n")
log("validation:", " | ".join(lines))
