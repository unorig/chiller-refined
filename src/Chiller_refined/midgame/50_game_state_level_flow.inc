;==============================================================================
; $5B00-$6000 - GAME STATE, LEVEL FLOW, DAMAGE, SCORE, HIGH SCORE GUIDE
;==============================================================================
;
; This is the high-level game-state switchboard. It reacts to the current level
; status, initializes new levels, updates score/lives/health, checks enemy and
; projectile results, and decides when to transition between gameplay, death,
; level-complete, title/menu, and high-score states.
;
; The nested files keep the original byte order while separating the major
; concerns: collision pickups/damage, level-start drawing, HUD/player screen
; setup, and the long level-configuration loader.
;
.include "level_flow/00_collision_collectibles_damage.inc"
.include "level_flow/10_level_start_screen_draw.inc"
.include "level_flow/20_hud_player_screen_setup.inc"
.include "level_flow/30_level_config_score_map.inc"
