;==============================================================================
; $C000-$C147 - SCREEN CLEAR, SPRITE OVERLAP, COLLISION MASK SETUP
;==============================================================================
;
; Contains the entry point reached by sprite overlap checks, the main screen and
; color RAM clear helpers, the character/background collision mask generator,
; and the vertical enemy sprite activator.
;
;    MAIN GAMEPLAY CORE OBJECTS AND COLLISION LOGIC
; ====================================================
;
; The $C000 page is the dense gameplay core. It contains screen clear helpers,
; sprite/background collision masks, player input decoding, block-copy SMC,
; row scrollers, sprite flicker, enemy pathing, projectile handling, and the
; $CFxx live state tables.
;
; This region is both the most useful to read and the riskiest to rearrange.
; It contains:
;   - self-modifying operands, e.g. a_C29E/a_C2A2 and similar =*+$01 labels
;   - small data bytes embedded between routines
;   - live state tables around $CF00
;   - some areas still marked as invalid/partial instructions
;
; Treat each routine as movable only after confirming there are no direct
; address dependencies, no fall-through assumptions, and no copied-code use.
;
s_sprite_overlap_handler
                    lda f_sprite_enable_bits,x ; x-ref: $c9f7, $cd14
                    and $d015            ; Sprite display Enable
                    beq b_sprite_overlap_done
                    nop 
                    nop 
                    nop 
                    jmp s_dispatch_sprite_spawn_or_update
b_sprite_overlap_done
                    jmp s_tick_projectile_step ; x-ref: $c006
s_update_sprite_animation
                    lda f_anim_timer_config ; x-ref: $ca51
                    cmp #$ff
                    beq r_C031
                    dec a_sprite_anim_global_timer
                    jsr s_player_position_tick
                    bne r_C031
                    lda f_anim_timer_config
                    sta a_sprite_anim_global_timer
                    lda $d016            ; VIC Control Register 2
                    and #$f7
                    sta $d016            ; VIC Control Register 2
                    jsr s_scroller_raster_sync
r_C031              rts                  ; x-ref: $c016, $c01e
s_set_player_y      lda a_player_y       ; x-ref: $c6ac; Player sprite 0 saved Y position. Written by set_sprite0_y.
                    jmp s_restore_player_sprite_pos
                    lda #$90
; === Screen clear logic ===
; Fills character RAM screen rows ($0400-$07E7) with empty spaces and sets corresponding Color RAM rows ($D800-$DBE7) to Light Gray ($0B).
s_clear_screen_and_color
                    sta ptr_lo
                    lda #$04
                    sta ptr_hi
                    lda #$00
                    ldy #$00
b_clear_page_loop   ldx #$00             ; x-ref: $c065
b_clear_row_loop    jsr s_draw_char_light_gray ; x-ref: $c056
                    jsr s_next_screen_row
                    ldy #$00
                    inc ptr_lo
                    bne b_clear_page_boundary
                    inc ptr_hi
b_clear_page_boundary
                    cpx #$07             ; x-ref: $c050
                    bne b_clear_row_loop
b_clear_col_loop    inc ptr_lo           ; x-ref: $c061
                    bne b_clear_col_step
                    inc ptr_hi
b_clear_col_step    inx                  ; x-ref: $c05a
                    cpx #$28
                    bne b_clear_col_loop
                    cmp #$7e
                    bne b_clear_page_loop
                    rts 
; === Active character/background mask collision checker ===
; Reads tile configuration parameters from active sprite registers and compares them with screen layout blocks to register player collision flags.
s_check_char_collision
                    jsr s_clear_sprite_collision_table ; x-ref: $526c
                    ldy #$00
                    lda #$78
                    sta ptr_lo
                    lda #$04
                    sta ptr_hi
                    ldx #$00
b_collision_cell_loop
                    lda f_sprite_bitmask_table,y ; x-ref: $c086, $c09c
a_C07B =*+$01
a_C07C =*+$02
                    and f_zero_init_buf_30A0,x
                    bne b_C083
                    lda #$a0
                    sta (ptr_lo),y
b_C083              iny                  ; x-ref: $c07d
                    cpy #$08
                    bne b_collision_cell_loop
                    ldy #$00
                    inx 
                    lda #$28
b_collision_row_advance
                    inc ptr_lo           ; x-ref: $c098
                    bne b_collision_row_step
                    inc ptr_hi
b_collision_row_step
                    clc                  ; x-ref: $c08f
                    sbc #$00
                    cmp #$00
                    bne b_collision_row_advance
                    cpx #$08
                    bne b_collision_cell_loop
                    rts 
                    nop 
                    nop 
                    nop 
s_quick_clear_screen_char_ram
                    lda #$00
                    sta ptr_lo
                    lda #$04
                    sta ptr_hi
                    ldy #$00
b_quick_clear_screen_loop
                    lda #$a0             ; x-ref: $c0ba, $c0c0
                    sta (ptr_lo),y
                    inc ptr_lo
                    bne b_quick_clear_screen_step
                    inc ptr_hi
b_quick_clear_screen_step
                    lda ptr_lo           ; x-ref: $c0b2
                    cmp #$e8
                    bne b_quick_clear_screen_loop
                    lda ptr_hi
                    cmp #$07
                    bne b_quick_clear_screen_loop
s_quick_clear_color_ram
                    lda #$00
                    sta ptr_lo
                    lda #$d8
                    sta ptr_hi
                    ldy #$00
b_quick_clear_color_loop
                    lda #$0b             ; x-ref: $c0da, $c0e0
                    sta (ptr_lo),y
                    inc ptr_lo
                    bne b_quick_clear_color_step
                    inc ptr_hi
b_quick_clear_color_step
                    lda ptr_lo           ; x-ref: $c0d2
                    cmp #$e8
                    bne b_quick_clear_color_loop
                    lda ptr_hi
                    cmp #$db
                    bne b_quick_clear_color_loop
                    rts 
a_C0E3              .byte $01            ; x-ref: $5662, $c0ea
; === VIC vertical enemy sprite activator ===
; Updates slot position arrays for vertical enemy waves, configures sprite priorities, and updates VIC sprite display pointers.
s_activate_vertical_enemies
                    jsr update_vertical_enemies ; x-ref: $2fe5, $c69e
                    jsr s_initialize_enemy_sprite
                    lda a_C0E3
                    tax 
                    lda f_sprite_enable_bits,x
                    ora $d015            ; Sprite display Enable
                    sta $d015            ; Sprite display Enable
                    lda a_level_status_param_68,x
                    sta enemy_sprite_ptrs,x
                    jmp s_update_enemy_colour
f_sprite_bitmask_table
                    .byte $80, $40, $20, $10, $08, $04, $02, $01 ; x-ref: $c077
s_clear_sprite_collision_table
                    ldy #$00             ; x-ref: $c068
                    lda #$78
                    sta ptr_lo
                    lda #$04
                    sta ptr_hi
                    ldx #$00
b_clear_collision_cell_loop
                    lda #$80             ; x-ref: $c11b, $c131
                    sta (ptr_lo),y
                    iny 
                    cpy #$08
                    bne b_clear_collision_cell_loop
                    ldy #$00
                    inx 
                    lda #$28
b_clear_collision_row_advance
                    inc ptr_lo           ; x-ref: $c12d
                    bne b_clear_collision_row_step
                    inc ptr_hi
b_clear_collision_row_step
                    clc                  ; x-ref: $c124
                    sbc #$00
                    cmp #$00
                    bne b_clear_collision_row_advance
                    cpx #$08
                    bne b_clear_collision_cell_loop
                    rts 
                    brk 
                    brk 
                    brk 
                    rts 
                    .byte $00, $a9, $00, $9d, $00, $d4, $e8, $e0
                    .byte $18, $d0, $f8, $60
