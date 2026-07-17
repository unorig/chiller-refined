;==============================================================================
; $3000-$51C3 - INTRO TEXT, CONFIG TABLES, PLAYER/ENEMY STATE SEEDS
;==============================================================================
;
; Starts with the externalized zero-init/font/buffer block, then defines intro
; text and compact tables used by player collision, enemies, animation timing,
; level setup, sprite priority, SID voice presets, and respawn behavior.
;
.binary "../assets/chiller_refined_asset_3000_40ff_tables.bin"
                    .encode 
                    .enc "none"
; Forest intro (short): 'MOVE THE BOY AROUND THE FOREST COLLECTING THE BLUE MAGIC CROSSES.'
f_intro_forest_v1   .text $0f, $0f, $12, $1c, $8e, $08, "THE FOREST", $11, $11, $11, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $1f
                    .text "MOVE THE BOY AROUND THE FOREST", $11, "        C"
                    .text "OLLECTING THE BLUE MAGIC CROSSES.", $01, $ff, $ff, $ff, $ff, $ff
; Cinema intro (short): 'THERE'S A MESSAGE SCRAWLED IN BLOOD...'
f_intro_cinema_v1   .text $ff, $0f, $0f, $12, $1c, "THE CINEMA", $11, $11, $0d, $1f, $12, " THERE", $91, ",", $11, "S A MESSAGE"
                    .text " SCRAWLED IN BLOOD...", $01, "N."
; Ghetto intro (short): 'SAVE YOUR ENERGY, YOU'LL NEED IT.'
f_intro_ghetto_v1   .text $01, $0f, $0f, $12, $1c, "THE GHETTO", $11, $11, $11, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $1f
                    .text "SAVE YOUR ENERGY, YOU", $91, ",", $11, "LL NEED IT.", $01, $ff, $ff, $ff
; Graveyard intro (short): 'THE MIDNIGHT HOUR IS CLOSE AT HAND, YOUR DEATH AWAITS IN THIS EVIL LAND.'
f_intro_graveyard_v1
                    .text $ff, $ff, $0f, $0e, $1c, $12, "THE GRAVEYARD", $11, $11, $11, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d, $9d
                    .text $9d, $9d, $9d, $9d, $9d, $9d, $9d, $1f, "THE MIDNIGHT HOUR IS CLOSE AT HA"
                    .text "ND,", $11, "     YOUR DEATH AWAITS IN THIS EVIL "
                    .text "LAND.", $01, $ff, $ff, $ff, $ff, $0f
; Haunted House intro: 'YOUR GIRLFRIEND YOU ARE SEARCHING FOR, WHEN YOU HAVE THE CROSSES SHE'LL APPEAR BY THE DOOR.'
f_intro_haunted_house_v1
                    .text $0c, $1c, $12, "THE HAUNTED HOUSE", $11, $11, $1f, $0d, $12, " YOUR GIRLFRIEN"
                    .text "D YOU ARE SEARCHING FOR,", $11, " WHEN YOU HAVE "
                    .text "THE CROSSES SHE", $91, ",", $11, "LL APPEAR", $11, "            "
                    .text "   BY THE DOOR.", $01, $ff, $ff, $ff, $ff, $ff, $ff, $ff, $0f, $0c, $1c, $12
; Haunted House intro (homeward): 'BOTH BOY AND GIRL MUST TURN AROUND, BECAUSE YOUR QUEST IS HOMEWARD BOUND.'
f_intro_haunted_house_v2
                    .text "THE HA"
                    .endencode 
; SID music command data stream ($4302 area): pointed to by snd_ptr1/2 ($B7/$B8, $B9/$BA). Parsed by music_irq_handler each frame.
f_music_data        .byte $55, $4e, $54, $45, $44, $20, $48, $4f
                    .byte $55, $53, $45, $11, $11, $1f, $0d, $12
                    .byte $20, $20, $42, $4f, $54, $48, $20, $42
                    .byte $4f, $59, $20, $41, $4e, $44, $20, $47
                    .byte $49, $52, $4c, $20, $4d, $55, $53, $54
                    .byte $20, $54, $55, $52, $4e, $20, $41, $52
                    .byte $4f, $55, $4e, $44, $2c, $0d, $12, $11
                    .byte $20, $20, $42, $45, $43, $41, $55, $53
                    .byte $45, $20, $59, $4f, $55, $52, $20, $51
                    .byte $55, $45, $53, $54, $20, $49, $53, $20
                    .byte $48, $4f, $4d, $45, $57, $41, $52, $44
                    .byte $20, $42, $4f, $55, $4e, $44, $2e, $01
                    .byte $ff, $ff, $ff, $ff, $ff, $ff, $0f, $0e
                    .byte $1c, $12, $54, $48, $45, $20, $47, $52
                    .byte $41, $56, $45, $59, $41, $52, $44, $11
                    .byte $11, $1f, $0d, $12, $20, $20, $20, $20
                    .byte $42, $59, $20, $4e, $4f, $57, $20, $59
                    .byte $4f, $55, $52, $20, $45, $4e, $45, $52
                    .byte $47, $59, $20, $4d, $55, $53, $54, $20
                    .byte $42, $45, $20, $4c, $4f, $57, $2c, $0d
                    .byte $12, $11, $20, $20, $53, $4f, $20, $46
                    .byte $49, $4e, $44, $20, $41, $20, $4d, $55
                    .byte $53, $48, $52, $4f, $4f, $4d, $20, $42
                    .byte $45, $46, $4f, $52, $45, $20, $59, $4f
                    .byte $55, $20, $53, $4c, $4f, $57, $2e, $01
                    .byte $ff, $ff, $ff, $ff, $ff, $ff, $0f, $0f
                    .byte $12, $1c, $54, $48, $45, $20, $47, $48
                    .byte $45, $54, $54, $4f, $11, $11, $1f, $0d
                    .byte $12, $20, $20, $20, $20, $20, $20, $54
                    .byte $48, $45
                    .encode 
                    .enc "none"
; Graveyard intro (long): 'BY NOW YOUR ENERGY MUST BE LOW, SO FIND A MUSHROOM BEFORE YOU SLOW.'
f_intro_graveyard_v2
                    .text " GHETTO IS A LONELY PLACE,", $0d, $12, $11, "  SO GET YO"
                    .text "UR CROSSES WITH GREAT HASTE.", $01, $ff, $ff, $ff, $ff, $ff, $ff, $ff, $0f, $0f, $12, $1c
                    .text "THE CINEMA", $11, $11, $1f, $0d, $12, "    THE F"
; Ghetto intro (long): 'THE GHETTO IS A LONELY PLACE, SO GET YOUR CROSSES WITH GREAT HASTE.'
f_intro_ghetto_v2   .text "INAL MINUTES ARE WITH YOU,", $0d, $12, $11, "           "
                    .text "  USE THEM WELL.", $01, $ff, $ff, $ff, $0f, $0f, $1c, $12
; Forest intro (long/final): 'THE END IS NIGH, YOU'RE GOING TO DIE.'
f_intro_forest_v2   .text "THE FOREST", $11, $11, $1f, $0d, $12, " THE END IS NIGH, YOU", $91, ",", $11, "R"
; Cinema intro (long): 'THE FINAL MINUTES ARE WITH YOU, USE THEM WELL.'
f_intro_cinema_v2   .text "E GOING TO DIE.", $01, $ff, $ff, $ff, $ff, $a0, $a0, $a0, $a0, $a0, $a0, $a0, $a0, $a0, $a0, $a0, $a0, $a0, $a0, $a0, $a0
                    .endencode 
a_col_active_up     .byte $00            ; UP collision active flag: non-zero = player colliding with solid tile above. ; x-ref: $5658, $c72a
a_col_active_down   .byte $00            ; DOWN collision active flag. ; x-ref: $570a, $c753
a_col_active_left   .byte $01            ; LEFT collision active flag. ; x-ref: $5386, $53f1, $c77d
a_var_on_rope       .byte $01            ; Rope contact flag: non-zero = player is on a rope tile. ; x-ref: $5389, $53f4, $58b3, $c7a7
a_col_frame_up      .byte $01            ; x-ref: $c73a
a_col_frame_down    .byte $01            ; x-ref: $c764
a_col_frame_left    .byte $01            ; x-ref: $c78e
a_col_frame_right   .byte $01            ; x-ref: $c7b8
f_dir_index_table   .byte $00, $01, $02, $03 ; x-ref: $c7e2
a_flag_slow_movement
                    .byte $02            ; x-ref: $596f, $5973, $5a62, $5a77, $5a8a, ...
a_movement_timer_active
                    .byte $00            ; x-ref: $c819
a_sprite_system_config
                    .byte $f6            ; x-ref: $2e6d, $2e9a, $c4cd
a_scroll_enable_flag
                    .byte $f0            ; x-ref: $c64c
a_xmsb_scroll_config
                    .byte $0a            ; x-ref: $5551, $c40a
a_movement_mode     .byte $00            ; x-ref: $c852
a_col_right_bound   .byte $3c            ; x-ref: $c871, $c87d
a_col_left_bound    .byte $3c            ; x-ref: $c889
a_col_top_bound     .byte $0c            ; x-ref: $c895
a_col_bottom_bound  .byte $14            ; x-ref: $c8a1
a_player_x          .byte $80            ; Player sprite 0 saved X position. Written by set_sprite0_x, read by restore_sprite0_position. ; x-ref: $5532, $55a3, $7f20, $c147
a_player_y          .byte $e0            ; Player sprite 0 saved Y position. Written by set_sprite0_y. ; x-ref: $5538, $55a9, $7f27, $c032
a_player_xmsb       .byte $00            ; x-ref: $5543, $55b1, $5e81, $7f2e, $c1e6
; Enemy initial X positions (5 entries, one per slot). Set from level config. Used by init_sprite_movement.
a_enemy_xpos        .byte $00            ; Enemy initial X positions (5 entries). Set from level config, used by init_sprite_movement. ; x-ref: $56ee, $5fa4, $cb2e, $cb98
; Enemy initial Y positions (5 entries). Used by init_sprite_movement.
a_enemy_ypos        .byte $d0            ; x-ref: $56f4, $cb34, $cc01
; Enemy X MSB bits (5 entries): bit set = enemy sprite X > 255.
a_enemy_xmsb        .byte $00            ; x-ref: $56ff, $cb48, $cbac
; Enemy direction sequence length (5 entries): number of direction entries in movement table. 0=stationary.
a_enemy_dir_count   .byte $00            ; x-ref: $cb52, $cb6b
f_enemy_path_step_limit
                    .byte $20, $20, $f0, $00, $00, $00, $00, $d0 ; x-ref: $cbbb, $cbd4
                    .byte $00, $00, $20, $00, $30, $00, $00, $70
                    .byte $50, $d0, $40, $00, $80, $09, $10, $00
                    .byte $00, $10, $00
; Enemy animation end frame (5 entries): animation loops when sprite pointer reaches this value.
a_enemy_anim_end_frame
                    .byte $00, $00, $00, $00, $00 ; x-ref: $cc91
; Enemy animation loop-back frame (5 entries): when end_frame hit, reset sprite pointer to this.
a_enemy_anim_loop_frame
                    .byte $00, $00, $00, $00, $00 ; x-ref: $cc32
; Enemy initial direction values (5 entries, $20 each = direction code). Loaded by init_enemy_dirs.
f_enemy_init_dirs   .byte $20, $20, $20, $20, $20, $dc ; x-ref: $2c02
f_enemy_init_seed_table
                    .byte $04, $05, $04, $08, $08 ; x-ref: $56b5
; Enemy type/sprite table (5 entries): sprite data index for each active enemy slot. Used by enemy_active_dispatch.
f_enemy_type_table  .byte $08            ; x-ref: $cd26
f_enemy_sprite7_index_list
                    .byte $04, $04, $04, $04, $04 ; x-ref: $ca77
a_sprite7_range     .byte $10            ; x-ref: $cd04
a_sprite7_steps     .byte $10            ; x-ref: $cd5b
; Animation timer enable config (6 entries): $FF=disabled for that sprite. Used by sprite_anim_frame_tick.
f_anim_timer_config .byte $ff, $01, $00, $01, $00, $00 ; x-ref: $c011, $c020
f_enemy_init_movement_delay
                    .byte $40, $10, $20, $04, $10, $e7, $c8, $4d ; x-ref: $c63f
                    .byte $21, $21, $03, $00, $01, $00, $00
; Jump easing/arc table: per-step delay values for Sprite 0 fall/return animation. 24 entries indexed by fall step.
f_jump_easing_table .byte $11, $11, $11, $11, $11, $02, $01, $01 ; Jump easing/arc table: per-step delay values for fall/return animation. 24 entries indexed by fall step.
                    .byte $01, $01, $01, $10, $f4, $30, $40, $50
a_level_anim_config .byte $ff, $fa, $01, $00, $00, $00, $00, $28 ; x-ref: $53fa
                    .byte $11, $11, $11, $28, $11, $00
a_vic_irq_config_val1
                    .byte $21            ; x-ref: $ca8b, $cae5
a_vic_irq_config_val2
                    .byte $0d            ; x-ref: $ca91
a_vic_irq_config_val3
                    .byte $fa            ; x-ref: $ca97
f_vic_irq_offset_table
                    .byte $1d, $1d, $1c, $1c, $1b, $1b, $1a, $1a ; x-ref: $caab
                    .byte $19, $19, $18, $18, $17, $17, $16, $16
                    .byte $15, $02, $02, $14, $13, $13, $12, $12
                    .byte $11, $11, $10, $10, $0f, $0f, $0e, $0e
; Initial enemy respawn counts (6 entries): number of times each enemy slot can respawn. Loaded by init_enemy_counts.
f_enemy_respawn_counts
                    .byte $01            ; x-ref: $c694
a_enemy_wave_max_limit
                    .byte $05            ; x-ref: $c6a7
a_enemy_sprite_config
                    .byte $00            ; x-ref: $5eee, $c6e0
a_sprite_enable_mask
                    .byte $0b            ; x-ref: $5ef4
; Sprite state flags 2-entry table ($45EF): enable values passed to set_state_flag in init_sprite_system_body.
f_sprite_state_flags
                    .byte $01            ; x-ref: $5efa
a_sprite_priority_config
                    .byte $08            ; x-ref: $5f00
; Sprite-to-background priority mask ($45F1): 3 entries written to $D01B by init_sprite_system_body.
f_sprite_priority_mask
                    .byte $0b            ; x-ref: $5f06
a_sprite1_init_color
                    .byte $0a            ; x-ref: $c6be
f_sid_voice3_presets
                    .byte $80, $20, $20, $28 ; x-ref: $2f0a
; Enemy movement-enable flags (5 entries): non-zero = this enemy slot is allowed to move. From init_enemy_counts.
f_enemy_enable_flags
                    .byte $11, $aa, $00, $00, $80 ; x-ref: $5492
a_level_state_lo    .byte $80
a_level_state_hi    .byte $80
col_sprite1_enable  .byte $21
; Sprite 0 active/enable flags ($45FF, 14 entries): used by player_state_check to gate movement.
sprite0_active_flags
                    .byte $0d, $00, $00, $80, $80, $80, $21, $0c
                    .byte $00, $00, $80, $80, $80, $21, $0c, $00
                    .byte $00, $80, $80, $80, $41, $69, $00, $00
                    .byte $80, $80, $80, $21, $99, $00, $00, $00
                    .byte $11, $0a, $00
f_enemy_respawn_y_threshold
                    .byte $09            ; x-ref: $2fdf, $5f48
a_enemy_config_vars_1
                    .byte $09            ; x-ref: $5f4e
a_enemy_config_vars_2
                    .byte $09            ; x-ref: $5f54
a_enemy_config_vars_3
                    .byte $09            ; x-ref: $5f5a
a_enemy_config_vars_4
                    .byte $09            ; x-ref: $5f60
f_enemy_init_types  .byte $c8, $c8, $15, $01, $c8 ; x-ref: $2d02, $566f
a_sprite_ptr_backup_45EC
                    .byte $e2            ; x-ref: $55b7, $567a
a_sprite_system_init_val
                    .byte $b3            ; x-ref: $2e62
a_level_state_init_val_1
                    .byte $01            ; x-ref: $2da5
a_level_state_init_val_2
                    .byte $01, $05       ; x-ref: $2e8b
a_sprite_priority_init_val
                    .byte $00, $ff, $00  ; x-ref: $2d1f, $2e94
a_sprite_multicolor_init_val
                    .byte $ff            ; x-ref: $2d19, $5549
a_sprite_mc_reg0_init_val
                    .byte $f9            ; x-ref: $2d0d, $c4e0
a_sprite_mc_reg1_init_val
                    .byte $f1            ; x-ref: $2d13, $c4f2
f_enemy_wave_active_flags
                    .byte $00, $01, $00, $00, $00 ; x-ref: $2d52
a_level_config_reg_1
                    .byte $20            ; x-ref: $2ce4
a_level_config_reg_2
                    .byte $08            ; x-ref: $2cea
a_level_config_reg_3
                    .byte $00            ; x-ref: $2d6c, $2ee3
a_level_config_reg_4
                    .fill 14, $01        ; x-ref: $5350, $c19d
                    .fill 17, $07
