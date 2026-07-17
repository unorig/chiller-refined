;==============================================================================
; $2D00-$2F4F - ENEMY/Sprite INIT, MENU/DEATH FLOW, SID SETUP ENTRY
;==============================================================================
;
; Handles enemy-count initialization, enemy slot processing, sprite init,
; menu/fire/death flow, sprite system setup, X-MSB/SID voice retrigger, sprite
; colors, and SID setup entry.
;
; === init_enemy_counts === Copies f_45E7 enemy type table to f_CF76 (5 entries) then dispatches enemy slot initialisation.
s_init_enemy_counts ldx #$00             ; x-ref: $c6dd
b_init_counts_loop  lda f_enemy_init_types,x ; x-ref: $2d0b
                    sta f_level_enemy_type_seeds,x
                    inx 
                    cpx #$05
                    bne b_init_counts_loop
                    lda a_sprite_mc_reg0_init_val
                    sta $d025            ; Sprite Multi-Color Register 0
                    lda a_sprite_mc_reg1_init_val
                    sta $d026            ; Sprite Multi-Color Register 1
                    lda a_sprite_multicolor_init_val
                    sta $d01c            ; Sprites Multi-Color Mode Select
                    lda a_sprite_priority_init_val
                    sta $d01d            ; Sprites Expand 2x Horizontal (X)
                    nop 
                    nop 
                    nop 
                    jmp s_game_frame_update_dispatch
                    .byte $a0, $e4, $a2, $00, $98, $9d, $f8, $07
                    .byte $18, $69, $04, $a8, $bd, $ad, $45, $9d
                    .byte $27, $d0, $e8, $e0, $05, $d0, $ed, $ad
                    .byte $f4, $45, $4a, $4a, $8d, $1c, $d0, $60
b_enemy_loop_reset  ldx #$00             ; x-ref: $ca24
b_enemy_loop_body   lda f_level_enemy_type_seeds,x ; x-ref: $2ec9
                    beq b_enemy_skip_check
b_enemy_check_active
                    lda f_enemy_wave_active_flags,x ; x-ref: $2ec1
                    beq b_enemy_skip_active
                    jmp b_enemy_sprite_collision
b_enemy_skip_check  jmp b_enemy_check_sprite_enabled ; x-ref: $2d50
b_enemy_skip_active jmp b_enemy_next_slot ; x-ref: $2d55
b_enemy_sprite_collision
                    lda $d01f            ; x-ref: $2d57; Sprite to Background Collision Detect
                    sta b_sprite_bg_col_save
                    and #$02
                    cmp #$00
                    beq b_enemy_check_boss
                    lda a_level_config_reg_3
                    beq b_enemy_check_boss
                    jsr s_flicker_sprite1_blank
                    jmp b_enemy_check_boss
                    .byte $ea, $7b, $2d
b_sprite_bg_col_save
                    .byte $00            ; x-ref: $2d63
b_enemy_check_boss  lda a_sprite_process_flag ; x-ref: $2d6a, $2d6f, $2d74
                    cmp #$01
                    beq b_enemy_tick_object
                    jmp j_goto_boss_action
b_enemy_tick_object jsr game_object_tick ; x-ref: $2d80
                    jmp level_end_handler
                    .byte $ea, $ea
; Initializes a sprite animation sequence.
; Stores the sprite data pointer/index (passed in A) into $07F9, then resets
; the animation frame counter at $C544 to 0.
; Called before enabling sprite 1 and beginning a new animation cycle.
; 
; Inputs:  A = sprite frame data index (e.g. lookup table value + $E0)
; Outputs: None
; Side Effects: $07F9 updated with sprite data index; $C544 (anim counter) zeroed
s_init_sprite       sta sprite_ptr1      ; x-ref: $c7ec
                    lda #$00
                    sta a_sprite1_flicker_ctr
                    rts 
                    .byte $ea
b_menu_check_fire   lda $dc00            ; x-ref: $2dc7; CIA1: Data Port Register A
                    and #$10
                    bne b_menu_fire_not_pressed
                    beq b_menu_fire_pressed
; Post-death handler: reset player position, decrement lives, check game-over condition.
s_after_death       lda #$00             ; x-ref: $5db9
                    sta $d015            ; Sprite display Enable
                    lda a_level_state_init_val_1
                    jsr s_set_state_flag
                    lda #$ff
                    sta a_delay_loop_param_lo
                    sta a_delay_loop_param_hi
                    jsr s_delay_wait_loop
b_menu_fire_not_pressed
                    lda a_music_play_status_flag ; x-ref: $2d9c
                    cmp #$00
                    beq b_reset_to_menu_screen
                    jmp s_set_irq_and_mem
b_reset_to_menu_screen
                    jsr game_state_reset ; x-ref: $2dbb
                    lda zp_joystick_port2_state
                    cmp #$40
                    beq b_menu_check_fire
b_menu_fire_pressed lda $d020            ; x-ref: $2d9e; Border Color
                    and #$0f
                    beq b_menu_start_game
                    jsr s_check_and_update_high_score
b_menu_start_game   jmp s_reset_cutscene_girl ; x-ref: $2dce
s_store_horizontal_sprite_maybe_airborne
                    sta sprite_ptr0
                    lda a_C19C
                    bne b_store_airborne_sprite
                    lda a_fall_step_counter
                    bne b_store_airborne_sprite
                    rts
b_store_airborne_sprite
                    lda a_input_delta_x
                    bmi b_store_airborne_left
                    lda a_rope_slide_offset
                    bne b_store_airborne_sprite_done
b_store_airborne_left
                    lda a_rope_slide_timer
b_store_airborne_sprite_done
                    sta sprite_ptr0
                    rts
                    .fill 5, $ea
j_game_entry        jsr patch_stop_key_jmp_main ; x-ref: $0834
                    jmp s_load_level_state
s_health_then_double_jump
                    jsr health_bar_updates
                    jsr s_try_double_jump
                    rts
a_smc_raster_opcode1
                    .fill 26, $ea        ; SMC patch slot 1: written with $03 or $05 by start_level to gate the raster sync count in the CIA wait loop. ; x-ref: $2c6f, $2c81
a_smc_raster_opcode2
                    .fill 65, $ea        ; SMC patch slot 2: companion to smc_raster_opcode1; set to $04 or $05 to control how many raster ticks to wait. ; x-ref: $2c74, $2c86
; Initialises the sprite system state. Copies the sprite config byte from the
; data table (a_45ED) into state variable a_CF7C, clears the a_CF7D sprite-
; processing flag to 0, then jumps to the continuation at j_2E8B.
; 
; Inputs:  None (reads from static data tables at $45ED, $45EF, $45F1, $450E)
; Outputs: A = value of a_450E on return
; Side Effects: a_CF7C updated; a_CF7D cleared; $D01B sprite priority set;
;               a_2E81 patched via s_2CF3; s_60CA called for additional init
s_init_sprite_system
                    lda a_sprite_system_init_val ; x-ref: $2c0d
                    sta a_sprite_system_config_val
                    lda #$00
                    sta a_sprite_process_flag
                    lda a_sprite_system_config
                    jmp s_init_sprite_system_body
                    nop 
                    lda music_v1_ctrl
                    and #$04
                    beq b_init_sprite_done
                    lda #$ff
                    sta a_music_play_status_flag
a_smc_jump_opcode =*+$01
b_init_sprite_done  jmp f_music_data_6001 ; x-ref: $2e79
                    .byte $03, $4c, $00, $2e, $4c, $31, $ea, $ea
; === init_sprite_system_body === Loads sprite enable/state byte from level config table, calls set_state_flag, calls init_sid_music, sets sprite-to-background priority, returns sprite_system_config.
s_init_sprite_system_body
                    lda a_level_state_init_val_2 ; x-ref: $2e70
                    jsr s_set_state_flag
                    jsr s_init_sid_music
                    lda a_sprite_priority_init_val
                    sta $d01b            ; Sprite to Background Display Priority
                    lda a_sprite_system_config
                    rts 
                    nop 
                    nop 
                    lda $d001            ; Sprite 0 Y Pos
                    cmp #$b0
                    bne j_goto_boss_action
                    lda $d015            ; Sprite display Enable
                    inc $d001            ; Sprite 0 Y Pos
                    nop 
                    nop 
                    nop 
                    sta $d015            ; Sprite display Enable
j_goto_boss_action  jmp s_update_enemies_and_projectiles ; x-ref: $2d82, $2ea5
                    .byte $ea
b_enemy_check_sprite_enabled
                    lda f_enemy_x_msb_init_table,x ; x-ref: $2d5a
                    and $d015            ; Sprite display Enable
                    cmp #$00
                    beq b_enemy_next_slot
                    jmp b_enemy_check_active
b_enemy_next_slot   inx                  ; x-ref: $2d5d, $2ebf
                    cpx #$05
                    beq b_enemy_next_done
                    jmp b_enemy_loop_body
b_enemy_next_done   jmp s_increment_score ; x-ref: $2ec7
                    nop 
s_reset_enemy_ticker
                    lda #$00             ; x-ref: $2faf
                    sta a_0384
                    lda #$01
                    sta a_logic_tick_flag
                    rts 
                    .byte $a9, $00, $8d, $15, $d0, $4c, $94, $c6
b_enemy_state_init  lda a_level_config_reg_3 ; x-ref: $2cf0
                    nop 
                    nop 
                    nop 
                    jmp s_reset_cutscene_girl
                    lda #$60
                    sta j_C694
                    jsr s_init_hud_stats
                    lda #$ad
                    sta j_C694
                    lda #$1c
                    sta $d018            ; VIC Memory Control Register
                    rts 
                    .byte $ea
; Updates the Sprites X-MSB register ($D010) then retriggles SID Voice 3 with
; a preset frequency and triangle waveform from the table at f_45B3.
; Steps: (1) store A -> $D010; (2) gate off Voice 3; (3) copy 7-byte preset
; table into Voice 3 registers ($D40E-$D414), re-enabling the gate on X=4.
; 
; Inputs:  A = new value for $D010 (Sprites 0-7 X-MSB bitmask, pre-computed by caller)
; Outputs: None (A, X clobbered)
; Side Effects: $D010 updated; SID Voice 3 re-triggered with preset freq/envelope
s_set_xmsb_retrigger_v3
                    sta $d010            ; x-ref: $c816; Sprites 0-7 MSB of X coordinate
                    lda #$00
                    sta $d412            ; Voice 3: Control Register
                    ldx #$00
b_retrigger_freq_loop
                    lda f_sid_voice3_presets,x ; x-ref: $2f13
                    sta $d40e,x          ; Voice 3: Frequency Control - Low-Byte
                    inx 
                    cpx #$07
                    bne b_retrigger_freq_loop
                    rts 
                    sta enemy_sprite_ptrs,x
                    lda #$00
                    sta $ffff            ; IRQ
                    txa 
                    pha 
                    lda f_level_status_table_6d,x
                    sta ptr_lo
                    lda #$45
                    sta ptr_hi
                    ldy #$00
b_read_zp_loop      lda (ptr_lo),y       ; x-ref: $2f33
                    sta $ffff,y          ; IRQ
                    iny 
                    cpy #$07
                    bne b_read_zp_loop
                    pla 
                    tax 
                    rts 
s_set_enemy_colour  sta $d029,x          ; x-ref: $c6e3; Sprite 2 Color
                    lda f_enemy_anim_table_lo,x
                    sta ptr_lo
                    lda f_sprite_anim_table_hi,x
                    sta ptr_hi
                    ldy #$00
; Read byte from zero-page indirect pointer ($FB/$FC+Y) into sprite data table f_CF15,X.
s_read_zp_to_cf15   lda (ptr_lo),y       ; x-ref: $5712, $5716
                    sta f_sprite_anim_frame_pointers,x
                    rts 
; Thin wrapper that starts SID music initialisation.
; Delegates entirely to s_60CA, which silences all SID registers ($D400-$D41C),
; sets the music-data pointers ($B7/$B8, $B9/$BA -> $61F8) and installs the
; music-player IRQ handler at $0314/$0315 (pointing to the player at $60F5).
; 
; Inputs:  None
; Outputs: None
; Side Effects: SID registers cleared; CIA IRQ vector ($0314/$0315) set to SID player
