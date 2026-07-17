;==============================================================================
; $772B-$7BFF - HIGH SCORE HANDLING AND TITLE TEXT BLOCK
;==============================================================================
;
; Contains high-score scratch bytes, high-score compare/update routines,
; background color driving, and the screen-code title text. Ends by including
; the external HUD/title layout data block.
;
a_hiscore_temp_1    .byte $ff            ; x-ref: $7727, $772e, $77af; Invalid or partial instruction
a_hiscore_temp_2    .byte $ff            ; x-ref: $7731, $7738, $77ac; Invalid or partial instruction
a_hiscore_temp_3    asl a                ; x-ref: $773b, $7742, $77a7
r_game_reset_check_rts
                    rts                  ; x-ref: $772a, $7734, $773e
; Reset all game state variables to zero, call game_init_state,
; install default IRQ via sid_silence_reset.
game_state_reset    lda $d020            ; x-ref: $2dc0; Border Color
                    and #$0f
                    beq s_drive_bg_color_toggle
                    dec a_hiscore_temp_1
                    bne r_game_reset_check_rts
                    lda #$ff
                    sta a_hiscore_temp_1
                    dec a_hiscore_temp_2
                    bne r_game_reset_check_rts
                    lda #$ff
                    sta a_hiscore_temp_2
                    dec a_hiscore_temp_3
                    bne r_game_reset_check_rts
                    lda #$0a
                    sta a_hiscore_temp_3
                    jsr s_check_and_update_high_score
                    ldx #$bf
                    ldy #$5b
                    jsr s_copy_data
                    lda #$00
                    sta $d015            ; Sprite display Enable
                    sta $d022            ; Background Color 1, Multi-Color Register 0
                    sta $d023            ; Background Color 2, Multi-Color Register 1
                    sta $d020            ; Border Color
                    sta $d021            ; Background Color 0
                    ldx #$00
                    ldy #$7c
                    jsr enemy_slot_spawn
                    jsr sid_setup
                    rts 
                    .byte $00, $00
s_drive_bg_color_toggle
                    jsr bg_color_toggle  ; x-ref: $7725
                    rts 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
; Main game loop entry point (game_loop_tick target): install SID music via init_sid_music,
; begin main game loop calling main_game_loop_body each IRQ.
s_check_and_update_high_score
                    lda hiscore_screen_ref,x ; x-ref: $2dd0, $7745, $7790
                    cmp f_8422,x
                    bmi b_hiscore_check_done
                    beq b_hiscore_check_next
                    jmp s_update_hiscore_table
b_hiscore_check_next
                    inx                  ; x-ref: $7788
                    cpx #$06
                    bne s_check_and_update_high_score
b_hiscore_check_done
                    jmp s_hiscore_compare_done ; x-ref: $7786, $77a2
s_update_hiscore_table
                    ldx #$00             ; x-ref: $778a
b_hiscore_update_loop
                    lda hiscore_screen_ref,x ; x-ref: $77a0
                    sta f_8422,x
                    inx 
                    cpx #$06
                    bne b_hiscore_update_loop
                    jmp b_hiscore_check_done
s_hiscore_compare_done
                    lda #$0a             ; x-ref: $7792
                    sta a_hiscore_temp_3
                    lda #$ff
                    sta a_hiscore_temp_2
                    sta a_hiscore_temp_1
                    rts 
                    .fill 13, $00
                    .encode 
                    .enc "screen"
; Reverse screen code text string: 'PROGRAMMED BY DAVID AND RICHARD DARLING.' used for the top row of the title screen.
f_title_top_row_text
                    .text $90, $92, $8f, $87, $92, $81, $8d, $8d, $85, $84, $a0, $82, $99, $a0, $84, $81, $96, $89, $84, $a0, $81, $8e, $84, $a0, $92, $89, $83, $88, $81, $92, $84, $a0, $84, $81, $92, $8c, $89, $8e, $87, $ae ; x-ref: $72ef
                    .endencode 
                    .fill 24, $00
