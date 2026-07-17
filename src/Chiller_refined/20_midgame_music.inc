;==============================================================================
; $3000-$61F1 - MIDGAME TABLES, PLAYER HELPERS, GAME FLOW, SID MUSIC GUIDE
;==============================================================================
;
; This region used to be a large mixed file containing external data, intro
; strings, live state tables, player fall/SFX helpers, per-frame game logic,
; level transition code, and the SID music IRQ player.
;
; It is split here by topic while preserving the original byte order.
;
; Files:
;   midgame/00_intro_config_tables.inc
;     External $3000-$40FF buffer/font asset block, intro text, music stream
;     fragments, collision state, player/enemy config, and level seed tables.
;
;   midgame/10_screen_helpers_unused.inc
;     Screen row offset tables, draw-char helper, and routines currently marked
;     as unreferenced pixel/color manipulation helpers.
;
;   midgame/20_player_fall_sfx.inc
;     Player fall/landing checks, jump/easing state, collision snapshot helpers,
;     and SID voice 3 fall/sweep sound effects.
;
;   midgame/30_input_sprite_setup.inc
;     Input scan helpers, sprite pointer updates, path buffer setup, enemy-count
;     reset, HUD score/lives drawing, and level animation table references.
;
;   midgame/40_main_frame_body.inc
;     Main per-frame gameplay body: state patching, joystick/rope handling,
;     player/girl transition, health tick logic, collision side effects.
;
;   midgame/50_game_state_level_flow.inc
;     Game-state dispatcher, level start/end, collectibles, health/damage,
;     enemy spawning, score refresh, high-score and level config loading.
;
;   midgame/60_sid_music_irq.inc
;     SID music data handler, filter setup, IRQ install, page-$61 command
;     dispatcher, disable-music routine, and music stream byte reader.
;
;==============================================================================

.include "midgame/00_intro_config_tables.inc"
.include "midgame/10_screen_helpers_unused.inc"
.include "midgame/20_player_fall_sfx.inc"
.include "midgame/30_input_sprite_setup.inc"
.include "midgame/40_main_frame_body.inc"
.include "midgame/50_game_state_level_flow.inc"
.include "midgame/60_sid_music_irq.inc"
