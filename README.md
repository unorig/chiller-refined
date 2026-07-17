# Chiller Refined

Gameplay refinement project for the Commodore 64 version of **Chiller** by
Mastertronic.

## About The Original Game

**Chiller** is a budget C64 platform game published by Mastertronic in the
mid-1980s. It is a single-player, multi-screen horror platformer built around
five themed areas: the forest, cinema, ghetto, graveyard, and haunted house.

The game casts the player as a hero trying to rescue his girlfriend from the
House of the Undead. Progression is based around collecting magic crosses while
avoiding energy-draining enemies such as ghouls, bats, ghosts, and zombies.
Mushrooms can restore energy, but some are poisonous. The original C64 controls
use joystick port 2, or keyboard controls with `Z` for left, `C` for right, and
`Shift` for jump.

This project is for the Mastertronic C64 platform game, not the unrelated Exidy
light-gun game also called Chiller.

Useful references:

- [Lemon64 game entry](https://www.lemon64.com/game/chiller)
- [Lemon64 manual text](https://www.lemon64.com/doc/chiller/139)
- [Mastertronic Collectors Archive review](https://mastertronic.co.uk/game-review-chiller-commodore-64-mastertronic/)
- [Games That Weren't: Chiller V1](https://www.gamesthatwerent.com/gtw64/chiller-v1/)

## About This Project

This is a buildable modded disassembly of `Chiller (1985)(Mastertronic)` with the current tested gameplay changes:

- Jump sound removed.
- Faster, cleaner jump arc.
- Horizontal air control while jumping/falling.
- Double jump.
- Jump/fall sprite direction fixes.
- Magic cross collection no longer cancels the jump arc.
- Rope climbing work-in-progress changes.
- Restored vertical enemy movement routine after the rope helper was moved out of `$5300`.

The aim is not to remake Chiller into a different game. The intent is to keep
the original look, structure, and atmosphere while refining rough edges in the
movement, collision, and level feel. Future changes may include map tweaks,
enemy behavior improvements, and further quality-of-life fixes.

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
