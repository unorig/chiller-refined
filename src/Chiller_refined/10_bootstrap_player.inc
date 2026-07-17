;==============================================================================
; $0801-$2FFF - BOOTSTRAP, ASSETS, PLAYER COLLISION, EARLY GAME INIT GUIDE
;==============================================================================
;
; This region starts the game, seeds the live $CE/$CF pages, includes early
; sprite/graphics assets, then enters the player collision and early game setup
; routines.
;
; Files:
;   bootstrap_player/00_bootstrap_watermark_init.inc
;     BASIC stub, s_start, ANTISOFT watermark, initial VIC register data, and
;     byte tables copied into $CE00/$CF00.
;
;   bootstrap_player/10_sprite_graphics_assets.inc
;     External enemy sprite block and large graphics/data block that precedes
;     executable player collision code.
;
;   bootstrap_player/20_player_collision_copy_helpers.inc
;     Player position/grid collision, fall trigger state, block-copy setup,
;     screen row stepping, render/decompress helpers, and screen control setup.
;
;   bootstrap_player/30_score_irq_level_init.inc
;     Enemy direction init, score increment/reset, level start, IRQ copy,
;     set_irq_and_mem, state flag setup, and level-state load.
;
;   bootstrap_player/40_enemy_sprite_sid_setup.inc
;     Enemy count init, enemy active loop, sprite init/menu/death handling,
;     sprite system init, X-MSB/SID retrigger, sprite color and SID setup.
;
;   bootstrap_player/50_zp_reads_enemy_respawn.inc
;     Zero-page indirect reads into sprite tables, IRQ/sprite reset, and periodic
;     enemy respawn ticker.
;
;==============================================================================

.include "bootstrap_player/00_bootstrap_watermark_init.inc"
.include "bootstrap_player/10_sprite_graphics_assets.inc"
.include "bootstrap_player/20_player_collision_copy_helpers.inc"
.include "bootstrap_player/30_score_irq_level_init.inc"
.include "bootstrap_player/40_enemy_sprite_sid_setup.inc"
.include "bootstrap_player/50_zp_reads_enemy_respawn.inc"
