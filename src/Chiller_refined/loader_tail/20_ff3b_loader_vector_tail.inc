;==============================================================================
; $FF3B-$FFFF - LOADER/VECTOR OVERLAY TAIL
;==============================================================================
;
; This tail uses KERNAL entry-point addresses as executable loader logic. The
; comments preserve the original KERNAL-name hints while the labels describe
; what the bytes do in Chiller's relocated/decompressed image.

b_FF3B              lda (zp_loader_src_ptr),y ; x-ref: $ff50
                    bne b_FF60
                    jsr KERNAL_RDTIM     ; $FFDE - read jiffy clock
                    lda (zp_loader_src_ptr),y
                    sta (zp_loader_dest_ptr),y
                    jsr s_FFE5
b_FF49              jsr KERNAL_RDTIM     ; x-ref: $ff77; $FFDE - read jiffy clock
b_FF4C              lda zp_loader_src_ptr_hi ; x-ref: $ff9d
                    cmp #$ff
                    bne b_FF3B
                    ldx #$0a
b_FF54              lda f_FFEC,x         ; x-ref: $ff5b
                    sta irq_trampoline,x
                    dex 
                    bpl b_FF54           ; $FF5B (jmp) - init VIC & screen editor
                    jmp irq_trampoline
b_FF60              tax                  ; x-ref: $ff3d
                    dex 
                    bne b_FF79
                    jsr KERNAL_RDTIM     ; $FFDE - read jiffy clock
                    lda (zp_loader_src_ptr),y
                    tax 
b_FF6A              jsr KERNAL_RDTIM     ; x-ref: $ff75; $FFDE - read jiffy clock
b_FF6D              lda (zp_loader_src_ptr),y ; x-ref: $ff9b
                    sta (zp_loader_dest_ptr),y
                    jsr s_FFE5
                    dex 
                    bne b_FF6A
b_FF77              beq b_FF49           ; x-ref: $ffb5
b_FF79              dex                  ; x-ref: $ff62
                    bne KERNAL_SCNKEY    ; $FF9F - scan keyboard
                    jsr KERNAL_RDTIM     ; $FFDE - read jiffy clock
                    lda (zp_loader_src_ptr),y
                    pha                  ; $FF81 - init VIC & screen editor
                    jsr KERNAL_RDTIM     ; $FFDE - read jiffy clock
                    lda (zp_loader_src_ptr),y
                    tax                  ; $FF87 - RAM test & search RAM end
                    jsr KERNAL_RDTIM     ; $FFDE - read jiffy clock
b_FF8B              lda (zp_loader_src_ptr),y ; x-ref: $ff90, $ff97
                    sta (zp_loader_dest_ptr),y ; $FF8D - read/set I/O vectors
                    iny 
                    bne b_FF8B           ; $FF90 - enable/disable KERNAL messages
                    inc zp_loader_src_ptr_hi
                    inc zp_loader_dest_ptr_hi
                    dex                  ; $FF96 - send secondary addr after talk
                    bne b_FF8B
                    pla                  ; $FF99 - read/set top of memory
                    tax 
                    bne b_FF6D
                    beq b_FF4C
KERNAL_SCNKEY       dex                  ; $FF9F - scan keyboard ; x-ref: $ff7a
                    bne KERNAL_READST    ; $FFB7 - read I/O status byte
                    jsr KERNAL_RDTIM     ; $FFA2 - set IEEE timeout; $FFDE - read jiffy clock
                    lda (zp_loader_src_ptr),y ; $FFA5 - input byte from SERIAL
                    tax 
                    jsr KERNAL_RDTIM     ; $FFA8 - output byte to SERIAL; $FFDE - read jiffy clock
                    lda (zp_loader_src_ptr),y ; $FFAB - untalk all SERIAL devices
b_FFAD              sta (zp_loader_dest_ptr),y ; x-ref: $ffb3, $ffda
                    jsr s_FFE5
                    dex 
                    bne b_FFAD
b_FFB5              beq b_FF77           ; x-ref: $ffdc
KERNAL_READST       jsr KERNAL_RDTIM     ; $FFB7 - read I/O status byte ; x-ref: $ffa0; $FFDE - read jiffy clock
                    lda (zp_loader_src_ptr),y ; $FFBA - set file parameters
                    pha 
                    jsr KERNAL_RDTIM     ; $FFBD - set file name; $FFDE - read jiffy clock
                    lda (zp_loader_src_ptr),y ; $FFC0 - open log.file after SETLFS,SETNAM
                    tax 
                    jsr KERNAL_RDTIM     ; $FFC3 - close a logical file; $FFDE - read jiffy clock
                    lda (zp_loader_src_ptr),y ; $FFC6 - open channel for input
b_FFC8              sta (zp_loader_dest_ptr),y ; x-ref: $ffcb, $ffd0
                    iny 
                    bne b_FFC8
                    inc zp_loader_dest_ptr_hi
                    dex                  ; $FFCF - input character
                    bne b_FFC8
KERNAL_CHROUT       tay                  ; $FFD2 - output character ; x-ref: $2cd6, $55e5, $5cda, $5cf7, $c654
                    pla 
                    tax 
                    tya                  ; $FFD5 - load after call SETLFS,SETNAM
                    ldy #$00
KERNAL_SAVE         cpx #$00             ; $FFD8 - save after call SETLFS,SETNAM ; x-ref: $5ded
                    bne b_FFAD
                    beq b_FFB5
KERNAL_RDTIM        inc zp_loader_src_ptr ; $FFDE - read jiffy clock ; x-ref: $ff3f, $ff49, $ff64, $ff6a, $ff7c, ...
                    bne KERNAL_GETIN     ; $FFE4 - get a byte from channel
                    inc zp_loader_src_ptr_hi
KERNAL_GETIN        rts                  ; $FFE4 - get a byte from channel ; x-ref: $ffe0
s_FFE5              inc zp_loader_dest_ptr ; x-ref: $ff46, $ff71, $ffaf
                    bne r_FFEB           ; $FFE7 - close or abort all files
                    inc zp_loader_dest_ptr_hi
r_FFEB              rts                  ; x-ref: $ffe7
f_FFEC              .byte $a9, $37, $85, $01 ; x-ref: $ff54
KERNAL_PLOT         cli                  ; $FFF0 - read/set cursor position ; x-ref: $5ceb
                    jsr s_A659
                    jmp j_A7AE
                    .byte $01, $cf, $16, $08, $40, $00, $9e, $32
                    .byte $30            ; IRQ ; x-ref: $2cdd, $2cfa, $2f1b, $2f2d, $2f7d, ...
