;==============================================================================
; $C8F0-$CEFF - FRAME, ENEMY, PROJECTILE, AND MIXED CE GUIDE
;==============================================================================
;
; Contains the generic VIC sprite coordinate updater, watermark/checksum-style
; logic, per-frame dispatcher, enemy/projectile update loop, enemy wave/path
; initialization, animation managers, projectile logic, and the ambiguous
; $CE00-range mixed code/data area.
;
; This guide keeps the original memory order but breaks the dense region into
; navigable subfiles. The $CE00 tail is especially mixed; treat it carefully
; until Regenerator or emulator traces prove which bytes are code and which are
; data tables.
;
.include "frame_enemy_projectile/00_sprite_coordinates_idle_watermark.inc"
.include "frame_enemy_projectile/10_frame_dispatch_irq_reset.inc"
.include "frame_enemy_projectile/20_enemy_path_initialization.inc"
.include "frame_enemy_projectile/30_sprite_animation.inc"
.include "frame_enemy_projectile/40_player_projectile.inc"
.include "frame_enemy_projectile/50_ce_mixed_sfx_data.inc"
