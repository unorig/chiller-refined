;==============================================================================
; $2C00-$2DFF - SCORE, IRQ COPY, LEVEL START, STATE FLAGS
;==============================================================================
;
; Initializes enemy directions, increments/resets score, starts levels, copies
; IRQ handler bytes, sets IRQ/memory state, and loads level-related state.
;
; === init_enemy_dirs === Copies f_45E7 direction table (5 entries) to f_CF76 (respawn counts), then tail-calls j_2C00 to copy display seed and reset game state.
s_init_enemy_dirs   ldx #$00             ; x-ref: $2cb1
b_init_dirs_loop    lda f_enemy_init_dirs,x ; x-ref: $2c0b
                    jsr set_sprite_direction
                    inx 
                    cpx #$05
                    bne b_init_dirs_loop
                    jsr s_init_sprite_system
                    rts 
                    .fill 8, $ea
; === increment_score === Increments 4-digit reverse-video score array $CFA6-$CFA9. Cascades carry across digits $B0-$B9 on overflow.
s_increment_score   ldx #$03             ; x-ref: $2ecc
b_score_inc_loop    inc f_hud_score_digit_1,x ; x-ref: $2c2d
                    lda f_hud_score_digit_1,x
                    cmp #$ba
                    bne b_score_inc_done
                    lda #$b0
                    sta f_hud_score_digit_1,x
                    dex 
                    cpx #$00
                    bne b_score_inc_loop
b_score_inc_done    jsr enemy_dir_table  ; x-ref: $2c23
                    jmp s_start_level
                    lda #$60
                    sta a_smc_vector_projectile
                    jsr s_reset_sprite_frame_and_irq
                    lda #$4c
                    sta a_smc_vector_projectile
                    rts 
                    .byte $ea
; === start_level === Increments level_round_ctr, patches SMC raster opcode slots ($2E07/$2E21), clears level_pass_score, copies IRQ table, jumps to level_init_loop.
s_start_level       lda #$01             ; x-ref: $2c32
                    sta $d015            ; Sprite display Enable
                    ldx #$00
b_start_level_irq_copy
                    lda irq_handler_addrs,x ; x-ref: $2c66
                    sta f_0345,x
                    lda f_D9EF,x
                    sta f_034E,x
                    lda f_vic_irq_config_val_cfa1,x
                    sta irq_handler_addrs,x
                    lda a_D81A
                    sta f_D9EF,x
                    inx 
                    cpx #$09
                    bne b_start_level_irq_copy
                    lda #$00
                    sta a_level_pass_score
                    lda #$03
                    sta a_smc_raster_opcode1 ; SMC patch slot 1: written with $03 or $05 by start_level to gate the raster sync count in the CIA wait loop.
                    lda #$04
                    sta a_smc_raster_opcode2 ; SMC patch slot 2: companion to smc_raster_opcode1; set to $04 or $05 to control how many raster ticks to wait.
                    sta raster_sync_ctr
b_raster_sync_wait  lda raster_sync_ctr  ; x-ref: $2c7d
                    cmp #$06
                    bne b_raster_sync_wait
                    lda #$05
                    sta a_smc_raster_opcode1 ; SMC patch slot 1: written with $03 or $05 by start_level to gate the raster sync count in the CIA wait loop.
                    lda #$07
                    sta a_smc_raster_opcode2 ; SMC patch slot 2: companion to smc_raster_opcode1; set to $04 or $05 to control how many raster ticks to wait.
                    lda #$00
                    sta a_level_status_flag
                    ldx #$00
b_restore_irq_copy  lda f_0345,x         ; x-ref: $2c9f
                    sta irq_handler_addrs,x
                    lda f_034E,x
                    sta f_D9EF,x
                    inx 
                    cpx #$09
                    bne b_restore_irq_copy
                    jmp j_C694
; Initializes the three reverse-video score digit bytes at $CFA7-$CFA9 to the
; starting value "001" (in C64 reverse-video screen-code encoding: $B0='0',
; $B1='1'), then tail-calls j_2C00 to copy the display seed table (f_4542) into
; the display buffer and reset related game-state variables.
; 
; Inputs:  None
; Outputs: $CFA7=$B0, $CFA8=$B0, $CFA9=$B1 (counter reset to 001)
; Side Effects: Tail-calls j_2C00 which fills display buffer and inits $CF7C/$CF7D
s_reset_score_display
                    lda #$b0             ; x-ref: $5900
                    sta a_hud_score_digit_2
                    sta a_hud_score_digit_3
                    lda #$b1
                    sta a_hud_score_digit_4
                    jmp s_init_enemy_dirs
; === set_irq_and_mem === SEI, sets IRQ vector to $EA31, CLI, clears game_mode_flag and other state, sets VIC colours, outputs CLEAR char, jumps to set_screen_control.
s_set_irq_and_mem   sei                  ; x-ref: $2dbd, $ca5e
                    lda #$31
                    sta irq_vector_lo    ; IRQ
                    lda #$ea
                    sta irq_vector_hi    ; IRQ
                    cli 
                    lda #$00
                    sta a_music_play_status_flag
                    lda #$05
                    sta $d020            ; Border Color
                    lda #$0f
                    sta $d021            ; Background Color 0
                    lda #$15
                    sta $d018            ; VIC Memory Control Register
                    lda #$93
                    jsr KERNAL_CHROUT    ; $FFD2 - output character
                    lda #$00
                    sta zp_keyboard_buffer_len
                    sta $ffff            ; IRQ
                    jmp s_set_screen_control
                    .byte $ea
s_load_level_state  lda a_level_config_reg_1 ; x-ref: $2dfd
                    sta a_smc_physics_speed_limit
                    lda a_level_config_reg_2
                    sta a_smc_game_tick_limit
                    jmp b_enemy_state_init
; Stores the value in A to the state flag variable (a_2E81).
; If the flag is non-zero, clears the hardware IRQ vector high byte ($FFFF = $00),
; redirecting the IRQ handler to page $00 (KERNAL ROM banked out; $FFFF is RAM).
; 
; Inputs:  A = state flag value (callers load from a_45EE or a_45EF)
; Outputs: a_2E81 = updated with new flag value
; Side Effects: If A != 0, writes $00 to $FFFF (hardware IRQ hi-byte vector)
s_set_state_flag    sta a_smc_jump_opcode ; x-ref: $2da8, $2e8e
                    beq r_enemy_skip_rts
                    lda #$00
                    sta $ffff            ; IRQ
r_enemy_skip_rts    rts                  ; x-ref: $2cf6
                    .byte $ea, $ea
