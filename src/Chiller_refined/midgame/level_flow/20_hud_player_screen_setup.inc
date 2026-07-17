f_hud_stage_text_forest
                    .byte $8d, $81, $93, $94, $85, $92, $94, $92 ; x-ref: $5c71
                    .byte $8f, $8e, $89, $83, $a0, $93, $0f, $12
                    .byte $1c, $8e, $08, $54, $48, $45, $20, $46
                    .byte $4f, $52, $45, $53, $54, $01, $00, $00
a_player_x_sprite_target =*+$01
update_player_sprite_pos
                    ldx #$00             ; x-ref: $5e4b
a_player_y_sprite_target =*+$01
                    ldy #$b0
                    stx ptr_lo
                    sty ptr_hi
                    lda #$00
                    sta ptr2_lo
                    lda #$d8
                    sta ptr2_hi
b_player_x_sprite_done
                    ldy #$00             ; x-ref: $5d53, $5d59
                    lda (ptr_lo),y
                    sta (ptr2_lo),y
                    iny 
                    lsr a
                    lsr a
                    lsr a
                    lsr a
                    sta (ptr2_lo),y
                    inc ptr_lo
                    bne b_player_y_sprite_done
                    inc ptr_hi
b_player_y_sprite_done
                    inc ptr2_lo          ; x-ref: $5d3f
                    bne b_player_sprite_pos_done
                    inc ptr2_hi
b_player_sprite_pos_done
                    inc ptr2_lo          ; x-ref: $5d45
                    bne b_player_sprite_pos_active
                    inc ptr2_hi
b_player_sprite_pos_active
                    lda ptr2_hi          ; x-ref: $5d4b
                    cmp #$db
                    bne b_player_x_sprite_done
                    lda ptr2_lo
                    cmp #$f0
                    bne b_player_x_sprite_done
                    rts 
                    .byte $00, $00, $00, $00
; Check player sprite 0 overlap with all active enemy sprites;
; if collision trigger player_death_handler.
draw_player_on_screen
                    lda #$01             ; x-ref: $c649
                    sta var_character_select
                    jsr game_state_smc_patch
                    rts 
s_rope_fall_or_climb_step
                    .byte $4c, $db, $76 ; JMP s_rope_fall_or_climb_step_full ($76DB)
                    .fill 20, $00
s_enable_multicolor_and_disable_kernal
                    lda $d016            ; x-ref: $5a6a; VIC Control Register 2
                    ora #$10
                    sta $d016            ; VIC Control Register 2
                    lda #$0b
                    sta $d023            ; Background Color 2, Multi-Color Register 1
                    lda #$01
                    sta $d024            ; Background Color 3
                    jsr disable_kernal_rom
                    jmp b_init_collision_limits
; Per-frame update for all active game objects: move, animate, and collision-check each object slot.
game_object_tick    jsr s_CFC0           ; x-ref: $2d85
                    lda #$10
                    sta snd_ptr1_lo
                    sta snd_ptr2_lo
                    lda #$6b
                    sta snd_ptr1_hi
                    sta snd_ptr2_hi
                    rts 
                    .byte $00
; === level_end_handler === Waits for music_section_ctr != 0, then calls disable_music, clears damage counters, returns to main game loop.
