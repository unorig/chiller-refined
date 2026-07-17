;==============================================================================
; $5500-$57FF - INPUT SCAN, SPRITE SETUP, ENEMY SEEDS, HUD DRAW HELPERS
;==============================================================================
;
; Covers input/button scan fragments, sprite pointer selection, path-buffer
; initialization, enemy direction/count reset, HUD score/lives drawing, and
; level animation table staging.
;
f_enemy_speed_configs
                    .byte $04            ; x-ref: $569c, $56a0, $56a9, $56b8, $5f2a, ...
a_sfx_duration      .byte $06            ; x-ref: $5f30
a_sfx_envelope      .byte $04            ; x-ref: $5f36
a_sfx_v3_freq_lo    .byte $04            ; x-ref: $5f3c
a_sfx_v3_freq_hi    .byte $04            ; x-ref: $5f42
sfx_sweep_continue  lda #$00             ; x-ref: $549c
                    sta a_sfx_temp_lo
                    lda #$08
                    sta a_sfx_temp_hi
                    jmp b_check_movement_delay
                    .byte $ea, $ea
a_sfx_temp_lo       .byte $00            ; x-ref: $54ec, $54fc, $5502, $5518, $551e
a_sfx_temp_hi       .byte $08, $ea       ; x-ref: $54f1, $550a, $550d, $5526, $5529
fall_continue_down  lda a_sfx_temp_lo    ; x-ref: $53bb
                    sec 
                    sbc #$05
                    sta a_sfx_temp_lo
                    sta $d40e            ; Voice 3: Frequency Control - Low-Byte
                    bcc b_sound_descend_done
                    inc a_sfx_temp_hi
b_sound_descend_done
                    lda a_sfx_temp_hi    ; x-ref: $5508
                    sta $d40f            ; Voice 3: Frequency Control - High-Byte
                    rts 
                    .byte $ea, $ea, $ea, $ea
fall_continue_up    lda a_sfx_temp_lo    ; x-ref: $53de
                    clc 
                    adc #$05
                    sta a_sfx_temp_lo
                    sta $d40e            ; Voice 3: Frequency Control - Low-Byte
                    bcs b_sound_ascend_done
b_sound_ascend_done dec a_sfx_temp_hi    ; x-ref: $5524
                    lda a_sfx_temp_hi
                    sta $d40f            ; Voice 3: Frequency Control - High-Byte
                    rts 
                    nop 
                    nop 
                    lda a_player_x       ; Player sprite 0 saved X position. Written by set_sprite0_x, read by restore_sprite0_position.
                    sta $d000            ; Sprite 0 X Pos
                    lda a_player_y       ; Player sprite 0 saved Y position. Written by set_sprite0_y.
                    sta $d001            ; Sprite 0 Y Pos
                    lda #$03
                    sta $d015            ; Sprite display Enable
                    lda a_player_xmsb
                    sta $d010            ; Sprites 0-7 MSB of X coordinate
                    lda a_sprite_multicolor_init_val
                    and #$01
                    sta $d01c            ; Sprites Multi-Color Mode Select
                    lda a_xmsb_scroll_config
                    sta $d027            ; Sprite 0 Color
                    nop 
b_input_scan_loop   .byte $23            ; x-ref: $559d; Invalid or partial instruction
                    cpy #$c1
                    ldx #$00
                    lda a_input_delta_x
                    cmp #$01
                    bne b_input_delta_x_done
                    lda #$03
                    jsr s_update_and_set_player_sprite_ptr
b_input_delta_x_done
                    lda a_input_delta_x  ; x-ref: $5562
                    cmp #$ff
                    bne b_input_delta_y_check
                    lda #$02
                    jsr s_update_and_set_player_sprite_ptr
b_input_delta_y_check
                    lda a_input_delta_y  ; x-ref: $556e
                    cmp #$01
                    bne b_input_delta_y_done
                    lda #$01
                    jsr s_update_and_set_player_sprite_ptr
b_input_delta_y_done
                    lda a_input_delta_y  ; x-ref: $557a
                    cmp #$ff
                    bne b_input_fire_check
                    nop
                    nop
                    nop
                    lda #$00
                    jsr s_update_and_set_player_sprite_ptr
b_input_fire_check  lda a_input_fire_button ; x-ref: $5586
                    bne b_input_fire_pressed
                    lda #$20
                    sta a_delay_loop_param_lo
                    sta a_delay_loop_param_hi
                    jsr s_delay_wait_loop
                    jmp b_input_scan_loop
b_input_fire_pressed
                    lda $d000            ; x-ref: $5590; Sprite 0 X Pos
                    sta a_player_x       ; Player sprite 0 saved X position. Written by set_sprite0_x, read by restore_sprite0_position.
                    lda $d001            ; Sprite 0 Y Pos
                    sta a_player_y       ; Player sprite 0 saved Y position. Written by set_sprite0_y.
                    lda $d010            ; Sprites 0-7 MSB of X coordinate
                    and #$01
                    sta a_player_xmsb
                    lda sprite_ptr0
                    sta a_sprite_ptr_backup_45EC
                    sta a_watermark_sprite0_backup
                    lda #$00
                    sta $d015            ; Sprite display Enable
                    lda #$15
                    sta $d018            ; VIC Memory Control Register
                    lda #$00
                    sta zp_keyboard_buffer_len
                    rts 
                    .byte $ea, $ea
a_sprite_index_temp .byte $02            ; x-ref: $55cf, $55d5
s_update_and_set_player_sprite_ptr
                    sta a_sprite_index_temp ; x-ref: $5566, $5572, $557e, $558a
                    jsr s_update_sprite_coordinates
                    lda a_sprite_index_temp
                    asl a
                    clc 
                    adc #$d8
                    sta sprite_ptr0
                    rts 
                    lda #$93
                    jsr KERNAL_CHROUT    ; $FFD2 - output character
                    ldx #$88
                    ldy #$c1
                    jsr s_copy_data
                    ldx #$8e
                    ldy #$c1
                    jsr s_copy_data
                    ldx #$94
                    ldy #$c1
                    jsr s_copy_data
                    rts 
                    nop 
                    nop 
                    ldx #$00
b_clear_path_buf_loop
                    lda #$ff             ; x-ref: $560a
                    sta a_intro_path_step_ctr,x
                    inx 
                    cpx #$40
                    bne b_clear_path_buf_loop
                    lda #$0e
                    sta sprite_ptr0
                    sta sprite_ptr1
                    lda #$2c
                    sta $d000            ; Sprite 0 X Pos
                    sta $d002            ; Sprite 1 X Pos
                    lda #$70
                    sta $d001            ; Sprite 0 Y Pos
                    lda #$9a
                    sta $d003            ; Sprite 1 Y Pos
                    lda #$0f
                    sta $d027            ; Sprite 0 Color
                    sta $d028            ; Sprite 1 Color
                    lda #$0b
                    sta $d015            ; Sprite display Enable
                    sta $d010            ; Sprites 0-7 MSB of X coordinate
                    lda #$03
                    sta $d01b            ; Sprite to Background Display Priority
                    ldx #$00
b_sprite3_drop_loop lda #$1e             ; x-ref: $564e
                    sta a_delay_loop_param_lo
                    sta a_delay_loop_param_hi
                    jsr s_delay_wait_loop
                    inc $d007            ; Sprite 3 Y Pos
                    inx 
                    cpx #$15
                    bne b_sprite3_drop_loop
b_init_collision_limits =*+$01
                    adc #$ad
                    .byte $ff            ; Invalid or partial instruction
                    eor zp_decrypter_key
                    php 
                    lda #$00
                    sta a_col_active_up  ; UP collision active flag: non-zero = player colliding with solid tile above.
                    jmp s_set_down_collision_and_load_cf
                    jmp s_load_cf_and_set_y_offset_30
                    .byte $60
; Store sprite slot index A into a_C0E3 scratch, TAX, mark slot active (STA f_CF33,X=1), return slot in A.
update_vertical_enemies
                    sta a_C0E3           ; x-ref: $c0e4
                    tax 
                    lda #$01
                    sta f_sprite_anim_ticks,x
                    txa 
                    rts 
s_reset_enemy_counts_vars
                    ldx #$00             ; x-ref: $56bc
b_reset_enemy_counts_vars_loop
                    lda f_enemy_init_types,x ; x-ref: $5678
                    sta f_level_enemy_type_seeds,x
                    inx 
                    cpx #$05
                    bne b_reset_enemy_counts_vars_loop
                    lda a_sprite_ptr_backup_45EC
                    sta sprite_ptr0
                    rts 
; Enemy movement direction sequence tables ($575E-$5762, 5 entries): per-slot pointer (lo) into enemy movement direction sequence.
enemy_dir_table     ldx #$00             ; x-ref: $2c2f
b_enemy_dir_loop    lda f_custom_font_data_34E8,x ; x-ref: $56af
                    cmp f_enemy_anim_max_ticks,x
                    beq b_enemy_dir_next_slot
                    clc 
                    lda f_enemy_anim_max_ticks,x
                    lsr a
                    sta f_enemy_anim_max_ticks,x
                    bcc b_enemy_dir_half_speed
                    lda #$01
                    sta f_enemy_anim_max_ticks,x
                    bcs b_enemy_dir_next_slot
b_enemy_dir_half_speed
                    lda f_enemy_speed_configs,x ; x-ref: $5693
                    lsr a
                    sta f_enemy_speed_configs,x
                    cmp #$01
                    bne b_enemy_dir_next_slot
                    lda #$02
                    sta f_enemy_speed_configs,x
b_enemy_dir_next_slot
                    inx                  ; x-ref: $5689, $569a, $56a5
                    cpx #$05
                    bne b_enemy_dir_loop
                    rts 
; Set sprite slot direction: store direction A into f_CF9C,X; load next expected direction from f_4548,X into f_54E5,X.
set_sprite_direction
                    sta f_enemy_anim_max_ticks,x ; x-ref: $2c05
                    lda f_enemy_init_seed_table,x
                    sta f_enemy_speed_configs,x
                    rts 
s_reset_enemy_counts_vars_wrapper
                    jsr s_reset_enemy_counts_vars ; x-ref: $caf5
                    rts 
                    brk 
                    sty zpa_50
                    sty cpu_io_dir
                    .byte $04
                    .fill 8, $60
                    plp 
                    lsr zp_decrypter_key
                    eor #$28
                    cld 
                    plp 
                    eor (zp_decrypter_key,x)
                    .byte $44, $28, $04, $a2, $d9
b_zp_modify_loop    lda cpu_io_dir,x     ; x-ref: $56e5
b_zp_modify_step    ora #$80             ; x-ref: $574a
                    sta cpu_io_dir,x
                    inx 
                    cpx #$f2
                    bne b_zp_modify_loop
                    rts 
                    ldx #$00
                    lda f_enemy_init_coordinate_index,x
                    tax 
                    lda a_enemy_xpos,x   ; Enemy initial X positions (5 entries). Set from level config, used by init_sprite_movement.
                    sta $d000            ; Sprite 0 X Pos
                    lda a_enemy_ypos,x
                    sta $d001            ; Sprite 0 Y Pos
                    lda #$00
                    sta $d010            ; Sprites 0-7 MSB of X coordinate
                    lda a_enemy_xmsb,x
                    beq r_vertical_limit_rts
                    lda #$01
                    sta $d010            ; Sprites 0-7 MSB of X coordinate
r_vertical_limit_rts
                    rts                  ; x-ref: $5702
s_set_down_collision_and_load_cf
                    sta a_col_active_down ; x-ref: $565b; DOWN collision active flag.
                    lda #$2c
                    sta a_smc_player_y_bound
                    jsr s_read_zp_to_cf15
                    rts 
s_load_cf_and_set_y_offset_30
                    jsr s_read_zp_to_cf15 ; x-ref: $565e
                    lda #$30
                    sta a_smc_player_y_bound
                    rts 
                    ldx #$00
                    lda #$00
b_draw_score_next   sta f_zero_init_buffer,x ; x-ref: $5730
                    sta f_zero_init_buf_3100,x
                    sta f_zero_init_buf_3200,x
                    sta f_zero_init_buf_3300,x
                    inx 
                    bne b_draw_score_next
                    rts 
draw_score_display  nop 
                    nop 
                    nop 
                    ldx #$00
b_draw_lives_next   lda f_hud_char_screencodes_574A,x ; x-ref: $5747
                    lda sprite_ptr_config,x
                    lda a_D805
                    lda f_DBCA,x
                    inx 
                    cpx #$13
                    bne b_draw_lives_next
                    rts 
f_hud_char_screencodes_574A
                    bcc b_zp_modify_step ; x-ref: $5738
                    sta zpa_93
                    .byte $93, $a0, $83, $94, $92, $8c, $a0, $86
                    .byte $8f, $92, $a0, $8d, $85, $8e, $95, $ea
f_enemy_anim_table_lo
                    .byte $1e            ; x-ref: $2f3b, $5f66, $cc3c, $cc6f
a_level_anim_temp_1 .byte $20            ; x-ref: $5f6c
a_level_anim_temp_2 .byte $1e            ; x-ref: $5f72
a_level_anim_temp_3 .byte $1e            ; x-ref: $5f78
a_level_anim_temp_4 .byte $22, $ea, $ea, $ea, $ea, $ea ; x-ref: $5f7e
j_jmp_to_7280       jmp s_check_level_end_transition ; x-ref: $58e3
; Patch game state machine: writes immediate operand bytes to STA instructions
; throughout the game logic area ($5844-$57B3), selecting between two sets of level
; parameters (a_5A08 flag selects which parameter set).
game_state_smc_patch
                    jsr wait_for_raster  ; x-ref: $5d65, $7288
                    nop 
                    nop 
                    jsr setup_sprite_positions
                    lda $d015            ; Sprite display Enable
                    ora #$03
                    sta $d015            ; Sprite display Enable
                    lda var_character_select
                    beq change_to_girl
                    jmp change_to_boy
change_to_girl      lda #$01             ; x-ref: $577e
                    sta var_character_select
                    lda #$e8
                    sta a_level_config_index
                    lda #$ec
                    sta a_player_action_temp
                    lda #$f0
                    sta a_rope_slide_speed
                    lda #$f1
                    sta a_key_scanner_temp_1
                    lda #$f6
                    sta a_key_scanner_temp_2
                    lda #$f2
                    sta a_joystick_x_delta
                    lda #$f6
                    sta a_joystick_y_delta
                    lda #$f1
                    sta a_rope_slide_offset
                    lda #$f0
                    sta a_rope_slide_timer
                    lda #$0a
                    sta $d020            ; Border Color
                    jmp sub_MainGameEntry
change_to_boy       lda #$00             ; x-ref: $5780
                    sta var_character_select
                    lda #$d8
                    sta a_level_config_index
                    lda #$dc
                    sta a_player_action_temp
                    lda #$e0
                    sta a_rope_slide_speed
                    lda #$e1
                    sta a_key_scanner_temp_1
                    lda #$e6
                    sta a_key_scanner_temp_2
                    lda #$e2
                    sta a_joystick_x_delta
                    lda #$e6
                    sta a_joystick_y_delta
                    lda #$e1
                    sta a_rope_slide_offset
                    lda #$e0
                    sta a_rope_slide_timer
                    lda #$06
                    sta $d020            ; Border Color
                    jmp sub_MainGameEntry
                    .fill 9, $ea
