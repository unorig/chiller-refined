s_enemy_sprite_anim_dispatcher
                    tax                  ; x-ref: $ca2f
                    lda f_sprite_active_status,x
                    cmp #$01
                    beq b_route_active_enemy
                    jmp s_sprite_anim_frame_tick
b_route_active_enemy
                    jmp s_tick_enemy_path_delay ; x-ref: $cdef
                    nop 
                    nop 
                    nop 
s_unref_check_enemy_active
                    lda f_sprite_active_status,x ; x-ref: $cddb
                    cmp #$01
f_sfx_envelope_data_ce00 =*+$01
                    bne b_CDAA
                    .byte $7f            ; Invalid or partial instruction
                    .byte $03            ; Invalid or partial instruction
                    php 
                    .byte $ef            ; Invalid or partial instruction
                    ora (zpp_10,x)
                    eor zpf_7F,x
                    .byte $7f            ; Invalid or partial instruction
                    .byte $7f            ; Invalid or partial instruction
                    brk 
                    .byte $f7            ; Invalid or partial instruction
                    .byte $f7            ; Invalid or partial instruction
                    .byte $f7            ; Invalid or partial instruction
                    .byte $ff            ; Invalid or partial instruction
                    bpl s_sfx_trigger
                    bpl b_CE44
                    bpl b_CE46
                    bpl b_CE1B
                    sec 
                    brk 
                    brk 
b_CE1B              .byte $02, $03, $0f, $00, $01, $10, $ab, $af ; x-ref: $ce16
                    .byte $bf, $ff, $ff, $ef, $ef, $ef, $ea, $fa
                    .byte $fe, $ff, $ff, $ef, $ef, $ef, $03, $08
                    .byte $00, $01, $04, $fd, $f7, $df, $7f, $03
                    .byte $04, $ff, $01, $1a, $00, $03, $07
s_sfx_trigger       .byte $07            ; x-ref: $59df, $5af6, $ce10; Invalid or partial instruction
                    .byte $03            ; Invalid or partial instruction
b_CE44              .byte $7c, $fe       ; x-ref: $ce12
b_CE46              inc f_E31C,x         ; x-ref: $ce14
                    .byte $f7            ; Invalid or partial instruction
                    .byte $f7            ; Invalid or partial instruction
                    .byte $83            ; Invalid or partial instruction
                    .byte $7c            ; Invalid or partial instruction
                    inc @w ptr2_hi,x
                    cpx #$f0
                    beq b_CDD4
                    .byte $7c, $fe, $fe, $a8, $80, $03, $04, $6a
                    .byte $01, $04, $aa, $aa, $aa, $25, $03, $04
                    .byte $95, $01, $05, $a5, $af, $00, $00, $c0
                    .byte $03, $05
s_sfx_helper_1      beq b_CE71           ; x-ref: $2fa3
                    .byte $03            ; Invalid or partial instruction
b_CE71              brk                  ; x-ref: $ce6e
                    brk 
                    .byte $03            ; Invalid or partial instruction
                    .byte $03            ; Invalid or partial instruction
                    ora zpa_0F
                    .byte $03            ; Invalid or partial instruction
                    php 
                    brk 
                    ora (zp_sound_param_ptr,x)
                    .byte $f3            ; Invalid or partial instruction
                    cpy #$2a
                    .byte $03            ; Invalid or partial instruction
                    ora zpa_AA
                    ora (zp_sound_param_ptr,x)
                    .byte $fc, $fc, $32
s_sfx_helper_2      .byte $03            ; x-ref: $ca3f, $cd9a; Invalid or partial instruction
                    ora zpa_8A
                    ora (zp_sound_param_ptr,x)
                    .byte $ff            ; Invalid or partial instruction
                    .byte $3f            ; Invalid or partial instruction
                    sty cia1_dd_port_a
                    ldx #$01
                    .byte $03            ; Invalid or partial instruction
                    .byte $cf            ; Invalid or partial instruction
                    .byte $0f            ; Invalid or partial instruction
                    .byte $a3            ; Invalid or partial instruction
                    .byte $03            ; Invalid or partial instruction
                    ora zpa_A8
                    .byte $03            ; Invalid or partial instruction
                    bpl b_CE9D
b_CE9D              ora (zpa_06,x)       ; x-ref: $ce9b
                    .byte $93, $83, $8f, $92, $85, $a0, $03, $06
                    .byte $b0, $01, $16, $a0, $93, $89, $8c, $96
                    .byte $85, $92, $a0, $83, $92, $8f, $93, $93
                    .byte $85, $93, $a0, $b0, $b0, $a0, $88, $89
                    .byte $a0, $03, $06, $b0, $01, $06, $85, $8e
                    .byte $85, $92, $87, $99, $03, $21, $a9, $01
                    .byte $0b, $a1, $62, $00, $00, $00, $62, $00
                    .byte $00, $62, $1a
j_CEDA              .byte $1b            ; x-ref: $cafc; Invalid or partial instruction
                    .byte $03            ; Invalid or partial instruction
                    .byte $14            ; Invalid or partial instruction
                    brk 
                    ora (zpp_16,x)
                    and (zpp_22,x)
                    .byte $62, $00, $00, $62, $00, $00, $00, $62
                    .byte $62, $00, $00, $00
s_CEEE              .byte $62, $00, $00, $62, $00, $62, $1a, $1b ; x-ref: $cc98
j_CEF6              .byte $03            ; x-ref: $c40d; Invalid or partial instruction
                    bpl b_CEF9
b_CEF9              ora (zpp_1A,x)       ; x-ref: $cef7
                    and (zpp_22,x)
                    .byte $62, $00
a_CEFF              .byte $62            ; x-ref: $c3f8, $c3fe, $c401, $c69b, $c6a1, ...
; Level configuration vector pointer, copied from $0800 tables during initialization.
