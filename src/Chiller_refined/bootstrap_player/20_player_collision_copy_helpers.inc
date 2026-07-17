;==============================================================================
; $2A00-$2BFF - PLAYER GRID COLLISION, COPY PARAMS, RENDER HELPERS
;==============================================================================
;
; Converts Sprite 0 pixel position to screen-cell pointers, checks collision
; tiles, triggers fall/jump state, patches the block-copy engine parameters, and
; contains early render/decompression helpers.
;
b_pos_x_underflow   lsr a                ; x-ref: $2a17
                    clc 
                    bcc b_pos_x_divide
; ====================================================
;    PLAYER MOVEMENT AND COLLISION ENGINE
; ====================================================
;
; This subsystem converts sprite pixel positions into C64 character-grid cells,
; checks nearby tiles, tracks rope/platform/collectible interactions, and starts
; the fall/jump animation state machine when needed.
;
; Key state:
;   a_var_player_dir     movement direction: 0 up, 1 down, 2 left, 3 right
;   a_var_jump_phase     outbound/return phase of the easing animation
;   a_var_jump_timer     per-step delay while falling/jumping
;   ptr_lo/ptr_hi        screen/tile pointer scratch
;
s_player_position   sta a_var_player_dir ; x-ref: $536d, $53a5, $546f, $c530, $c535, ...; Player movement direction: 0=up, 1=down, 2=left, 3=right.
                    lda $d001            ; Sprite 0 Y Pos
                    sec 
a_smc_player_y_bound =*+$01
                    sbc #$2c
                    lsr a
                    lsr a
                    lsr a
                    tay 
                    lda $d000            ; Sprite 0 X Pos
                    sec 
                    sbc #$0c
                    bcc b_pos_x_underflow
                    lsr a
                    sta a_var_player_x_low
                    lda $d010            ; Sprites 0-7 MSB of X coordinate
                    and #$01
                    beq b_pos_x_msb_zero
                    lda #$80
b_pos_x_msb_zero    ora a_var_player_x_low ; x-ref: $2a22
b_pos_x_divide      lsr a                ; x-ref: $2a02
                    lsr a
                    tax 
                    tya 
                    asl a
                    tay 
                    lda f_screen_row_offsets,y
                    sta ptr_lo
                    lda f_screen_row_offsets_hi,y
                    sta ptr_hi
                    txa 
                    clc 
                    adc ptr_lo
                    sta ptr_lo
                    bcc b_pos_row_hi_done
                    inc ptr_hi
b_pos_row_hi_done   lda ptr_hi           ; x-ref: $2a3f
                    clc 
                    adc #$04
                    sta ptr_hi
                    ldx a_var_player_dir ; Player movement direction: 0=up, 1=down, 2=left, 3=right.
                    lda f_dir_screen_check_offsets,x
                    tay 
                    lda (ptr_lo),y
                    cmp #$a0
                    bne b_pos_check_tile_type
b_player_no_collision
                    ldx #$00             ; no collision path: X=0 (no-hit flag) ; x-ref: $2a64, $5aa2, $5b91, $5b9d, $5baa
                    lda a_var_player_dir ; Player movement direction: 0=up, 1=down, 2=left, 3=right.
                    jsr s_update_sprite_coordinates
                    rts 
b_pos_check_tile_type
                    cmp #$2a             ; x-ref: $2a55
                    bpl b_if_not_platform
                    jmp b_player_no_collision
b_if_not_platform   cmp #$54             ; x-ref: $2a62
                    bpl b_if_touching_mushroom
                    jmp check_interactive_or_solid_tile
b_if_touching_mushroom
                    jmp collect_mushroom     ; x-ref: $2a69
                    .byte $60
b_start_fall        sta a_C19B           ; x-ref: $c1ba
                    lda f_player_jump_easing_table
                    sta a_var_jump_timer ; Jump step delay timer: loaded from easing table l_544A,X; counts down before advancing step index.
                    jmp update_player_direction
a_var_jump_phase    .byte $07            ; Jump phase: 0=ascending/peak, 1=returning to ground. Controls easing table direction. ; x-ref: $53ab, $53cb, $53ce, $5444, $c1b7
a_var_jump_timer    .byte $09            ; Jump step delay timer: loaded from easing table l_544A,X; counts down before advancing step index. ; x-ref: $2a78, $5396, $53b8, $53db
; Configures and executes the self-modifying block copy engine (s_C29D).
; Caller passes X:Y as a pointer to a 6-byte parameter block, laid out as:
;   [0] source_lo, [1] source_hi  - copy source start address
;   [2] end_lo,    [3] end_hi     - copy end address (inclusive)
;   [4] dest_lo,   [5] dest_hi   - copy destination start address
; The routine patches these 6 values into s_C29D's inline immediate operands
; (self-modifying code), then calls s_C29D to perform the actual byte-by-byte copy.
; 
; Inputs:  X = param block pointer low byte, Y = param block pointer high byte
; Outputs: None
; Side Effects: Patches s_C29D's SMC operands; copies memory block
s_copy_data         stx ptr_lo           ; x-ref: $55ec, $55f3, $55fa, $5bcb, $5e39, ...
                    sty ptr_hi
                    ldy #$00
                    lda (ptr_lo),y
                    sta a_C29E
                    iny 
                    lda (ptr_lo),y
                    sta a_C2A2
                    iny 
                    lda (ptr_lo),y
                    sta a_C2AE
                    iny 
                    lda (ptr_lo),y
                    sta a_C2B3
                    iny 
                    lda (ptr_lo),y
                    sta a_C2A6
                    iny 
                    lda (ptr_lo),y
                    sta a_C2AA
                    jsr s_C29D
                    rts 
                    .byte $dc, $9d, $bd, $a0, $2a, $20, $80, $2a
                    .byte $a2, $c3, $a0, $2a, $20, $80, $2a, $60
                    .byte $28, $04, $ff, $07, $28, $41, $28, $d8
                    .byte $ff, $db, $28, $46, $ea, $ea, $ea
s_next_screen_row   tay                  ; x-ref: $c049
                    iny 
                    tya 
                    sta a_wrap_temp
                    cmp #$2a
                    bne b_wrap_skip_row1
                    clc 
                    lda ptr_lo
                    adc #$28
                    sta ptr_lo
b_wrap_skip_row1    cmp #$54             ; x-ref: $2ad4
                    bne b_wrap_skip_row2
                    clc 
                    lda ptr_lo
                    adc #$28
                    sta ptr_lo
b_wrap_skip_row2    lda a_wrap_temp      ; x-ref: $2adf
                    rts 
s_stop_fall_animation
                    sta a_C19C           ; x-ref: $2bee
                    lda #$00
                    sta a_fall_step_counter
                    rts 
                    .fill 8, $ea
a_wrap_temp         .byte $7e            ; x-ref: $2acf, $2ae8
; Player movement direction: 0=up, 1=down, 2=left, 3=right. Written by Sub_UpdateSpritePositions dispatch.
a_var_player_dir    .byte $01            ; Player movement direction: 0=up, 1=down, 2=left, 3=right. ; x-ref: $2a04, $2a4a, $2a59
a_var_player_x_low  .byte $58, $a9, $00, $85, $fb, $a9 ; x-ref: $2a1a, $2a26
                    cld 
                    sta ptr_hi
                    lda #$00
                    sta a_0384
                    lda #$00
                    sta a_0386
b_render_sprites_row_loop
                    lda #$00             ; x-ref: $2b85
                    sta a_0385
b_render_sprites_col_loop
                    lda #$00             ; x-ref: $2b6e
                    sta a_0387
b_render_sprites_pixel_loop
                    ldy #$00             ; x-ref: $2b61
                    ldx a_0386
                    lda f_active_sprites_start,x
                    sta a_0388
                    ldx a_0387
                    lda f_render_bit_mask,x
                    and a_0388
s_decompress_shift_loop
                    cmp #$00             ; x-ref: $2b42
                    beq b_render_write_char
                    cmp #$01
                    beq b_render_write_char
                    cmp #$02
                    beq b_render_write_char
                    cmp #$03
                    beq b_render_write_char
                    lsr a
                    lsr a
                    jmp s_decompress_shift_loop
b_render_write_char tax                  ; x-ref: $2b32, $2b36, $2b3a, $2b3e
                    lda f_render_pixel_vals,x
                    sta (ptr_lo),y
                    inc a_0387
                    inc ptr_lo
                    bne b_render_inc_hi1
                    inc ptr_hi
b_render_inc_hi1    sta (ptr_lo),y       ; x-ref: $2b50
                    inc ptr_lo
                    bne b_render_inc_hi2
                    inc ptr_hi
b_render_inc_hi2    lda a_0387           ; x-ref: $2b58
                    cmp #$04
                    bne b_render_sprites_pixel_loop
                    inc a_0386
                    inc a_0385
                    lda a_0385
                    cmp #$03
                    bne b_render_sprites_col_loop
                    lda #$10
b_render_pad_loop   inc ptr_lo           ; x-ref: $2b7b
                    bne b_render_pad_done
                    inc ptr_hi
b_render_pad_done   clc                  ; x-ref: $2b74
                    sbc #$00
                    bne b_render_pad_loop
                    inc a_0384
                    lda a_0384
                    cmp #$15
                    bne b_render_sprites_row_loop
                    rts 
f_render_bit_mask   cpy #$30             ; x-ref: $2b2a
                    .byte $0c, $03
f_render_pixel_vals .byte $f0, $f9, $fb, $f1, $ad, $25, $d0, $8d ; x-ref: $2b46
                    .byte $8d, $2b, $ad, $26, $d0, $8d, $8f, $2b
                    .byte $ad, $2a, $d0, $8d, $8e, $2b, $ad, $0f
                    .byte $45, $8d, $8c, $2b, $4c, $00, $2b, $a2
                    .byte $08, $a9, $01, $a0, $01, $20, $ba, $ff
                    .byte $ad, $84, $03, $a2, $85, $a0, $03, $20
                    .byte $bd, $ff, $60, $20, $ab, $2b, $a9, $00
                    .byte $85, $fb, $a9, $30, $85, $fc, $a9, $fb
                    .byte $a2, $00, $a0, $4e, $20, $d8, $ff, $60
                    .byte $20, $ab, $2b, $a9, $00, $a2, $00, $a0
                    .byte $30, $20, $d5, $ff, $60
s_silence_voice3_and_stop_fall
                    lda #$00             ; x-ref: $ca80
                    sta $d412            ; Voice 3: Control Register
                    lda #$00
                    nop 
                    nop 
                    nop 
                    nop 
                    nop 
                    nop 
                    jmp s_stop_fall_animation
                    .byte $60
s_set_screen_control
                    sta $d015            ; x-ref: $2ce0; Sprite display Enable
                    lda $d016            ; VIC Control Register 2
                    ora #$08
                    sta $d016            ; VIC Control Register 2
                    rts 
                    .byte $ea, $1d
