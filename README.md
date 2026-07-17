# Chiller Refined

Standalone release folder for the Chiller gameplay refinement mod.

## What This Is

This is a buildable modded disassembly of `Chiller (1985)(Mastertronic)` with the current tested gameplay changes:

- Jump sound removed.
- Faster, cleaner jump arc.
- Horizontal air control while jumping/falling.
- Double jump.
- Jump/fall sprite direction fixes.
- Magic cross collection no longer cancels the jump arc.
- Rope climbing work-in-progress changes.
- Restored vertical enemy movement routine after the rope helper was moved out of `$5300`.

## Folder Layout

- `dist/Chiller Refined.prg` - current rebuilt PRG.
- `src/Chiller Refined.asm` - main 64tass entry point.
- `src/Chiller_refined/` - modular source tree.
- `src/chiller_refined_asset_*.bin` - extracted binary asset blocks required by the source includes.
- `tools/build.sh` - rebuilds the PRG into `dist/`.
- `tools/launch_vice.sh` - launches the PRG in VICE C64SC with sound disabled.

## Build

From this folder:

```sh
tools/build.sh
```

Expected output:

```text
Data:      63487   $0801-$ffff   $f7ff
```

The rebuilt PRG should be `63489` bytes.

## Test In VICE

```sh
tools/launch_vice.sh
```

## GitHub Note

Be careful publishing the rebuilt PRG or extracted `.bin` assets publicly unless you are sure you have the right to redistribute the original game content. A safer public GitHub release is often:

- source notes,
- patch/diff files,
- build instructions,
- screenshots,
- and a release description explaining that users need their own original copy.
