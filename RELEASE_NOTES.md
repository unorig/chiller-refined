# Release Notes

## Chiller Refined - Working Build

Current rebuilt PRG:

- `dist/Chiller Refined.prg`
- Size: `63489` bytes
- Load/data range: `$0801-$FFFF`

## Gameplay Changes

- Removed the jump sound effect.
- Added modern-style horizontal air control.
- Added double jump.
- Tuned jump timing to remove the long pause at the top of the arc.
- Preserved correct facing/jump sprites when changing direction in the air.
- Made falling use the jump-style pose rather than cycling walk frames.
- Fixed magic cross collection so collecting a cross mid-jump continues normal movement.
- Moved rope helper code out of `$5300` and restored the original routine there, fixing vertical enemy movement wrapping.

## Movement Details

- The top-of-jump pause was removed by making `fall_phase_flip` continue straight
  into descent instead of returning for an extra suspended beat.
- The current approved jump timing uses the 20-percent-faster post-half-speed
  easing curve in `f_player_jump_easing_table`.
- Horizontal jump travel was kept under control after removing the temporary
  airborne physics speed-up.

## Rope Details

- Upward rope climbing uses the normal UP animation pair (`d8/d9` boy,
  `e8/e9` girl) instead of the jump-looking rope-slide frames.
- Rope-UP checks rope contact first. If the player is on a rope, UP performs a
  direct climb step; otherwise UP falls through to the normal jump path.
- Rope helper code was moved out of `$5300`, restoring the original routine used
  by vertical enemy movement.

## Current Caveat

Rope climbing is still being tuned. The latest build keeps the main jump behavior intact and avoids the enemy movement corruption, but the rope-up feel may still need more work.
