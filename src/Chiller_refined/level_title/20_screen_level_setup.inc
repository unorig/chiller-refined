;==============================================================================
; $7600-$772A - LEVEL SCREEN SETUP, SCREEN COPY, SPRITE SETUP
;==============================================================================
;
; Waits for music section transitions, initializes the level screen, copies row
; buffers, loads level data, sets sprite pointers/colors, and prepares border /
; sprite-enable state before gameplay or high-score flow.
;
; Initialize screen display: clear screen RAM, draw game border, write title text.
screen_init_display lda #$00             ; x-ref: $7659
                    sta ptr_lo
                    lda #$04
                    sta ptr_hi
                    ldy #$00
b_copy_screen_buffer_loop
                    lda (ptr_lo),y       ; x-ref: $7651
                    sta screen_buffer,y
                    iny 
                    cpy #$50
                    bne b_copy_screen_buffer_loop
                    jmp start_new_level
                    .byte $ea, $ea, $ea
; Load level parameters from level table: set wave count, enemy speed,
; sprite colors for current level.
level_data_load     jsr screen_init_display ; x-ref: $5a41
                    lda #$00
                    sta ptr_lo
                    lda #$02
                    sta ptr_hi
                    ldy #$00
b_copy_row_buffer_loop
                    lda (ptr_lo),y       ; x-ref: $766e
                    sta screen_row0,y
                    iny 
                    cpy #$50
                    bne b_copy_row_buffer_loop
                    rts 
                    .byte $00, $00
s_check_floor_collision
                    cmp #$70             ; x-ref: $5bad
                    bmi r_floor_collision_rts
                    jmp jump_height_check
r_floor_collision_rts
                    rts                  ; x-ref: $7675
                    .byte $00, $00, $00, $00
level_to_load       .byte $08            ; x-ref: $7680, $769a
; === setup_level_sprites === Loads level sprite frame table from f_7FE8, calls copy_data to copy $7FE0 data, then dispatches to high_score_handler for the level.
setup_level_sprites stx level_to_load    ; x-ref: $7f19
                    cpx #$0a
                    bpl b_skip_level_data_copy
                    lda f_7FE8,x
                    sta a_level_ptr_lo
                    lda f_7FE9,x
                    sta a_level_ptr_hi
                    ldx #$e0
                    ldy #$7f
                    jsr s_copy_data
b_skip_level_data_copy
                    ldx level_to_load    ; x-ref: $7685
                    inx 
                    inx 
                    jsr high_score_handler
                    lda var_character_select
                    beq b_set_default_border_color
                    lda sprite_ptr0
                    ldx sprite_ptr1
                    sta sprite_ptr1
                    stx sprite_ptr0
                    lda #$0a
                    sta $d020            ; Border Color
                    jmp check_floor_set_falling
b_set_default_border_color
                    lda #$06             ; x-ref: $76a5
                    sta $d020            ; Border Color
check_floor_set_falling
                    ldy #$73             ; x-ref: $76b8
                    lda #$01
                    sta a_C19B
                    lda (ptr_level_map),y
                    cmp #$0a
                    bmi b_enable_player_only
                    lda #$03
                    sta $d015            ; Sprite display Enable
                    rts 
                    .byte $ea, $ea
b_enable_player_only
                    lda #$01             ; x-ref: $76cb
                    sta $d015            ; Sprite display Enable
                    rts 
s_rope_fall_or_climb_step_full
                    lda #$01
                    jsr s_player_position
                    clc
                    rts
; MOD: when the jump/fall sequencer is active, let UP become a rope-climb step
; only if the player's current screen cell is the rope tile ($62).
s_rope_air_climb_gate
                    lda $dc00
                    lsr
                    bcs b_rope_air_normal
                    lda $d001
                    sec
                    sbc #$2c
                    lsr
                    lsr
                    lsr
                    asl
                    tax
                    lda f_screen_row_offsets,x
                    sta ptr_lo
                    lda f_screen_row_offsets_hi,x
                    ora #$04
                    sta ptr_hi
                    lda $d000
                    sec
                    sbc #$0c
                    bcc b_rope_air_normal
                    lsr
                    lsr
                    lsr
                    tay
                    lda (ptr_lo),y
                    cmp #$62
                    bne b_rope_air_normal
                    jsr s_rope_maybe_move_up
                    rts
b_rope_air_normal   dec a_var_jump_timer
                    lda a_var_jump_timer
                    rts
