;==============================================================================
; $C3DD-$C5FF - PATH STEPPING, SCROLLING, HUD SETUP, FLICKER, BIT FLIP
;==============================================================================
;
; Covers path stepping, row scrolling, raster-synchronized scroll updates, HUD
; stats initialization, Sprite 1 flicker, and the sprite mirror bit-flip helper
; used for sprite/background collision-style effects.
;
; === Sprite step path handler ===
; Periodically ticks through the active sprite path lookup arrays (f_sprite_movement_paths) using offset index pointers in page 3 ($0380).
                    ldx a_intro_path_step_ctr
                    lda f_sprite_movement_paths,x
                    beq r_C3DC
                    bmi r_C3DC
                    ldy a_C3A6
                    dey 
                    sty $ffff            ; IRQ
r_C3DC              rts                  ; x-ref: $c3d1, $c3d3
                    dec $d001            ; Sprite 0 Y Pos
                    dec $d003            ; Sprite 1 Y Pos
                    dec $d005            ; Sprite 2 Y Pos
                    dec $d007            ; Sprite 3 Y Pos
                    dec $d009            ; Sprite 4 Y Pos
                    dec $d00b            ; Sprite 5 Y Pos
                    dec $d00d            ; Sprite 6 Y Pos
                    dec $d00f            ; Sprite 7 Y Pos
                    rts 
                    lda #$00
                    sta a_CEFF
b_C3FB              jsr s_initialize_enemy_sprite ; x-ref: $c404
                    inc a_CEFF
                    lda a_CEFF
                    bne b_C3FB
                    rts 
j_C407              sta $d010            ; x-ref: $c1e9; Sprites 0-7 MSB of X coordinate
                    lda a_xmsb_scroll_config
                    jmp j_CEF6
a_C410              .byte $bb            ; x-ref: $c415, $c4be
a_C411              .byte $a0            ; x-ref: $c41b, $c4c4
; === Screen character row horizontal shifter ===
; Shifts characters and colors of rows 1-6 left or right dynamically, producing the side-scrolling effect on active platforms.
s_scroll_rows_horizontal
                    lda a_D828           ; x-ref: $c472
                    sta a_C410
                    lda screen_row1_right_save
                    sta a_C411
                    ldx #$00
b_scroll_row1_loop  lda screen_row1,x    ; x-ref: $c42f
                    sta screen_row1_right_save,x
                    lda f_D829,x
                    sta a_D828,x
                    inx 
                    cpx #$d2
                    bne b_scroll_row1_loop
                    ldx #$00
b_scroll_row3_loop  lda f_04FA,x         ; x-ref: $c442
                    sta screen_row3,x
                    lda f_D8FA,x
                    sta f_D8F9,x
                    inx 
                    cpx #$fa
                    bne b_scroll_row3_loop
                    ldx #$00
b_scroll_row5_loop  lda f_05F4,x         ; x-ref: $c455
                    sta f_05F3,x
                    lda f_D9F4,x
                    sta f_D9F3,x
                    inx 
                    cpx #$fa
                    bne b_scroll_row5_loop
                    ldx #$00
b_scroll_row6_loop  lda f_06EE,x         ; x-ref: $c468
                    sta screen_row6_end,x
                    lda f_DAEE,x
                    sta f_DAED,x
                    inx 
                    cpx #$fa
                    bne b_scroll_row6_loop
                    rts 
; === Raster sync line scroll driver ===
; Waits for VIC scanline $4B, triggers the row shifter (s_C412), and jumps to the vertical screen scroll registers update.
s_scroller_raster_sync
                    lda $d012            ; x-ref: $c02e, $c470; Raster Position
                    cmp #$4b
                    bne s_scroller_raster_sync
                    jsr s_scroll_rows_horizontal
                    jmp j_C47F
                    .byte $ea, $ad, $76, $c4, $ea, $ea, $ea
j_C47F              lda #$bf             ; x-ref: $c475
                    sta ptr_lo
                    lda #$07
                    sta ptr_hi
                    lda #$bf
                    sta ptr2_lo
                    lda #$db
                    sta ptr2_hi
b_C48F              ldy #$00             ; x-ref: $c4b6, $c4bc
                    lda (ptr_lo),y
                    tax 
                    lda (ptr2_lo),y
                    ldy #$28
                    sta (ptr2_lo),y
                    txa 
                    sta (ptr_lo),y
                    lda ptr_lo
                    clc 
                    sbc #$27
                    bcs b_C4A8
                    dec ptr_hi
                    dec ptr2_hi
b_C4A8              sta ptr_lo           ; x-ref: $c4a2
                    sta ptr2_lo
                    lda ptr_hi
                    cmp #$03
                    beq b_C4BE
                    lda ptr_lo
                    cmp #$27
                    bne b_C48F
                    lda ptr_hi
                    cmp #$04
                    bne b_C48F
b_C4BE              lda a_C410           ; x-ref: $c4b0
                    sta a_D84F
                    lda a_C411
                    sta scroll_corner_save
                    rts 
                    nop 
                    nop 
s_init_hud_stats    lda a_sprite_system_config ; x-ref: $2ef1
                    jsr s_draw_player_hud
                    rts 
                    lda $d02a            ; Sprite 3 Color
                    sta a_D824
                    sta a_D825
                    lda $d025            ; Sprite Multi-Color Register 0
                    sta a_sprite_mc_reg0_init_val
                    sta a_D84D
                    sta a_D84C
                    lda $d026            ; Sprite Multi-Color Register 1
                    sta a_D875
                    sta a_D874
                    sta a_sprite_mc_reg1_init_val
                    rts 
                    .byte $f8, $60, $ea, $ea, $ea
; === Sprite 1 visual flicker logic ===
; Swaps sprite indices to alternate Sprite 1 frames and toggle display state on alternate vertical blanks.
s_flicker_sprite1_blank
                    cmp #$01             ; x-ref: $2d71
                    bne b_toggle_sprite1_ptr
s_disable_sprite1   lda $d015            ; x-ref: $c521; Sprite display Enable
                    and #$fd
                    sta $d015            ; Sprite display Enable
                    rts 
b_toggle_sprite1_ptr
                    lda sprite_ptr1      ; x-ref: $c4fd
                    eor #$ff
                    eor #$fe
                    sta sprite_ptr1
                    inc a_sprite1_flicker_ctr
                    lda a_sprite1_flicker_ctr
                    cmp #$05
                    bne r_flicker_rts
                    lda #$00
                    sta a_sprite1_flicker_ctr
                    jmp s_disable_sprite1
r_flicker_rts       rts                  ; x-ref: $c51a
s_player_position_tick
                    lda a_sprite_anim_global_timer ; x-ref: $c01b
                    and #$07
                    bne b_C538
                    ldx #$01
                    lda #$03
                    jsr s_player_position
                    lda #$02
                    jsr s_player_position
b_C538              lda a_sprite_anim_global_timer ; x-ref: $c52a
                    rts 
                    .byte $ea, $ea
a_decomp_bit7       .byte $00            ; x-ref: $c551, $c5ed
a_decomp_bit6       .byte $00            ; x-ref: $c557, $c5de
a_decomp_bit5       .byte $00            ; x-ref: $c55d, $c5cf
a_decomp_bit4       .byte $00            ; x-ref: $c563, $c5c0
a_decomp_bit3       .byte $00            ; x-ref: $c569, $c5b1
a_decomp_bit2       .byte $00            ; x-ref: $c56f, $c5a2
a_sprite1_flicker_ctr
                    .byte $00            ; x-ref: $2d92, $c512, $c515, $c51e, $c575, ...
; === Sprite-to-background priority collision generator ===
; Checks Sprite 0 collision flag registers, sweeps active collision maps, and updates the mirrored frame buffers on overlap.
a_decomp_bit0       brk                  ; x-ref: $c57b, $c584
a_flipped_byte_result
                    brk                  ; x-ref: $c581, $c58d, $c590, $c59c, $c59f, ...
                    and #$80
                    ldx #$00
j_C54B              lda f_sprite_0_mirror_buffer,x ; x-ref: $c601
                    tay 
                    and #$80
                    sta a_decomp_bit7
                    tya 
                    and #$40
                    sta a_decomp_bit6
                    tya 
                    and #$20
                    sta a_decomp_bit5
                    tya 
                    and #$10
                    sta a_decomp_bit4
                    tya 
                    and #$08
                    sta a_decomp_bit3
                    tya 
                    and #$04
                    sta a_decomp_bit2
                    tya 
                    and #$02
                    sta a_sprite1_flicker_ctr
                    tya 
                    and #$01
                    sta a_decomp_bit0
                    nop 
                    lda #$00
                    sta a_flipped_byte_result
                    lda a_decomp_bit0
                    cmp #$00
                    beq b_flip_bit0_done
                    lda #$80
b_flip_bit0_done    ora a_flipped_byte_result ; x-ref: $c589
                    sta a_flipped_byte_result
                    lda a_sprite1_flicker_ctr
                    cmp #$00
                    beq b_flip_bit1_done
                    lda #$40
b_flip_bit1_done    ora a_flipped_byte_result ; x-ref: $c598
                    sta a_flipped_byte_result
                    lda a_decomp_bit2
                    cmp #$00
                    beq b_flip_bit2_done
                    lda #$20
b_flip_bit2_done    ora a_flipped_byte_result ; x-ref: $c5a7
                    sta a_flipped_byte_result
                    lda a_decomp_bit3
                    cmp #$00
                    beq b_flip_bit3_done
                    lda #$10
b_flip_bit3_done    ora a_flipped_byte_result ; x-ref: $c5b6
                    sta a_flipped_byte_result
                    lda a_decomp_bit4
                    cmp #$00
                    beq b_flip_bit4_done
                    lda #$08
b_flip_bit4_done    ora a_flipped_byte_result ; x-ref: $c5c5
                    sta a_flipped_byte_result
                    lda a_decomp_bit5
                    cmp #$00
                    beq b_flip_bit5_done
                    lda #$04
b_flip_bit5_done    ora a_flipped_byte_result ; x-ref: $c5d4
                    sta a_flipped_byte_result
                    lda a_decomp_bit6
                    cmp #$00
                    beq b_flip_bit6_done
                    lda #$02
b_flip_bit6_done    ora a_flipped_byte_result ; x-ref: $c5e3
                    sta a_flipped_byte_result
                    lda a_decomp_bit7
                    cmp #$00
                    beq b_flip_bit7_done
                    lda #$01
b_flip_bit7_done    ora a_flipped_byte_result ; x-ref: $c5f2
                    sta f_sprite_0_mirror_buffer,x
                    inx 
                    cpx #$40
                    beq r_flip_rts
                    jmp j_C54B
r_flip_rts          rts                  ; x-ref: $c5ff
                    .byte $8f, $00
