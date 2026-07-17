;==============================================================================
; $61F2-$72FF - LEVEL CONFIG POINTERS, BANK COPY, DAMAGE FLASH, TITLE ROW
;==============================================================================
;
; Includes the external music/level/text block, then handles level-config pointer
; lookup, all-RAM block copying, damage border flash timing, and top-row title
; character/color setup.
;
.binary "../../chiller_refined_asset_61f2_727f_music_level_data.bin"
; ====================================================
;    LEVEL CONFIGURATION AND SCREEN INTRO TEXT
; ====================================================
;
; Level setup code and data live together here. The pointer table chooses the
; active level config, bankswitch copy routines pull level assets into RAM, and
; the intro/title text is interleaved with music/control data.
;
; The .binary block immediately above contains the music stream and nearby
; level/text material. It is intentionally externalized to keep this source
; manageable.
;
s_check_level_end_transition
                    ldy #$73             ; x-ref: $5768
                    lda (ptr_level_map),y
                    cmp #$0a
                    bmi r_level_end_rts
                    jmp game_state_smc_patch
r_level_end_rts     rts                  ; x-ref: $7286
                    brk 
                    brk 
                    brk 
                    brk 
; Level config pointer table (10 lo/hi address pairs, indexed by LevelToLoad x 1):
;   [0]->$7000 Cinema  [2]->$7080 Ghetto  [4]->$7100 Graveyard
;   [6]->$7180 Haunted House  [8]->$7200 Forest
level_config_ptr_hi =*+$01
level_config_ptr_lo .word level_cinema_config, level_ghetto_config, level_graveyard_config, level_haunted_house_config, level_forest_config ; x-ref: $5e19
                    .byte $00, $73, $80, $73, $00, $74, $80, $74
                    .byte $00, $75
; Disable IRQs, switch to all-RAM ($01=$34), call copy_data to copy a data block,
; restore KERNAL ($01=$36), enable IRQs.
bankswitch_block_copy
                    sei                  ; x-ref: $5e2e
                    lda cpu_io_dir
                    ora #$02
                    sta cpu_io_dir
                    lda #$34
                    sta cpu_io_port
                    jsr s_copy_data
                    lda #$36
                    sta cpu_io_port
                    cli 
                    rts 
                    .byte $ea
var_damage_loop_ctr .byte $04            ; x-ref: $72bd, $72c4
; Decrement 4-frame counter (a_72B9); when zero, wait for raster $10 and toggle
; border color bit (XOR #$08). Also calls reduce_health_bar (score counter update) each frame.
damage_routine      jsr reduce_health_bar ; x-ref: $5b3e
                    dec var_damage_loop_ctr
                    bne r_damage_flash_rts
                    lda #$04
                    sta var_damage_loop_ctr
b_damage_raster_wait
                    lda $d012            ; x-ref: $72cc, $72d3; Raster Position
                    cmp #$10
                    bne b_damage_raster_wait
                    lda $d011            ; VIC Control Register 1
                    and #$80
                    bne b_damage_raster_wait
                    lda $d020            ; Border Color
                    eor #$08
                    sta $d020            ; Border Color
r_damage_flash_rts  rts                  ; x-ref: $72c0
                    .byte $00, $00
; Place marker char $AC at $04BA with color $02, then copy 40 bytes from data
; table f_77C0 to screen-RAM row $0400 with color 4 ($D800).
set_mastertronic_apostrophe
                    lda #$ac             ; x-ref: $5c81
                    sta screen_mastertronic_char
                    lda #$02
                    sta a_color_mastertronic_char
                    jmp set_title_screen_top_row
set_top_chars       ldx #$00             ; x-ref: $75e9
b_top_chars_copy_loop
                    lda f_title_top_row_text,x ; x-ref: $72fd
                    sta screen_row0,x
                    lda #$04
                    sta f_D800,x
                    inx 
                    cpx #$28
                    bne b_top_chars_copy_loop
                    rts 
