#!/usr/bin/env bash
# FULL THROTTLE UNREAL benchmark: runs the packaged Shipping game the way WispOS launches it
# (WISPOS_APP_DATA + a matching launch ticket), with -ftbench: the autopilot drives the level for N seconds
# at a fixed graphics preset, then the game writes bench/bench_L<level>_<PRESET>.json and quits.
# Usage: bench.sh <path to FullThrottle.exe> <out dir> "<level:quality> ..." [seconds] [WxH]
set -u
EXE="$1"; OUT="$2"; RUNS="$3"; SECS="${4:-60}"; RES="${5:-1920x1080}"
W="${RES%x*}"; H="${RES#*x}"
mkdir -p "$OUT"
for run in $RUNS; do
  L="${run%:*}"; Q="${run#*:}"
  APPDATA_DIR="$(mktemp -d)"
  TICKET="bench-$RANDOM$RANDOM"
  printf '%s' "$TICKET" > "$APPDATA_DIR/launch-ticket"
  WIN_APPDATA="$(cygpath -w "$APPDATA_DIR")"
  echo "== level $L quality $Q ($RES, ${SECS}s)"
  WISPOS_APP_DATA="$WIN_APPDATA" WISPOS_LAUNCH_TICKET="$TICKET" \
    timeout $((SECS + 150)) "$EXE" -ftbench -ftlevel="$L" -ftquality="$Q" -ftseconds="$SECS" \
    -windowed -ResX="$W" -ResY="$H" -nosound -NoVerifyGC
  cp "$APPDATA_DIR"/bench/*.json "$OUT/" 2>/dev/null && cat "$APPDATA_DIR"/bench/*.json && echo
  rm -rf "$APPDATA_DIR"
done
