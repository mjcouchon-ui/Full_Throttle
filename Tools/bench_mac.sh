#!/bin/bash
# FULL THROTTLE UNREAL benchmark on a Mac (2.6.1 performance audit): runs the editor binary in -game mode (no editor
# around it) through the WispOS backstage launcher, with -ftbench: the autopilot drives the level, then
# bench/bench_L<level>_<PRESET>_<tag>.json is written and the game quits.
# Usage: Tools/bench_mac.sh <tag> <level 0-7> <quality -1 auto|0..3> <seconds> <WxH> [extra args, e.g. '-ExecCmds=r.ScreenPercentage 67' or -ftexec=<file>]
# Needs: the editor modules built (Build.sh FullThrottleEditor Mac Development ... -NoHotReloadFromIDE) and
# Saved/Config/MacEditor/GameUserSettings.ini with FullscreenMode=2 (a hidden backstage window hangs in a full-screen toggle).
TAG=$1; L=$2; Q=$3; S=$4; RES=$5; shift 5
W=${RES%x*}; H=${RES#*x}
P="$(cd "$(dirname "$0")/.." && pwd)"
OUT=${FTPERF_OUT:-/private/tmp/ftperf/out}/$TAG; mkdir -p "$OUT"
export WISPOS_APP_DATA="$OUT/appdata"; mkdir -p "$WISPOS_APP_DATA"
~/Applications/WispOS.app/Contents/MacOS/wispos --backstage -- "/Users/Shared/Epic Games/UE_5.9/Engine/Binaries/Mac/UnrealEditor.app/Contents/MacOS/UnrealEditor" "$P/FullThrottle.uproject" -game \
  -ftbench -ftlevel=$L -ftquality=$Q -ftseconds=$S -fttag=_$TAG -windowed -ResX=$W -ResY=$H -nosound -NoVerifyGC -unattended -log -abslog="$OUT/game_L$L.log" "$@" 2>&1 | grep -v "^\[" | tail -3
cat "$WISPOS_APP_DATA"/bench/bench_L${L}_*.json 2>/dev/null; echo
