;==============================================================================
; $C8F0-$CEFF - COORDINATES, FRAME DISPATCH, ENEMY PATHS, PROJECTILES, MIXED CE
;==============================================================================
;
; Contains the generic VIC sprite coordinate updater, watermark/checksum-style
; logic, per-frame dispatcher, enemy/projectile update loop, enemy wave/path
; initialization, animation managers, projectile logic, and the ambiguous
; $CE00-range mixed code/data area.
;
; === Zero-page music player state saver ===
; Saves zero-page address pointer parameters for music commands before execution.
                    brk 
                    .byte $dc, $29, $10, $c9, $10, $f0, $05, $a9
                    .byte $01, $8d, $05, $5a, $60
a_target_dir        .byte $03            ; x-ref: $c902, $c90c
a_target_sprite_slot
                    .byte $05            ; x-ref: $c905, $c941, $c95b, $c971, $c98b
; === VIC Sprite Coordinate Updater ===
; Updates coordinate registers for the requested sprite index X based on direction vector A ($00=up, $01=down, $02=left, $03=right) and handles $D010 X-MSB toggle.
s_update_sprite_coordinates
                    ldy #$ff             ; x-ref: $2a5c, $55d2, $c37b, $c708, $c849, ...
                    sta a_target_dir
                    stx a_target_sprite_slot
                    txa 
                    clc 
                    asl a
                    tax 
                    lda a_target_dir
                    cmp #$00
                    beq b_update_up
                    cmp #$01
                    beq b_update_down
                    cmp #$02
                    beq b_update_left
                    cmp #$03
                    beq b_update_right
                    rts 
b_update_up         txa                  ; x-ref: $c911; Sprite 0 Y Pos
                    bne b_update_up_move
                    jsr s_grounded_up_move_gate
                    bne r_C92C
b_update_up_move
                    dec $d001,x          ; Sprite 0 Y Pos
                    iny
r_C92C              rts                  ; x-ref: $c925
b_update_down       lda $d001,x          ; x-ref: $c915; Sprite 0 Y Pos
                    cmp #$e3
                    beq r_C939
                    inc $d001,x          ; Sprite 0 Y Pos
                    ldy #$00
r_C939              rts                  ; x-ref: $c932
b_update_left       lda $d000,x          ; x-ref: $c919; Sprite 0 X Pos
                    cmp #$08
                    bne b_left_decrement
                    ldy a_target_sprite_slot
                    lda f_sprite_msb_set_masks,y
                    and $d010            ; Sprites 0-7 MSB of X coordinate
                    cmp #$00
                    bne b_left_decrement
                    ldy #$ff
                    rts 
b_left_decrement    dec $d000,x          ; x-ref: $c93f, $c94c; Sprite 0 X Pos
                    lda $d000,x          ; Sprite 0 X Pos
                    cmp #$ff
                    bne b_left_done
                    ldy a_target_sprite_slot
                    lda f_sprite_msb_clear_masks,y
                    and $d010            ; Sprites 0-7 MSB of X coordinate
                    sta $d010            ; Sprites 0-7 MSB of X coordinate
b_left_done         ldy #$00             ; x-ref: $c959
                    rts 
b_update_right      lda $d000,x          ; x-ref: $c91d; Sprite 0 X Pos
                    cmp #$4f
                    bne b_right_increment
                    ldy a_target_sprite_slot
                    lda f_sprite_msb_set_masks,y
                    and $d010            ; Sprites 0-7 MSB of X coordinate
                    cmp #$00
                    beq b_right_increment
                    ldy #$ff
                    rts 
b_right_increment   inc $d000,x          ; x-ref: $c96f, $c97c; Sprite 0 X Pos
                    lda $d000,x          ; Sprite 0 X Pos
                    cmp #$00
                    bne b_right_done
                    ldy a_target_sprite_slot
                    lda f_sprite_msb_set_masks,y
                    ora $d010            ; Sprites 0-7 MSB of X coordinate
                    sta $d010            ; Sprites 0-7 MSB of X coordinate
b_right_done        ldy #$00             ; x-ref: $c989
                    rts 
                    nop 
                    nop 
                    nop 
                    inc $d014            ; Latch Y Pos
                    ldx a_A914
                    brk 
                    sta a_joystick_x_limit
b_C9A7              lda #$00             ; x-ref: $c9be
                    sta a_joystick_y_limit
b_C9AC              inc a_joystick_y_limit ; x-ref: $c9b4
                    lda a_joystick_y_limit
                    cmp #$ff
                    bne b_C9AC
                    inc a_joystick_x_limit
                    lda a_joystick_x_limit
                    cmp #$01
                    bne b_C9A7
                    rts 
                    nop 
                    nop 
                    nop 
; === Joystick port input wrapper ===
; Reads port 2 input and tests if player is standing still to clear direction registers.
s_check_joystick_idle
                    lda $dc00            ; x-ref: $c8b7; CIA1: Data Port Register A
                    and #$0f
                    cmp #$0f
                    beq b_joystick_idle_done
                    lda zp_joystick_port2_state
                    cmp #$40
                    bne b_joystick_idle_done
                    lda #$00
                    sta a_input_delta_y
                    sta a_input_delta_x
b_joystick_idle_done
                    lda $dc00            ; x-ref: $c9cb, $c9d1; CIA1: Data Port Register A
                    rts 
                    nop 
                    nop 
f_sprite_msb_clear_masks
                    inc f_FBFD,x         ; x-ref: $c95e
                    .byte $f7            ; Invalid or partial instruction
                    .byte $ef            ; Invalid or partial instruction
                    .byte $df            ; Invalid or partial instruction
                    .byte $bf            ; Invalid or partial instruction
                    .byte $7f            ; Invalid or partial instruction
f_sprite_msb_set_masks
                    ora (music_state,x)  ; x-ref: $c944, $c974, $c98e
                    .byte $04, $08, $10, $20, $40, $80
; === Copy protection signature watermark checker ===
; Increments the watermark check counter and hashes the signature keys using the sprite overlap table and the current joystick input mask to verify code integrity.
