;==============================================================================
; $7C00-$BFFF - HUD/TEXT ASSETS, SCROLL TICK, GAME LOOP, LEVEL GRAPHICS
;==============================================================================
;
; Includes title/instruction assets, player coordinate setters, magic-cross
; detection, level scroll tick, the game_loop_tick IRQ body, and the large
; external level graphics/map/charset assets.
;
.binary "../../chiller_refined_asset_7800_7bff_title_text.bin"
                    .encode 
                    .enc "none"
; Main game intro/instructions text ($7C00-$7CBF):
; 'WELCOME TO THE GAME OF CHILLER. RESCUE YOUR GIRLFRIEND FROM THE HAUNTED HOUSE
; AND RETURN TO YOUR CAR. USE KEYS Z,C,? AND SHIFT, OR A JOYSTICK IN PORT TWO.'
game_intro_text     .text $0f, $04, $12, $05, "WELCOME TO THE GAME OF CHILLER.", $0d, $1f, $12, $11, $11
                    .text "RESCUE YOUR GIRLFRIEND FROM THE HAUNTED "
                    .text $11, "     HOUSE AND RETURN TO YOUR CAR.", $0d, $12, $9e, $11, $08
                    .text "    USE THE KEYS Z, C, ? AND SHIFT,", $0d, $12, $11, "  "
                    .text "     OR A JOYSTICK IN PORT TWO.", $01
                    .endencode 
.binary "../../chiller_refined_asset_7cc0_7ef0_sprite_position_data.bin"
; Store A into a_7EF0, then update bit 0 of $D010 (Sprite 0 X MSB) to match A.
set_sprite0_xmsb    sta a_7EF0           ; x-ref: $7f31
                    lda $d010            ; Sprites 0-7 MSB of X coordinate
                    and #$fe
                    ora a_7EF0
                    sta $d010            ; Sprites 0-7 MSB of X coordinate
                    rts 
magic_cross_counter_check
                    dec var_magic_crosses ; x-ref: $5b86
                    bne b_cross_keep_moving
                    nop
b_7F06              nop                  ; x-ref: $7f03
                    nop 
                    nop 
                    lda #$03
                    sta $d015            ; Sprite display Enable
                    ldy #$73
                    lda (ptr_level_map),y
                    tax 
                    cpx #$12
                    bne b_7F19
                    ldx #$fe
b_7F19              jmp setup_level_sprites ; x-ref: $7f15
b_cross_keep_moving
                    jmp b_player_no_collision
                    .byte $00
; Store A to both player_x and $D000 (VIC Sprite 0 X position).
set_sprite0_x       sta a_player_x       ; Store A to player_x and $D000 (Sprite 0 X position). ; x-ref: $5e5f; Player sprite 0 saved X position. Written by set_sprite0_x, read by restore_sprite0_position.
                    sta $d000            ; Sprite 0 X Pos
                    rts 
; Store A to both player_y and $D001 (VIC Sprite 0 Y position).
set_sprite0_y       sta a_player_y       ; Store A to player_y and $D001 (Sprite 0 Y position). ; x-ref: $5e65; Player sprite 0 saved Y position. Written by set_sprite0_y.
                    sta $d001            ; Sprite 0 Y Pos
                    rts 
; Store A to player_xmsb, then call set_sprite0_xmsb to apply to $D010 bit 0.
set_sprite0_xmsb_and_save
                    sta a_player_xmsb    ; Store A to player_xmsb then call set_sprite0_xmsb to apply to $D010 bit 0. ; x-ref: $5e6b
                    jsr set_sprite0_xmsb
                    rts 
                    .fill 27, $00
s_check_magic_cross_collect
                    lda ptr_hi           ; x-ref: $5b5b
                    nop 
                    clc 
                    adc #$d4
                    sta ptr_hi
                    lda (ptr_lo),y
                    and #$04
                    tax 
                    lda var_character_select
                    beq b_cross_char0_check
                    cpx #$00
                    beq b_cross_collect_success
                    rts 
b_cross_char0_check cpx #$04             ; x-ref: $7f60
                    beq b_cross_collect_success
                    rts 
b_cross_collect_success
                    lda ptr_hi           ; x-ref: $7f64, $7f69
                    sec 
                    sbc #$d4
                    sta ptr_hi
                    lda #$a0
                    sta (ptr_lo),y
                    jmp inc_magic_cross_right
                    .byte $ea, $ea, $ea, $ea, $ea, $ea
; 40-frame level scroll timer: decrement a_5A16; when zero reset to $28 and
; read next level map row from (ptr_level_map)+$5D, scroll each of 10 rows by -8 pixels.
level_scroll_tick   dec a_magic_cross_collected ; x-ref: $7fb1
                    bne r_scroll_tick_rts
                    lda #$28
                    sta a_magic_cross_collected
                    ldy #$5d
                    ldx #$00
b_scroll_row_loop   iny                  ; x-ref: $7fab
                    lda (ptr_level_map),y
                    sta ptr_lo
                    iny 
                    lda (ptr_level_map),y
                    clc 
                    adc #$d4
                    sta ptr_hi
                    tya 
                    pha 
                    ldy #$00
                    lda (ptr_lo),y
                    sec 
                    sbc #$08
                    sta (ptr_lo),y
                    pla 
                    tay 
                    inx 
                    cpx #$0a
                    bne b_scroll_row_loop
r_scroll_tick_rts   rts                  ; x-ref: $7f83
; === game_loop_tick === Main per-IRQ entry: calls sprite0_fall_step_update then enemy_state_loop.
game_loop_tick      jsr sprite0_fall_step_update ; x-ref: $75bf, $75c7
                    jsr level_scroll_tick
                    rts 
s_set_fall_jump_sprite_base
                    lda #$60
                    sta check_sliding_on_rope
                    lda a_rope_slide_speed
                    sta a_C79C
                    sta a_C7C6
                    lda a_input_delta_x
                    beq b_fall_saved_sprite
                    bpl b_fall_sprite_right
b_fall_sprite_left  lda a_rope_slide_timer
b_fall_sprite_store sta sprite_ptr0
                    rts
b_fall_saved_sprite
                    lda a_collision_check_temp
                    and #$04
                    beq b_fall_sprite_left
b_fall_sprite_right lda a_rope_slide_offset
                    bne b_fall_sprite_store
                    .fill 3, $00
                    .byte $28, $04, $f0, $07
a_level_ptr_lo      .byte $00            ; x-ref: $768a
a_level_ptr_hi      .byte $ec, $00, $00  ; x-ref: $7690
f_7FE8              .byte $00            ; x-ref: $7687
f_7FE9              .byte $78, $00, $e0, $00, $e4, $00, $e8, $00 ; x-ref: $768d
                    .byte $ec, $00, $00, $00, $00, $00, $00, $4e
                    .byte $3a, $41, $49, $50, $2c, $44, $24
.binary "../../chiller_refined_asset_8000_a7ad_level_graphics.bin"
j_A7AE              .byte $ff            ; x-ref: $fff4; Invalid or partial instruction
                    .byte $ff            ; Invalid or partial instruction
                    .byte $b9            ; Invalid or partial instruction
.binary "../../chiller_refined_asset_a7b1_bfff_background_graphics.bin"
; ====================================================
