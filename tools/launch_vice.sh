#!/bin/sh
set -eu

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
PRG="$ROOT/dist/Chiller Refined.prg"

exec x64sc -sounddev dummy "$PRG"
