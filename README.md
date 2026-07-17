# Chiller Refined

Gameplay refinement project for the Commodore 64 version of **Chiller** by
Mastertronic.

## About the Original Game

**Chiller** is a budget C64 platform game published by Mastertronic in the
mid-1980s. It is a single-player, multi-screen horror platformer built around
five themed areas: the forest, cinema, ghetto, graveyard, and haunted house.

## About This Project

Chiller Refined is a playable update to **Chiller** that keeps the original
game intact while smoothing out some of its rougher movement and collision
behavior.

Player-visible changes include:

- Jumping no longer plays the repetitive jump sound.
- Jumping has a cleaner arc with less hang time at the top.
- The player can steer left and right while jumping or falling.
- A second jump can be triggered while airborne.
- The correct left/right jump sprite is used when changing direction in the air.
- Collecting a magic cross during a jump no longer interrupts the jump.
- Rope climbing feels smoother and less like repeated jumps.

The aim is not to remake Chiller into a different game. The intent is to keep
the original look, structure, and atmosphere while making the game feel a little
fairer and more responsive to play.

## Folder Layout

- `src/Chiller Refined.asm` - main 64tass entry point.
- `src/Chiller_refined/` - modular source tree.
- `src/Chiller_refined/assets/` - extracted binary asset blocks required by the source includes.
- `tools/build.sh` - rebuilds the PRG locally into `dist/`.
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

The rebuilt PRG is written to `dist/Chiller Refined.prg` and should be `63489`
bytes. Generated PRG files are local build outputs and are not tracked in this
repo.

## Test In VICE

```sh
tools/launch_vice.sh
```
