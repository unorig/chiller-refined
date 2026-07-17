#!/bin/sh
set -eu

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
ASM="$ROOT/src/Chiller Refined.asm"
OUT="$ROOT/dist/Chiller Refined.prg"

TASS64="${TASS64:-/opt/homebrew/bin/64tass}"

"$TASS64" --labels="$ROOT/dist/Chiller Refined.labels" -o "$OUT" "$ASM"
wc -c "$OUT"
