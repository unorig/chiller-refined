level_end_handler   lda music_section_ctr ; x-ref: $2d88, $5dac
                    beq level_end_handler
                    jsr disable_music
                    lda #$00
                    sta pending_health_restore_count
                    sta queued_damage_effect_count
                    jmp s_after_death
s_try_double_jump
                    lda a_double_jump_state
                    cmp #$02
                    beq r_double_jump_done
                    lda $dc00
                    and #$01
                    beq b_double_jump_up_pressed
                    lda a_double_jump_state
                    cmp #$03
                    bne r_double_jump_done
                    lda #$01
                    sta a_double_jump_state
                    rts
b_double_jump_up_pressed
                    lda a_double_jump_state
                    beq b_mark_jump_pressed
                    cmp #$01
                    beq b_do_double_jump
                    rts
b_mark_jump_pressed
                    lda #$03
                    sta a_double_jump_state
                    rts
b_do_double_jump
                    ldx #$02
                    stx a_double_jump_state
                    lda #$01
                    sta a_C19C
                    lsr a
                    sta a_C19B
                    stx a_var_jump_phase
                    stx a_var_jump_timer
r_double_jump_done
                    rts
s_airborne_fast_physics_then_double_jump
                    lda #$10
                    sta a_smc_physics_speed_limit
                    jmp s_try_double_jump
                    .fill 6, $00
; Handle title-screen state: wait for fire button, then call game_init_state and transition to gameplay.
disable_kernal_rom  lda cpu_io_port      ; x-ref: $5d92, $5e14
                    and #$fe
                    sta cpu_io_port
                    lda cpu_io_port
                    and #$01
                    bne disable_kernal_rom
                    rts 
                    .byte $00, $00
; Handle high-score display state: show score table, wait for input, then restart game.
high_score_handler  lda level_config_ptr_lo,x ; x-ref: $769f, $c662
                    sta ptr_level_map
                    lda level_config_ptr_hi,x
                    sta zpa_12
                    nop 
                    nop 
                    jsr game_substate_main
                    nop 
                    nop 
                    ldx ptr_level_map
                    ldy zpa_12
                    jsr bankswitch_block_copy
                    lda ptr_level_map
                    clc 
                    adc #$06
                    tax 
                    ldy zpa_12
                    jsr s_copy_data
                    ldy #$0c
                    lda (ptr_level_map),y
                    sta a_player_x_sprite_target
                    iny 
                    tya 
                    pha 
                    lda (ptr_level_map),y
                    sta a_player_y_sprite_target
                    jsr update_player_sprite_pos
                    pla 
                    tay 
                    iny 
                    lda (ptr_level_map),y
                    sta $d022            ; Background Color 1, Multi-Color Register 0
                    iny 
                    lda (ptr_level_map),y
                    sta $d023            ; Background Color 2, Multi-Color Register 1
                    iny 
                    lda (ptr_level_map),y
                    jsr set_sprite0_x
                    iny 
                    lda (ptr_level_map),y
                    jsr set_sprite0_y
                    iny 
                    lda (ptr_level_map),y
                    jsr set_sprite0_xmsb_and_save
                    iny 
                    lda (ptr_level_map),y
                    sta sprite_ptr0
                    iny 
                    lda (ptr_level_map),y
                    sta $d002            ; Sprite 1 X Pos
                    iny 
                    lda (ptr_level_map),y
                    sta $d003            ; Sprite 1 Y Pos
                    iny 
                    lda a_player_xmsb
                    and #$fd
                    ora (ptr_level_map),y
                    sta $d010            ; Sprites 0-7 MSB of X coordinate
                    iny 
                    lda (ptr_level_map),y
                    sta sprite_ptr1
                    iny 
                    lda (ptr_level_map),y
                    sta a_level_status_param_68
                    iny 
                    lda (ptr_level_map),y
                    sta a_level_status_param_69
                    iny 
                    lda (ptr_level_map),y
                    sta a_level_status_param_6a
                    iny 
                    lda (ptr_level_map),y
                    sta a_level_status_param_6b
                    iny 
                    lda (ptr_level_map),y
                    sta a_level_status_param_6c
                    iny 
                    lda (ptr_level_map),y
                    sta f_sprite_anim_loop_frames
                    iny 
                    lda (ptr_level_map),y
                    sta a_sprite_anim_param_3e
                    iny 
                    lda (ptr_level_map),y
                    sta a_sprite_anim_param_3f
                    iny 
                    lda (ptr_level_map),y
                    sta a_sprite_anim_param_40
                    iny 
                    lda (ptr_level_map),y
                    sta a_sprite_anim_param_41
                    iny 
                    lda (ptr_level_map),y
                    sta f_sprite_anim_end_frames
                    iny 
                    lda (ptr_level_map),y
                    sta a_sprite_anim_param_39
                    iny 
                    lda (ptr_level_map),y
                    sta a_sprite_anim_param_3a
                    iny 
                    lda (ptr_level_map),y
                    sta a_sprite_anim_param_3b
                    iny 
                    lda (ptr_level_map),y
                    sta a_sprite_anim_param_3c
                    iny 
                    lda (ptr_level_map),y
                    sta a_enemy_sprite_config
                    iny 
                    lda (ptr_level_map),y
                    sta a_sprite_enable_mask
                    iny 
                    lda (ptr_level_map),y
                    sta f_sprite_state_flags
                    iny 
                    lda (ptr_level_map),y
                    sta a_sprite_priority_config
                    iny 
                    lda (ptr_level_map),y
                    sta f_sprite_priority_mask
                    iny 
                    lda (ptr_level_map),y
                    sta f_enemy_anim_max_ticks
                    iny 
                    lda (ptr_level_map),y
                    sta a_enemy_anim_param_9d
                    iny 
                    lda (ptr_level_map),y
                    sta a_enemy_anim_param_9e
                    iny 
                    lda (ptr_level_map),y
                    sta a_enemy_anim_param_9f
                    iny 
                    lda (ptr_level_map),y
                    sta a_enemy_anim_param_a0
                    iny 
                    lda (ptr_level_map),y
                    sta f_enemy_speed_configs
                    iny 
                    lda (ptr_level_map),y
                    sta a_sfx_duration
                    iny 
                    lda (ptr_level_map),y
                    sta a_sfx_envelope
                    iny 
                    lda (ptr_level_map),y
                    sta a_sfx_v3_freq_lo
                    iny 
                    lda (ptr_level_map),y
                    sta a_sfx_v3_freq_hi
                    iny 
                    lda (ptr_level_map),y
                    sta f_enemy_respawn_y_threshold
                    iny 
                    lda (ptr_level_map),y
                    sta a_enemy_config_vars_1
                    iny 
                    lda (ptr_level_map),y
                    sta a_enemy_config_vars_2
                    iny 
                    lda (ptr_level_map),y
                    sta a_enemy_config_vars_3
                    iny 
                    lda (ptr_level_map),y
                    sta a_enemy_config_vars_4
                    iny 
                    lda (ptr_level_map),y
                    sta f_enemy_anim_table_lo
                    iny 
                    lda (ptr_level_map),y
                    sta a_level_anim_temp_1
                    iny 
                    lda (ptr_level_map),y
                    sta a_level_anim_temp_2
                    iny 
                    lda (ptr_level_map),y
                    sta a_level_anim_temp_3
                    iny 
                    lda (ptr_level_map),y
                    sta a_level_anim_temp_4
                    iny 
                    lda (ptr_level_map),y
                    sta f_sprite_anim_table_hi
                    iny 
                    lda (ptr_level_map),y
                    sta a_sprite_anim_table_25
                    iny 
                    lda (ptr_level_map),y
                    sta a_sprite_anim_table_26
                    iny 
                    lda (ptr_level_map),y
                    sta a_sprite_anim_table_27
                    iny 
                    lda (ptr_level_map),y
                    sta a_sprite_anim_table_28
                    ldx #$00
b_score_refresh_done
                    iny                  ; x-ref: $5faa
                    lda (ptr_level_map),y
                    sta a_enemy_xpos,x   ; Enemy initial X positions (5 entries). Set from level config, used by init_sprite_movement.
                    inx 
                    cpx #$19
                    bne b_score_refresh_done
                    lda #$0e
                    sta a_music_note_duration
                    jsr read_map_and_draw_char
                    lda #$0a
                    sta a_music_note_duration
                    jsr read_map_and_draw_char
                    jmp s_init_level_crosses
                    .byte $ea
; Read from level map pointer ($11+Y indirect) into $FB/$FC,
; write screen character ($57='W') across 5 columns of current screen row.
read_map_and_draw_char
                    ldx #$00             ; x-ref: $5fb1, $5fb9
b_map_char_read_done
                    iny                  ; x-ref: $5fe4
                    lda (ptr_level_map),y
                    sta ptr_lo
                    iny 
                    lda (ptr_level_map),y
                    sta ptr_hi
                    tya 
                    pha 
                    ldy #$00
                    lda #$57
                    sta (ptr_lo),y
                    lda ptr_hi
                    clc 
                    adc #$d4
                    sta ptr_hi
