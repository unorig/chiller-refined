;==============================================================================
; $C148-$C3DC - PLAYER RESTORE, INPUT DECODING, BLOCK COPY, PATH SETUP
;==============================================================================
;
; Contains Sprite 0 restore/HUD row helpers, joystick decoding, manual sprite X
; movement loops, and the self-modifying block-copy engine used by higher-level
; setup code. The SMC labels in this file are address-sensitive.
;
; === Sprite 0 position restoration ===
; Restores Sprite 0 coordinate parameters to hardware VIC registers ($D000/$D001) from game variable memory snapshot (a_player_x/y).
s_restore_player_sprite_pos
                    sta $d001            ; x-ref: $c035; Sprite 0 Y Pos
                    lda a_player_x       ; Player sprite 0 saved X position. Written by set_sprite0_x, read by restore_sprite0_position.
                    sta $d000            ; Sprite 0 X Pos
                    jmp s_set_player_sprite0_xmsb
                    ldx #$00
b_mirror_pixel_loop lda f_sprite_0_mirror_buffer,x ; x-ref: $c16d
                    sta a_mirror_temp_1
                    and #$55
                    asl a
                    sta a_mirror_temp_2
                    lda a_mirror_temp_1
                    and #$aa
                    lsr a
                    ora a_mirror_temp_2
                    sta f_sprite_0_mirror_buffer,x
                    inx 
                    cpx #$40
                    bne b_mirror_pixel_loop
                    rts 
s_clear_hud_rows    ldx #$00
                    lda #$20
b_clear_hud_loop    sta f_05EE,x         ; x-ref: $c180
                    sta f_06EE,x
                    sta f_053C,x
                    inx 
                    cpx #$00
                    bne b_clear_hud_loop
r_clear_hud_rts     rts 
                    nop 
                    nop 
                    jmp f_player_jump_easing_table
                    brk 
                    bmi b_C18B
b_C18B              .byte $34, $e8, $03, $00, $4e, $00, $52, $00 ; x-ref: $c189
                    .byte $30, $e8, $03, $e8, $07, $00, $4e, $ea
a_C19B              .byte $01            ; x-ref: $2a72, $53c0, $5431, $5441, $76c4
a_C19C              .byte $00            ; x-ref: $2aec, $5391, $53e4, $541f, $547d, ...
s_eval_level_movement_type
                    lda a_level_config_reg_4 ; x-ref: $c7d1
                    bne b_check_movement_stall
                    lda $d015            ; Sprite display Enable
                    jmp s_spawn_player_attack_flash
b_check_movement_stall
                    lda a_C19C           ; x-ref: $c1a0
                    beq b_route_fall_check
                    jmp b_check_movement_delay
b_route_fall_check  jmp fall_anim_check  ; x-ref: $c1ab
                    .byte $ea
s_reset_player_jump_phase
                    nop                  ; x-ref: $5422
                    lda #$00
                    sta a_var_jump_phase ; Jump phase: 0=ascending/peak, 1=returning to ground. Controls easing table direction.
                    jmp b_start_fall
a_mirror_temp_2     brk                  ; x-ref: $c15b, $c164
a_mirror_temp_1     brk                  ; x-ref: $c155, $c15e
                    brk 
s_read_joystick_port2_shifts
                    lda $dc00            ; CIA1: Data Port Register A
                    ldy #$00
                    ldx #$00
                    lsr a
                    bcs b_joy_down_check
                    dey 
b_joy_down_check    lsr a                ; x-ref: $c1c8
                    bcs b_joy_left_check
                    iny 
b_joy_left_check    lsr a                ; x-ref: $c1cc
                    bcs b_joy_right_check
                    dex 
b_joy_right_check   lsr a                ; x-ref: $c1d0
                    bcs b_joy_parse_done
                    inx 
b_joy_parse_done    lsr a                ; x-ref: $c1d4
                    stx a_input_delta_x
                    sty a_input_delta_y
                    jmp s_check_secondary_fire
s_set_player_sprite0_xmsb
                    lda $d010            ; x-ref: $c14d; Sprites 0-7 MSB of X coordinate
                    and #$fe
                    ora a_player_xmsb
                    jmp j_C407
a_input_delta_y     .byte $00            ; x-ref: $5575, $5581, $c1db, $c21c, $c222, ...
a_input_delta_x     .byte $00            ; x-ref: $555d, $5569, $5815, $5885, $5a20, ...
a_input_fire_button .byte $00            ; x-ref: $558d, $c22c, $c234, $c23c, $c7ca, ...
s_decode_joystick_input
                    ldx music_v1_ctrl    ; x-ref: $c237, $c23f
                    lda zp_joystick_port2_state
                    ldy a_zero_init_buf_3245
                    cmp #$40
                    beq r_decode_rts
                    cmp #$07
                    beq b_decode_vertical
                    cmp #$02
                    beq b_decode_horizontal
                    jmp s_decode_joystick_fire
b_decode_horizontal cpx #$00             ; x-ref: $c201
                    beq b_decode_right
                    lda #$ff
                    sta a_input_delta_x
                    rts 
b_decode_right      lda #$01             ; x-ref: $c208
                    sta a_input_delta_x
r_decode_rts        rts                  ; x-ref: $c1f9
b_decode_vertical   cpx #$00             ; x-ref: $c1fd
                    beq b_decode_down
                    lda #$ff
                    sta a_input_delta_y
                    rts 
b_decode_down       lda #$01             ; x-ref: $c218
                    sta a_input_delta_y
r_fire_rts          rts                  ; x-ref: $c228
s_decode_joystick_fire
                    cmp #$3c             ; x-ref: $c203
                    bne r_fire_rts
                    lda #$01
                    sta a_input_fire_button
                    rts 
s_check_secondary_fire
                    bcs b_clear_secondary_fire ; x-ref: $c1de
                    lda #$01
                    sta a_input_fire_button
                    jmp s_decode_joystick_input
b_clear_secondary_fire
                    lda #$00             ; x-ref: $c230
                    sta a_input_fire_button
                    jmp s_decode_joystick_input
                    ldy #$38
b_C244              inc $d000            ; x-ref: $c26b; Sprite 0 X Pos
                    lda $d000            ; Sprite 0 X Pos
                    cmp #$00
                    bne b_C256
                    lda $d010            ; Sprites 0-7 MSB of X coordinate
                    ora #$01
                    sta $d010            ; Sprites 0-7 MSB of X coordinate
b_C256              dey                  ; x-ref: $c24c
                    cpy #$00
                    lda #$00
b_C25B              ldx #$00             ; x-ref: $c267
b_C25D              inx                  ; x-ref: $c260
                    cpx #$00
                    bne b_C25D
                    tax 
                    inx 
                    txa 
                    cmp #$06
                    bne b_C25B
                    cpy #$00
                    bne b_C244
                    rts 
                    ldy #$28
b_C270              dec $d000            ; x-ref: $c297; Sprite 0 X Pos
                    lda $d000            ; Sprite 0 X Pos
                    cmp #$ff
                    bne b_C282
                    lda $d010            ; Sprites 0-7 MSB of X coordinate
                    and #$fe
                    sta $d010            ; Sprites 0-7 MSB of X coordinate
b_C282              dey                  ; x-ref: $c278
                    cpy #$00
                    lda #$00
b_C287              ldx #$00             ; x-ref: $c293
b_C289              inx                  ; x-ref: $c28c
                    cpx #$00
                    bne b_C289
                    tax 
                    inx 
                    txa 
                    cmp #$06
                    bne b_C287
                    cpy #$00
                    bne b_C270
                    rts 
                    .byte $ea, $ea, $ea
a_C29E =*+$01
s_C29D              lda #$00             ; x-ref: $2aa9
                    sta ptr_lo
a_C2A2 =*+$01
                    lda #$a8
                    sta ptr_hi
a_C2A6 =*+$01
                    lda #$00
                    sta ptr2_lo
a_C2AA =*+$01
b_C2A9              lda #$30             ; x-ref: $c318
                    sta ptr2_hi
a_C2AE =*+$01
                    lda #$ff
                    sta block_copy_end_lo
a_C2B3 =*+$01
                    lda #$ab
                    sta block_copy_end_hi
                    ldy #$00
b_C2B9              lda (ptr_lo),y       ; x-ref: $c2ce, $c2d5
                    sta (ptr2_lo),y
                    inc ptr_lo
                    bne b_C2C3
                    inc ptr_hi
b_C2C3              inc ptr2_lo          ; x-ref: $c2bf
                    bne b_C2C9
                    inc ptr2_hi
b_C2C9              lda block_copy_end_hi ; x-ref: $c2c5
                    cmp ptr_hi
                    bne b_C2B9
                    lda block_copy_end_lo
                    cmp ptr_lo
                    bne b_C2B9
                    rts 
                    ldx #$00
b_C2DA              lda #$0b             ; x-ref: $c2f9
                    sta f_D800,x
                    sta f_D900,x
                    sta f_DA00,x
                    sta f_DB00,x
                    lda #$a0
                    sta screen_row0,x
                    sta f_0500,x
                    sta f_0600,x
                    sta f_0700,x
                    inx 
                    cpx #$00
                    bne b_C2DA
                    rts 
                    nop 
                    nop 
                    nop 
                    nop 
f_C300              .byte $0c, $2d, $51, $66, $91, $c3, $fa, $18 ; x-ref: $c3b5, $caaf
                    .byte $5a, $a3, $cc, $23, $86, $f4, $30, $b4
                    .byte $47            ; Invalid or partial instruction
                    tya 
                    .byte $47            ; Invalid or partial instruction
                    .byte $0c            ; Invalid or partial instruction
                    sbc #$61
                    pla 
                    .byte $8f            ; Invalid or partial instruction
                    bmi b_C2A9
                    clc 
                    .byte $d2, $c3, $d1, $1f, $60, $1e, $31, $a5
                    .byte $87, $a2, $3e, $c1, $3c, $63, $4b, $0f
                    .byte $45, $7d, $83, $79, $c7, $97, $1e, $8b
                    .byte $fa, $06, $f3, $8f, $2e, $10, $c3, $12
                    .byte $d1, $15, $1f, $16, $60
f_C340              .byte $01, $01, $01, $01, $01, $01, $01, $02 ; x-ref: $c3bb, $cab5
                    .byte $02, $02, $02, $03, $03, $03, $04, $04
                    .byte $05, $05, $06, $07, $07, $08, $09, $0a
                    .byte $0b, $0c, $0e, $0f, $10, $12, $15, $16
                    .byte $19, $1c, $1f, $21, $25, $2a, $2c, $32
                    .byte $38, $3f, $43, $4b, $54, $59, $64, $70
                    .byte $7e, $86, $96, $a8, $b3, $c8, $e1, $fd
                    .byte $fd, $fd
s_call_sprite_axis_update
                    tya                  ; x-ref: $ca6c
                    jsr s_update_sprite_coordinates
                    rts 
                    nop 
s_update_title_intro_path
                    ldx a_intro_path_step_ctr
                    inc a_intro_path_step_ctr
                    lda f_sprite_movement_paths,x
                    cmp #$00
                    beq r_C3A5
                    cmp #$ff
                    beq b_C39D
                    cmp #$fe
                    beq b_C39D
                    cmp #$fd
                    beq b_C39D
                    cmp #$fc
                    bne b_C3A9
b_C39D              lda #$00             ; x-ref: $c38f, $c393, $c397
                    sta $ffff            ; IRQ
                    sta $ffff            ; IRQ
r_C3A5              rts                  ; x-ref: $c38b
a_C3A6              .byte $21, $ea, $ea  ; x-ref: $c3c1, $c3d5
b_C3A9              tay                  ; x-ref: $c39b
                    lda #$fe
                    and $ffff            ; IRQ
                    sta $ffff            ; IRQ
                    tya 
                    nop 
                    tax 
                    lda f_C300,x
                    sta $ffff            ; IRQ
                    lda f_C340,x
                    sta $ffff            ; IRQ
                    lda a_C3A6
                    sta $ffff            ; IRQ
                    rts 
                    nop 
                    nop 
                    nop 
