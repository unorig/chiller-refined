                    .byte $00, $b2, $ff, $b5, $00, $30, $ea, $ea
; === start_new_level === Copies level data to RAM, disables sprites, clears background colors, spawns next wave, restarts SID music pointer.
start_new_level     ldx #$bf             ; x-ref: $7653
                    ldy #$5b
                    jsr s_copy_data
                    lda #$00
                    sta $d015            ; Sprite display Enable
                    lda #$00
                    sta $d022            ; Background Color 1, Multi-Color Register 0
                    sta $d023            ; Background Color 2, Multi-Color Register 1
                    ldy #$74
                    lda (ptr_level_map),y
                    tax 
                    iny 
                    lda (ptr_level_map),y
                    tay 
                    jsr enemy_slot_spawn
                    sei 
                    lda #$18
                    sta snd_ptr1_lo
                    lda #$6a
                    sta snd_ptr1_hi
                    cli 
b_level_start_done  jmp level_screen_setup ; x-ref: $5bf4
                    beq b_level_start_done
                    rts 
                    .byte $ea, $ea, $ea, $ea
a_raster_wait_temp_1
                    .byte $6a            ; x-ref: $5caa, $5cad, $5cca
a_raster_wait_temp_2
                    .byte $00            ; x-ref: $5ccd
a_raster_wait_temp_3
                    .byte $01            ; x-ref: $5c5c, $5c87, $5c8a, $5c93, $5cd0
a_raster_wait_temp_4
                    .byte $00            ; x-ref: $5c3b, $5c3e, $5c47, $5cc7
a_raster_wait_temp_5
                    .byte $15            ; x-ref: $5c02, $5c96, $5c9c, $5ca0, $5ca6, ...
; Check if all enemies are cleared; if so advance level counter and trigger level-complete sequence.
raster_wait_screen  lda #$e7             ; x-ref: $5c5f
                    sta a_raster_wait_temp_5
                    jsr score_display_refresh
                    tax 
                    lda #$45
                    sta ptr_lo
                    lda #$05
                    sta ptr_hi
                    lda #$45
                    sta ptr2_lo
                    lda #$d9
                    sta ptr2_hi
                    ldy #$00
screen_level_draw   jsr enemy_slot_move  ; x-ref: $5c31, $5c4b
                    jmp screen_draw_advance
screen_draw_loop    sta (ptr_lo),y       ; x-ref: $5cb7
                    lda #$0a
                    sta (ptr2_lo),y
                    inx 
                    cpx #$05
                    beq b_draw_loop_y
                    tya 
                    clc 
                    adc #$28
                    tay 
                    jmp screen_level_draw
b_draw_loop_y       tya                  ; x-ref: $5c2a
                    sec 
                    sbc #$9f
                    tay 
                    ldx #$00
                    inc a_raster_wait_temp_4
                    lda a_raster_wait_temp_4
                    cmp #$03
                    bne b_draw_loop_x
                    lda #$00
                    sta a_raster_wait_temp_4
                    iny 
b_draw_loop_x       jmp screen_level_draw ; x-ref: $5c43
                    .byte $60
; Spawn new enemy into the next available sprite slot, set initial position from wave data table.
enemy_slot_spawn    jsr lives_display_refresh ; x-ref: $5be4, $5e04, $7764
                    nop 
                    nop 
                    lda #$00
                    sta $d020            ; Border Color
                    sta $d021            ; Background Color 0
                    sta a_raster_wait_temp_3
                    jsr raster_wait_screen
                    lda $d016            ; VIC Control Register 2
                    ora #$10
                    sta $d016            ; VIC Control Register 2
                    lda #$1c
                    sta $d018            ; VIC Memory Control Register
                    ldx #$00
b_enemy_spawn_done  lda f_hud_stage_text_forest,x ; x-ref: $5c7f
                    sta f_04D6,x
                    lda #$02
                    sta f_D8D6,x
                    inx 
                    cpx #$0e
                    bne b_enemy_spawn_done
                    jsr set_mastertronic_apostrophe
                    rts 
                    .byte $ea, $ea
; Update all active enemy sprites: move one pixel in current direction,
; bounce at boundary via move_sprite_directional.
enemy_slot_move     inc a_raster_wait_temp_3 ; x-ref: $5c1b
                    lda a_raster_wait_temp_3
                    cmp #$06
                    bne b_enemy_move_done
                    lda #$01
                    sta a_raster_wait_temp_3
                    lda a_raster_wait_temp_5
                    sec 
                    sbc #$63
                    sta a_raster_wait_temp_5
                    rts 
b_enemy_move_done   lda a_raster_wait_temp_5 ; x-ref: $5c8f
                    clc 
                    adc #$19
                    sta a_raster_wait_temp_5
                    rts 
screen_draw_advance inc a_raster_wait_temp_1 ; x-ref: $5c1e
                    lda a_raster_wait_temp_1
                    cmp #$6a
                    beq b_draw_advance_done
                    lda a_raster_wait_temp_5
                    jmp screen_draw_loop
b_draw_advance_done ldy #$31             ; x-ref: $5cb2
                    lda #$32
                    sta (ptr_lo),y
                    lda #$0a
                    sta (ptr2_lo),y
                    rts 
; Refresh the on-screen score display: write score digits from a_CFA7/$CFA8/$CFA9 to screen RAM.
score_display_refresh
                    lda #$00             ; x-ref: $5c05
                    sta a_raster_wait_temp_4
                    sta a_raster_wait_temp_1
                    sta a_raster_wait_temp_2
                    sta a_raster_wait_temp_3
                    rts 
                    .byte $00, $00, $00, $00
; Refresh the on-screen lives indicator: write lives icons from game state to screen RAM.
lives_display_refresh
                    lda #$93             ; x-ref: $5c4f
                    jsr KERNAL_CHROUT    ; $FFD2 - output character
                    stx ptr_lo
                    sty ptr_hi
                    ldy #$00
                    lda (ptr_lo),y
                    tax 
                    iny 
                    lda (ptr_lo),y
                    tay 
                    clc 
                    jsr KERNAL_PLOT      ; $FFF0 - read/set cursor position
                    ldy #$02
lives_icon_loop     lda (ptr_lo),y       ; x-ref: $5cfa
                    cmp #$01
                    beq r_lives_draw_rts
                    iny 
                    jsr KERNAL_CHROUT    ; $FFD2 - output character
                    jmp lives_icon_loop
r_lives_draw_rts    rts                  ; x-ref: $5cf4
                    .byte $ea, $ea
