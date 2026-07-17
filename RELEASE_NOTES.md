# Release Notes

## Chiller Refined - Working Build

Local rebuilt PRG:

- Output path: `dist/Chiller Refined.prg`
- Size: `63489` bytes
- Load/data range: `$0801-$FFFF`
- The rebuilt PRG is a local build output and is not published in this repo.

## Gameplay Changes

- Removed the jump sound effect.
- Added modern-style horizontal air control.
- Added double jump.
- Tuned jump timing to remove the long pause at the top of the arc.
- Preserved correct facing/jump sprites when changing direction in the air.
- Fixed magic cross collection so collecting a cross mid-jump continues normal movement.
- Changed rope climbing so UP while touching a rope uses a dedicated climb path
  and the normal upward animation rather than the original jump-like rope motion.
- Moved rope helper code out of `$5300` and restored the original routine there, fixing vertical enemy movement wrapping.

## Movement Details

- The top-of-jump pause was removed by making `fall_phase_flip` continue straight
  into descent instead of returning for an extra suspended beat.
- The current approved jump timing uses the 20-percent-faster post-half-speed
  easing curve in `f_player_jump_easing_table`.
- Horizontal jump travel was kept under control after removing the temporary
  airborne physics speed-up.

## Rope Details

- In the original game, moving up a rope feels close to repeated jump behavior.
  Chiller Refined separates rope-UP from the jump/fall path so climbing reads
  more like intentional rope movement.
- Upward rope climbing uses the normal UP animation pair (`d8/d9` boy,
  `e8/e9` girl) instead of the jump-looking rope-slide frames.
- Rope-UP checks rope contact first. If the player is on a rope, UP performs a
  direct climb step; otherwise UP falls through to the normal jump path.
- Rope helper code was moved out of `$5300`, restoring the original routine used
  by vertical enemy movement.
