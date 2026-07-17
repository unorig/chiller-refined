;==============================================================================
; $7300-$75FF - MENU COLOR TOGGLES, STOP-KEY PATCH, BORDER ROUTING
;==============================================================================
;
; Includes the external $7300-$757F block, then controls title/menu background
; color changes, patches the STOP-key vector, routes border color by game state,
; and initializes magic-cross display state.
;
.binary "../../chiller_refined_asset_7300_757f_menu_irq.bin"
; Read CIA joystick byte ($C9F1), compare with $40; toggle $D022 or $D023
; (background color registers) based on result.
bg_color_toggle     jsr s_increment_watermark ; x-ref: $776d
                    cpy #$00
                    bne r_bg_color_toggle_rts
                    jsr s_increment_watermark
                    cpy #$40
                    bpl b_toggle_bg_color2
                    lda $d022            ; Background Color 1, Multi-Color Register 0
                    eor #$02
                    sta $d022            ; Background Color 1, Multi-Color Register 0
                    jmp r_bg_color_toggle_rts
b_toggle_bg_color2  lda $d023            ; x-ref: $758c; Background Color 2, Multi-Color Register 1
                    eor #$02
                    sta a_shadow_bg_color2
                    jmp r_bg_color_toggle_rts
r_bg_color_toggle_rts
                    rts                  ; x-ref: $7585, $7596, $75a1
                    .byte $00, $00
; Patch stop-key check vector ($0328) to NOP ($EA), then JMP to main game entry at $5A60.
patch_stop_key_jmp_main
                    lda #$ea             ; x-ref: $2dfa
                    sta stop_key_check_byte ; $0328 (ind) - check stop key
                    jmp main_game_start
                    .byte $00
; Read game flags (a_5A11, a_5A08); if a_5A11!=0 set border orange ($D020=$0A);
; else if a_5A08!=0 set border blue ($D020=6); then continue to game_loop_tick.
damage_border_colour
                    lda queued_damage_effect_count ; x-ref: $ca00
                    bne b_border_done
                    lda var_character_select
                    bne b_border_light_red
a_runtime_default_border_color =*+$01
                    lda #$06
                    sta $d020            ; Border Color
                    jmp game_loop_tick
b_border_light_red  lda #$0a             ; x-ref: $75b8
                    sta $d020            ; Border Color
b_border_done       jmp game_loop_tick   ; x-ref: $75b3
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
set_title_screen_top_row
                    lda a_DA5C           ; x-ref: $72ea
                    and #$0f
                    cmp #$01
                    bne r_set_top_row_rts
                    jmp set_top_chars
r_set_top_row_rts   rts                  ; x-ref: $75e7
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
s_init_level_crosses
                    iny                  ; x-ref: $5fbc
                    lda (ptr_level_map),y
                    sta var_magic_crosses
                    lda #$03
                    sta $d015            ; Sprite display Enable
                    rts 
                    .byte $00, $00, $00, $00
; === level_screen_setup === Waits for music_section_ctr != 0 (end of music section), handles background color toggle from joystick, then calls sid_setup to restart music.
level_screen_setup  lda music_section_ctr ; x-ref: $5bf1, $761a, $762b, $7636
                    bne b_music_wait_done
                    jsr s_increment_watermark
                    cpy #$00
                    bne level_screen_setup
                    jsr s_increment_watermark
                    cpy #$40
                    bpl b_flicker_color2
                    lda $d022            ; Background Color 1, Multi-Color Register 0
                    eor #$02
                    sta $d022            ; Background Color 1, Multi-Color Register 0
                    jmp level_screen_setup
b_flicker_color2    lda $d023            ; x-ref: $7621; Background Color 2, Multi-Color Register 1
                    eor #$02
                    sta $d023            ; Background Color 2, Multi-Color Register 1
                    jmp level_screen_setup
b_music_wait_done   jsr sid_setup        ; x-ref: $7613
                    rts 
                    .byte $00, $00
