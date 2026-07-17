;==============================================================================
; $C000-$CFFF - MAIN GAMEPLAY CORE GUIDE
;==============================================================================
;
; The C000 page is the densest and most useful part of the disassembly. It is
; now split into smaller files so the work can move from "giant wall of code"
; toward actual subsystems.
;
; These includes are still emitted in address order. That keeps the build
; byte-identical while making the source easier to read and giving us safe
; future move points.
;
; Files:
;   c000_core/00_screen_collision.inc
;     Sprite overlap entry, screen clear routines, character/background mask
;     collision, quick screen/color clear, and the vertical enemy activator.
;
;   c000_core/10_player_input_blockcopy.inc
;     Sprite 0 restore, HUD row clear, movement state bytes, joystick decoding,
;     manual sprite X movement loops, and the self-modifying block-copy engine.
;
;   c000_core/20_scrolling_hud_flicker.inc
;     Intro/path stepping, row scroller, raster sync scroll driver, HUD setup,
;     sprite-1 flicker, sprite mirror/bit-flip collision helper.
;
;   c000_core/30_enemy_physics_dispatch.inc
;     Enemy respawn timing, HUD drawing, enemy array reset, color updates, delay
;     loops, sprite-1 position, player physics dispatcher, and joystick checks.
;
;   c000_core/40_frame_enemy_projectile.inc
;     Coordinate updater, watermark checker, frame dispatcher, enemy/projectile
;     update loop, enemy wave/path setup, animation manager, projectile logic,
;     and the ambiguous $CE00-range mixed code/data.
;
;   c000_core/50_cfxx_state_tables.inc
;     $CF00 live state tables for level data, global timers, enemy coordinates,
;     animation counters, projectile flags, sprite status, level state, music
;     status, and HUD score digits.
;
;==============================================================================

.include "c000_core/00_screen_collision.inc"
.include "c000_core/10_player_input_blockcopy.inc"
.include "c000_core/20_scrolling_hud_flicker.inc"
.include "c000_core/30_enemy_physics_dispatch.inc"
.include "c000_core/40_frame_enemy_projectile.inc"
.include "c000_core/50_cfxx_state_tables.inc"
