s_update_player_projectile
                    lda $d015            ; x-ref: $ca3c; Sprite display Enable
                    and #$80
                    cmp #$00
                    beq b_projectile_range_check
                    jmp s_tick_projectile_step
b_projectile_range_check
                    jsr s_check_projectile_range ; x-ref: $ccfc
                    cpy a_sprite7_range
                    bpl s_tick_projectile_step
b_projectile_watermark_loop
                    jsr s_increment_watermark ; x-ref: $cd11
                    tya 
                    and #$07
                    cmp #$05
                    bpl b_projectile_watermark_loop
                    tax 
                    jmp s_sprite_overlap_handler
s_spawn_player_projectile
                    cmp #$04             ; x-ref: $ca7a
                    beq s_tick_projectile_step
                    sta a_sprite_projectile_dir
                    lda $d015            ; Sprite display Enable
                    ora #$80
                    sta $d015            ; Sprite display Enable
                    lda f_enemy_type_table
                    sta $d02e            ; Sprite 7 Color
                    txa 
                    asl a
                    tax 
                    lda $d004,x          ; Sprite 2 X Pos
                    sta $d00e            ; Sprite 7 X Pos
                    lda $d005,x          ; Sprite 2 Y Pos
                    sta $d00f            ; Sprite 7 Y Pos
                    txa 
                    lsr a
                    tax 
                    lda #$7f
                    and $d010            ; Sprites 0-7 MSB of X coordinate
                    sta $d010            ; Sprites 0-7 MSB of X coordinate
                    and f_sprite_enable_bits,x
                    cmp #$00
                    beq s_tick_projectile_step
                    lda #$80
                    ora $d010            ; Sprites 0-7 MSB of X coordinate
                    sta $d010            ; Sprites 0-7 MSB of X coordinate
s_tick_projectile_step
                    inc a_sprite_projectile_steps ; x-ref: $c00e, $ca7d, $ccfe, $cd07, $cd19, ...
                    lda a_sprite_projectile_steps
                    cmp a_sprite7_steps
                    bne r_projectile_step_rts
                    lda #$00
                    sta a_sprite_projectile_steps
                    lda a_sprite_projectile_dir
                    ldx #$07
                    jsr s_update_sprite_coordinates
                    cpy #$ff
                    bne r_projectile_step_rts
                    lda $d015            ; Sprite display Enable
                    and #$7f
                    sta $d015            ; Sprite display Enable
r_projectile_step_rts
                    rts                  ; x-ref: $cd5e, $cd6f
                    .byte $ea
; === Projectile Range and Border Collision Calculator ===
; Decrypts signature values to set active projectile Y limits and performs coordinate comparisons against horizontal C64 screen border boundaries ($78).
s_check_projectile_range
                    jsr s_increment_watermark ; x-ref: $cd01
                    tya 
                    and #$7f
                    sta a_temp_projectile_y
                    jsr s_increment_watermark
                    tya 
                    and #$7f
                    cmp #$78
                    bpl b_projectile_in_range
                    ldy #$7f
                    jmp r_projectile_range_rts
b_projectile_in_range
                    ldy a_temp_projectile_y ; x-ref: $cd8c
r_projectile_range_rts
                    rts                  ; x-ref: $cd90, $cda3, $cdbb
                    nop 
                    nop 
                    nop 
                    jmp s_sfx_helper_2
                    lda ptr2_lo
                    and #$02
                    bne b_projectile_impact_check
                    jmp r_projectile_range_rts
b_projectile_impact_check
                    ldx #$00             ; x-ref: $cda1
                    ldy #$00
b_CDAA              lda f_sprite_enable_bits,x ; x-ref: $cdb5, $cdff
                    and ptr2_lo
                    beq b_CDB2
                    iny 
b_CDB2              inx                  ; x-ref: $cdaf
                    cpx #$05
                    bne b_CDAA
                    cpy #$00
                    bne b_CDBE
                    jmp r_projectile_range_rts
b_CDBE              ldx #$02             ; x-ref: $cdb9
                    jsr queue_damage_effect_ticks
                    rts 
                    .byte $da, $da, $da, $ad, $4d, $cf, $c9, $04
                    .byte $f0, $05, $e8, $4a, $4c, $ca, $cd, $8e
b_CDD4              lsr a_BDCF           ; x-ref: $ce52
                    lsr zpp_45,x
                    beq r_CDE8
                    jmp s_unref_check_enemy_active
                    ldx #$02
                    jsr queue_damage_effect_ticks
                    rts 
                    .byte $01, $4c, $02, $ce
r_CDE8              rts                  ; x-ref: $cdd9
