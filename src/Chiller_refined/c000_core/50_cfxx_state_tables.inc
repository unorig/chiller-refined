;==============================================================================
; $CF00-$CFBD - LIVE GAME STATE TABLES
;==============================================================================
;
; Live variables and compact tables used by the C000 gameplay core: global
; timers, enemy wave coordinates, animation counters, projectile status, sprite
; active flags, level state, music status, and HUD score digits.
;
f_level_data_ptr_cf00
                    .byte $00, $00       ; x-ref: $0823
; Global tick counter (increments every frame, checked by fall physics).
a_global_tick_counter
                    .byte $62            ; x-ref: $538e, $5800, $ca0c
; Sub-frame physics delay timer. Triggers player physics every 32 ticks.
a_game_frame_timer  .byte $00            ; x-ref: $ca12, $ca15, $ca1e
; Joystick horizontal movement delta speed threshold limit.
a_joystick_x_limit  .byte $00            ; x-ref: $c9a4, $c9b6, $c9b9
; Joystick vertical movement delta speed threshold limit.
a_joystick_y_limit  .byte $00            ; x-ref: $c9a9, $c9ac, $c9af
; Sprite collision and enable active check mask flags.
a_physics_collision_flags
                    .byte $62            ; x-ref: $c71d, $c737, $c761, $c78b, $c7b5, ...
; Frame slow-down loop speed tick counter.
a_slow_down_counter .byte $62            ; x-ref: $5a67, $5a7c, $5a8f, $c827, $c82a, ...
; Enemy initial X coordinate MSB bit configuration parameters.
f_enemy_x_msb_init_table
                    .byte $00, $41, $00, $62, $00 ; x-ref: $2eb7, $2fc4, $cb3d, $cba1
; Enemy wave start position vector indices.
f_enemy_init_coordinate_index
                    .byte $00, $62, $00, $62, $00 ; x-ref: $56ea, $cb2a, $cb64, $cb94, $cbcd, ...
; Enemy wave spawning layout configuration indices.
a_enemy_init_slot_index
                    .byte $62            ; x-ref: $cb5e, $cb68, $cb75, $cbc7, $cbd1, ...
a_temp_reg_y        .byte $6f            ; x-ref: $cb7a, $cb85, $cb90, $cbb5, $cbe3, ...
a_watermark_counter .byte $1b            ; x-ref: $c9f1, $c9f4
; Enemy sprite animation frames lo-byte address pointers.
f_sprite_anim_frame_pointers
                    .byte $03, $0c, $00, $01, $1d ; x-ref: $2f49, $cc16, $cc61, $cc7b
; Enemy animation frame tick speed counters.
f_sprite_anim_frame_counters
                    .byte $21, $6e, $62, $00, $62, $00, $62, $00 ; x-ref: $2ff2, $c6c9, $cc22, $cc25, $cc2f
                    .byte $00, $62
; Enemy animation frame lookup table high-page pointers.
f_sprite_anim_table_hi
                    .byte $00            ; x-ref: $2f40, $5f84, $cc41, $cc74
a_sprite_anim_table_25
                    .byte $43            ; x-ref: $5f8a
a_sprite_anim_table_26
                    .byte $00            ; x-ref: $5f90
a_sprite_anim_table_27
                    .byte $62            ; x-ref: $5f96
a_sprite_anim_table_28
                    .byte $62            ; x-ref: $5f9c
f_sprite_anim_step_index
                    .byte $00, $00, $00, $62, $00 ; x-ref: $2fed, $c6cc, $cc46, $cc4a, $cc5a
f_sprite_anim_duration_counters
                    .byte $00, $62, $00, $62, $42 ; x-ref: $c6cf, $ccab, $ccae, $ccb8
f_sprite_anim_ticks .byte $62, $62, $62, $28, $03 ; x-ref: $5668, $ccd1, $ccd4, $ccde
f_sprite_anim_end_frames
                    .byte $0a            ; x-ref: $5ed0, $cce7
a_sprite_anim_param_39
                    .byte $20            ; x-ref: $5ed6
a_sprite_anim_param_3a
                    .byte $01            ; x-ref: $5edc
a_sprite_anim_param_3b
                    .byte $bd            ; x-ref: $5ee2
a_sprite_anim_param_3c
                    .byte $28            ; x-ref: $5ee8
; Enemy animation frame sequences loop constraints.
f_sprite_anim_loop_frames
                    .byte $62            ; x-ref: $5eb2, $ccec
a_sprite_anim_param_3e
                    .byte $62            ; x-ref: $5eb8
a_sprite_anim_param_3f
                    .byte $62            ; x-ref: $5ebe
a_sprite_anim_param_40
                    .byte $44            ; x-ref: $5ec4
a_sprite_anim_param_41
                    .byte $62, $00       ; x-ref: $5eca
a_sprite_anim_global_timer
                    .byte $62            ; x-ref: $c018, $c023, $c525, $c538
; Player projectile horizontal/vertical direction vectors.
a_sprite_projectile_dir
                    .byte $00            ; x-ref: $cd1b, $cd65
a_sprite_projectile_steps
                    .byte $00            ; x-ref: $cd55, $cd58, $cd62
f_sprite_enable_bits
                    .byte $62, $00, $00, $00, $62, $62 ; x-ref: $c000, $c0ee, $c62f, $cd46, $cdaa
a_temp_projectile_y .byte $00, $00, $00  ; x-ref: $cd81, $cd93
; Sprite active and configuration status registers.
f_sprite_active_status
                    .byte $62, $00, $00, $62, $00 ; x-ref: $c63c, $c6d2, $ca70, $cdea, $cdfa
; Individual sprite speed delay interval counters.
f_sprite_movement_delays
                    .byte $62, $00, $62, $62, $62, $28 ; x-ref: $c61b, $c61e, $c642
a_score_carry_flag  .byte $02            ; x-ref: $59d2, $5ae9
a_score_block_param_b
                    .byte $0d            ; x-ref: $59d7, $5aee
a_score_block_param_c
                    .byte $32, $3c       ; x-ref: $59dc, $5af3
a_sprite_enable_mask_cf5e
                    .byte $a0            ; x-ref: $2f95
a_sfx_index_cf5f    .byte $a0            ; x-ref: $2fa0
a_flicker_counter   .byte $11, $0d, $0d  ; x-ref: $ca9f, $caa7, $cad2
a_flicker_slot_index
                    .byte $02, $28       ; x-ref: $caa4, $cac8, $cacb
a_delay_loop_param_lo
                    .byte $62            ; x-ref: $2dad, $5594, $563f, $c6eb, $cabd
a_delay_loop_param_hi
                    .byte $62            ; x-ref: $2db0, $5597, $5642, $c6f6, $cac2
a_delay_loop_working_ctr
                    .byte $62            ; x-ref: $c6ee, $c6f1
a_level_status_param_68
                    .byte $00            ; x-ref: $5e94, $c0f7
a_level_status_param_69
                    .byte $62            ; x-ref: $5e9a
a_level_status_param_6a
                    .byte $00            ; x-ref: $5ea0
a_level_status_param_6b
                    .byte $62            ; x-ref: $5ea6
a_level_status_param_6c
                    .byte $00            ; x-ref: $5eac
f_level_status_table_6d
                    .byte $00, $62, $00, $00, $00, $62 ; x-ref: $2f20, $2f80
a_level_pass_score  .byte $62            ; x-ref: $2c6a
a_level_status_flag .byte $00            ; x-ref: $2c8b
a_level_tick_timer  .byte $00            ; x-ref: $2fb2, $2fb5, $2fbf
f_level_enemy_type_seeds
                    .byte $00, $62, $00, $00, $62, $00 ; x-ref: $2d05, $2d4d, $2fce, $2fe8, $5672
; VIC system configuration and interrupt flags.
a_sprite_system_config_val
                    .byte $62            ; x-ref: $2e65, $caed
a_sprite_process_flag
                    .byte $00, $62       ; x-ref: $2d7b, $2e6a, $5a72
; Stage active state status flag (equal to $FF on exit/stage clear).
a_music_play_status_flag
                    .byte $62, $62       ; x-ref: $2cc2, $2db6, $2e7d, $ca57
a_logic_tick_flag   .byte $28            ; x-ref: $2ed7, $ca42, $ca4b
f_enemy_move_delay_counters
                    .byte $02, $0d, $12, $07, $0a, $0b, $07, $11 ; x-ref: $c607, $c60a, $c613
                    .byte $0d, $02, $28, $62, $62, $62, $00, $62
                    .byte $00, $62, $00, $00, $62
f_enemy_anim_tick_counters
                    .byte $00, $00, $00, $62, $62 ; x-ref: $ccbf, $ccc2, $cccb
f_enemy_anim_max_ticks
                    .byte $00            ; x-ref: $5686, $568c, $5690, $5697, $56b2, ...
a_enemy_anim_param_9d
                    .byte $00            ; x-ref: $5f12
a_enemy_anim_param_9e
                    .byte $00            ; x-ref: $5f18
a_enemy_anim_param_9f
                    .byte $62            ; x-ref: $5f1e
a_enemy_anim_param_a0
                    .byte $00            ; x-ref: $5f24
f_vic_irq_config_val_cfa1
                    .byte $00, $62, $00, $62, $00 ; x-ref: $2c57
; HUD energy and active score digits layout pointers.
f_hud_score_digit_1 .byte $62            ; x-ref: $2c1b, $2c1e, $2c27
a_hud_score_digit_2 .byte $62            ; x-ref: $2ca6
a_hud_score_digit_3 .byte $62            ; x-ref: $2ca9
a_hud_score_digit_4 .byte $28, $02, $0d, $0e, $a0, $11, $12, $a0 ; x-ref: $2cae
                    .byte $a0, $0d, $02, $28, $62, $62, $62, $00
                    .byte $62, $00, $62, $00, $00
;==============================================================================
; $CFBE-$EA30 - MIXED EXTERNAL DATA / STATE / ASSET REGION
;==============================================================================
;
; This exported block contains data that Regenerator has kept outside the main
; listing. Earlier analysis suggests it includes asset/state material rather
; than clean relocatable code. Keep it external until its internal structure is
; mapped more confidently.
;
