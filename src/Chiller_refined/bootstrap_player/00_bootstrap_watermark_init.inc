;==============================================================================
; $0801-$0AFF - BASIC STUB, BOOTSTRAP, ANTISOFT WATERMARK, INIT SEEDS
;==============================================================================
;
; Starts execution at s_start, copies embedded tables into $CE00/$CF00, initializes
; VIC-II registers, and stores the ANTISOFT watermark/signature data.
;
;==============================================================================
; $0801-$0AFF - BOOTSTRAP, COPY-PROTECTION WATERMARK, INITIAL STATE SEEDS
;==============================================================================
;
; The BASIC stub starts execution at s_start. The bootstrap copies two embedded
; byte tables into $CE00 and $CF00, then initializes VIC-II registers. The long
; ANTISOFT text is used as a visible copy-protection watermark/signature and is
; also referenced later by integrity-style logic.
;
; Keep this region in address order. Several tables here are not just passive
; data: they seed live code/state pages that the game executes later.
;
* = $0801
                    .byte $16, $08, $40, $00, $9e, $32, $30, $37
                    .byte $32, $20, $28, $41, $4e, $54, $49, $53
                    .byte $4f, $46, $54, $29, $00, $00, $00
; === Chiller ===
; C64 platformer. Play as Boy or Girl character through 4 maps.
; Collect items (mushrooms, red flowers, baskets) and magic crosses.
; Manage health bar. Slide on ropes. Avoid or jump on enemies.
; Bridges erode under the player. Game-over when health = 0.
s_start             ldx #$00
b_StartCopyData     lda f_initial_ce_page_vars,x ; x-ref: $0827
                    sta f_sfx_envelope_data_ce00,x
                    lda f_initial_cf_page_vars,x
                    sta f_level_data_ptr_cf00,x
                    inx 
                    bne b_StartCopyData
b_SetVic2Values     lda f_initial_vic_regs,x ; x-ref: $0832
                    sta $d000,x          ; Sprite 0 X Pos
                    inx 
                    cpx #$30
                    bne b_SetVic2Values
                    jmp j_game_entry
                    .encode 
                    .enc "none"
; ANTISOFT copy protection watermark: 'ANTISOFT' ASCII repeated ~89x filling $0837-$0AFF.
                    .text "ANTISOFTANTISOFT"
a_watermark_sprite0_backup
                    .text "ANTISOFTANTISOFTANTISOFTANTISOFTANTISOFT" ; x-ref: $55ba
                    .text "ANTISOFTANTISOFTANTISOFTANTISOFTANTISOFT"
                    .text "ANTISOFTANTISOFTANTISOFTANTISOFTANTISOFT"
                    .text "ANTISOFTANTISOFT,"
f_initial_vic_regs  .text $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $1b, $04, $d1, $00, $00, $c8, $00, $15, "y", $f0, $00, $00, $00, $00, $00, $f6, $f6, $f1, $f2, $f3, $f4, $f0, $f1 ; x-ref: $0829
                    .text $f2, $f3, $f4, $f5, $f6, $f7, $fc, $ff
f_initial_ce_page_vars
                    .text $dd, "`", $9d, "O", $cf, $a9, $fd, "-", $15, $d0, $8d, $15, $d0, "Lk", $ce, "`", $a2, $06, $fe, $05, $04, $bd, $05, $04, $c9, $ba, $d0, $0a, $a9, $b0, $9d, $05, $04, $ca, $e0, $00, $d0, $ec, "`" ; x-ref: $081a
                    .text $a2, $06, $de, $05, $04, $bd, $05, $04, $c9, $af, $d0, $0d, $a9, $b9, $9d, $05, $04, $ca, $e0, $00, $d0, $ec, " ", $11, $ce, "`", $ad, "\", $cf, $8d, "S", $ce, $ad, "Z", $cf, $c9, $00, $f0, $09, $ce
                    .text "Z", $cf, " ", $11, $ce, "LH", $ce, $ad, "[", $cf, $c9, $00, $f0, $0b, $ce, "[", $cf, $a9, $ff, $8d, "Z", $cf, "LR", $ce, "`", $ae, "N", $cf, $bd, "`E", $8d, "Z", $cf, $bd, "eE", $8d
                    .text "[", $cf, $bd, "jE", $8d, "\", $cf, " B", $ce, "`", $ea, $ea, $ea, $ad, $1e, $d0, $85, $fd, ")", $01, $d0, $03, "L", $9d, $cd, $a5, $fd, ")", $02, $f0, $01, "`", $a2, $00, $a0, $00, $bd, "F"
                    .text $cf, "%", $fd, $f0, $01, $c8, $e8, $e0, $06, $d0, $f3, $c0, $01, $f0, $01, "`", $a5, $fd, ")", $fc, $8d, "^", $cf, $a2, $00, $c9, $04, $f0, $05, $e8, "JL", $b9, $ce, $8e, "_", $cf, $a2, $02, " "
                    .text "D[L", $9d, $cd, "oE)", $01, $d0, $04, "`L{/L", $a7, "/ ", $ed, $ca, $ce, $1a, $04, $ad, $1a, $04, $c9, $b0, $f0, $01, "`", $a9, $01, $8d, "}", $cf, "`", $fe, "v"
                    .text $cf, $bd, $08, $cf, "`", $ea, $8d, "'", $d0, $ad, $ec, "EL", $ed, $ca, $05
f_initial_cf_page_vars
                    .text $00, $ea, $7f, $06, $00, $06, $00, $00, $04, $08, $10, " @", $00, $05, $0a, $0f, $14, $0f, $01, $a9, $03, $00, $03, $03, $02, $07, $02, $04, $00, $06, $ea, $ea, $ea, " ", $11, "JJJJ" ; x-ref: $0820
                    .text "J", $01, $01, $01, $01, $01, $1c, $01, $09, $01, $09, $03, $03, $01, $01, $00, $a3, $a7, $97, $93, ";", $a0, $a4, $94, $90, "8", $0d, "<", $02, $09, $04, $08, $10, " @", $80, "I", $10, $02, $00
                    .text $00, $00, $00, $00, "@", $08, $04, $08, $04, $0a, $00, $00, $11, "H ", $03, $0b, $00, $ea, $04, $ea, $ff, $00, $00, $a0, $a4, $94, $90, "8", $ba, $c1, $c8, $cf, $d6, $ea, $00, $00, $c1, $c6, $c6
                    .text $14, $00, $c6, "@", $b2, $00, $02, $00, $ea, $00, $0a, $0a, $0a, $0a, $0a, $0f, $00, $01, $00, $00, $00, $01, $00, $00, $01, $00, $01, $00, $00, $00, $00, $02, $01, $05, $04, $08, $1f, "(", $10, $08
                    .text " ", $8c, $85, $96, $85, $8c, $a0, $b0, $b0, $b1, $87, $81, $8d, $85, $a0, $8f, $96, $85, $92, $ec, $ec, $ec, $ed, $ec, $ec, $ec, $ed, $ec, $ed, $ed, $ed, $ed
; === Sub_GameOverText ===
; Draws game-over text: copies 9 bytes from $CFA1 (colour) -> $D838,X (screen row $D8xx colour area),
; and from $CFAA -> screen RAM $0438,X.
; ($0B00 area below contains enemy sprite bitmaps.)
s_game_over_text    .text " 3W", $a2, $00, $bd, $a1, $cf, $bd, $17, $06, $ad, $1a, $d8, $a9, $01, $ea, $9d, "8", $d8, $bd, $aa, $cf, $9d, "8", $04, $e8, $e0, $09, $d0, $e6, "`3W", $00, $00, $ee, $ec, $ed, $ee
                    .text $ed, $ed, $ed, $ed, $ed, $ed, $ed, $ed, $ee, $f2, $f2, $f2, "2", $f1, $f1, $f1, " ", $f1, $f1, $f0, $f1, $ef, $ef, $01
                    .endencode 
;==============================================================================
; $0B00-$0FFF - ENEMY SPRITE BITMAP FRAMES
;==============================================================================
;
