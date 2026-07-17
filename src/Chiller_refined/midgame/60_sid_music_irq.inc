;==============================================================================
; $6001-$61F1 - SID MUSIC IRQ PLAYER AND COMMAND DISPATCHER
;==============================================================================
;
; Parses the packed music command stream using a page-$61 computed jump. Handles
; voice frequency/control commands, ADSR/pulse setup, tempo/wait commands, music
; section counts, IRQ vector install/restore, and SID silence/reset.
;
a_music_note_duration =*+$01
                    lda #$0a
                    sta (ptr_lo),y
                    pla 
                    tay 
                    inx 
                    cpx #$05
                    bne b_map_char_read_done
                    rts 
                    .fill 9, $ea
                    .byte $00, $70, $80, $70, $00, $71, $80, $71
                    .byte $00, $72, $ea, $ea, $ea, $ea, $ea, $ea
                    .byte $40
f_music_data_6001   .byte $03            ; x-ref: $2e80; Invalid or partial instruction
                    .byte $d3            ; Invalid or partial instruction
                    .byte $5b            ; Invalid or partial instruction
                    brk 
                    .byte $ff            ; Invalid or partial instruction
                    .byte $ff            ; Invalid or partial instruction
                    .byte $ff            ; Invalid or partial instruction
b_music_track_done  sbc zpp_45,x         ; x-ref: $600f
                    .byte $ff            ; Invalid or partial instruction
                    .byte $ff            ; Invalid or partial instruction
                    .byte $ff            ; Invalid or partial instruction
                    .byte $f7            ; Invalid or partial instruction
                    .byte $ff            ; Invalid or partial instruction
                    bcc b_music_track_done
b_music_track_next =*+$01
                    bvc b_music_track_next
                    .byte $dc            ; Invalid or partial instruction
                    .byte $4b            ; Invalid or partial instruction
                    brk 
                    brk 
b_music_track_loop =*+$01
                    bne b_music_track_loop
                    .byte $ff            ; Invalid or partial instruction
                    brk 
                    .byte $ff            ; Invalid or partial instruction
                    .byte $d7            ; Invalid or partial instruction
                    .byte $ff            ; Invalid or partial instruction
                    .byte $ff            ; Invalid or partial instruction
                    .byte $ff            ; Invalid or partial instruction
                    .byte $ff            ; Invalid or partial instruction
                    sbc f_unref_ui_glyph_data,x
                    .byte $dc, $ff, $ff, $ff, $ff, $ff, $00, $ff
                    .byte $54, $ff, $f7, $ef, $ff, $ff, $00, $00
                    .byte $f5, $00, $df, $dd, $ff, $f5, $ff, $ff
                    .byte $00, $be, $ff, $ef, $df, $f7, $ff, $9f
                    .byte $ff, $df, $ff, $00, $ff, $ef, $ff, $d0
                    .byte $ff, $ff, $f5, $bf, $04, $7f, $11, $b2
                    .byte $91, $00, $ff, $7f, $00, $ff, $00, $ff
                    .byte $00, $ff, $ca, $50, $ff, $f7, $00, $ff
                    .byte $ff, $00, $ff, $00, $fe, $ff, $00, $f1
                    .byte $ff, $a0, $00, $b7, $fa, $46, $ff, $b5
                    .byte $00, $00, $ff, $00, $be, $ff, $00, $ff
                    .byte $db, $08, $ff, $ff, $c2, $00, $ab, $0a
                    .byte $45, $00, $cb, $00, $ff, $ff, $84, $ba
                    .byte $84, $10, $8f, $00, $ff, $00, $00, $90
                    .byte $81, $ff, $ff, $e5, $6d, $00, $ff, $ff
                    .byte $c0, $14, $00, $15
; Set SID filter cutoff ($D415-$D416=$4505), route all voices through filter
; with resonance $F ($D417=$F1), set volume max+LP+BP mode ($D418=$3F).
music_filter_cutoff lda #$05             ; x-ref: $60e5
                    sta $d415            ; Filter Cutoff Frequency: Low-Nybble
                    lda #$45
                    sta $d416            ; Filter Cutoff Frequency: High-Byte
                    lda #$f1
                    sta $d417            ; Filter Resonance Control / Voice Input Control
                    lda #$3f
                    sta $d418            ; Select Filter Mode and Volume
                    rts 
s_music_sfx_channel_check
                    lda #$03             ; x-ref: $60fd
                    cmp music_state
                    bne b_music_sfx_v1_done
                    ldx a_music_v1_gate_mirror
                    dex 
                    stx $d404            ; Voice 1: Control Register
                    lda #$50
                    sta $d40b            ; Voice 2: Control Register
b_music_sfx_v1_done jmp j_EA31           ; x-ref: $60b9
; Full SID init: clear all SID registers ($D400-$D420),
; set music data pointers ($B7/$B8=$61F8, $B9/$BA=$61F8),
; call sid_filter_vol_init, install music-player IRQ at $0314/$0315=$60F5.
sid_setup           lda #$00             ; x-ref: $2f4d, $7639, $7767
                    sta music_state
                    sta music_section_ctr
                    ldx #$20
b_music_sfx_v2_done sta $d400,x          ; x-ref: $60d7; Voice 1: Frequency Control - Low-Byte
                    dex 
                    bne b_music_sfx_v2_done
                    lda #$f8
                    sta snd_ptr1_lo
                    sta snd_ptr2_lo
                    lda #$61
                    sta snd_ptr1_hi
                    sta snd_ptr2_hi
                    jsr music_filter_cutoff
                    sei 
                    lda #$f5
                    sta irq_vector_lo    ; IRQ
                    lda #$60
                    sta irq_vector_hi    ; IRQ
                    cli 
                    rts 
; SID music IRQ player code (installed as $0314 IRQ handler by sid_setup).
; Processes SID command stream via $B7/$B8 pointer.
;
; Reading tip: music_state is a frame delay counter. When it reaches the command
; dispatcher, the next byte in the stream becomes the low byte of a patched JMP
; on page $61. The table below documents the command byte meanings.
music_irq_handler   ldy #$00
                    cmp music_state
                    beq music_cmd_dispatch
                    dec music_state
                    jmp s_music_sfx_channel_check
; === SID Music Command Dispatch Table (page $61) ===
; The music IRQ handler reads one command byte from the data stream and patches
; the low byte of the JMP at $610B, making it JMP $61_XX. Command bytes:
;   $00 = no-op (re-dispatch, advance to next byte)
;   $0E = set Voice 1 frequency [hi,lo], retrigger
;   $26 = set Voice 2 frequency [hi,lo], retrigger
;   $3E = set both Voice 1+2 frequency [hi1,lo1,hi2,lo2], retrigger
;   $6B = set tempo [1 byte] -> $02AA, end IRQ immediately
;   $74 = end IRQ tick (restore tick counter from $02AA, wait N frames)
;   $7C = increment section counter ($02FF), end IRQ
;   $7F = set voice control bytes [$02AC,$02AD] (gate/waveform), continue
;   $91 = set ADSR for both voices [V1atk/dec, V2atk/dec, V1sus/rel, V2sus/rel]
;   $AC = set pulse width for both voices [V1lo,V1hi,V2lo,V2hi]
;   $C7 = reset voice 1 data pointer to voice 2 position, end IRQ
;   $D2 = stop music (zero all SID regs, restore KERNAL IRQ at $EA31)
music_cmd_dispatch  inc snd_ptr1_lo      ; x-ref: $60f9, $6123, $613b, $6168, $618e, ...
                    bne b_music_cmd_done
                    inc snd_ptr1_hi
b_music_cmd_done    lda (snd_ptr1_lo),y  ; x-ref: $6102
                    sta a_smc_music_cmd_vector
a_smc_music_cmd_vector =*+$01
                    jmp cmd_end_tick
; Cmd $0E: read [freq_hi, freq_lo] -> Voice 1 $D401/$D400, load $02AC into $D404 (retrigger Voice 1).
cmd_v1_freq         jsr music_data_handler ; Cmd $0E: read [freq_hi, freq_lo] -> Voice 1 $D401/$D400, retrigger.
                    sta $d401            ; Voice 1: Frequency Control - High-Byte
                    jsr music_data_handler
                    sta $d400            ; Voice 1: Frequency Control - Low-Byte
                    lda a_music_v1_gate_mirror
                    sty $d404            ; Voice 1: Control Register
                    sta $d404            ; Voice 1: Control Register
                    jmp music_cmd_dispatch
; Cmd $26: read [freq_hi, freq_lo] -> Voice 2 $D408/$D407, load $02AD into $D40B (retrigger Voice 2).
cmd_v2_freq         jsr music_data_handler ; Cmd $26: read [freq_hi, freq_lo] -> Voice 2 $D408/$D407, retrigger.
                    sta $d408            ; Voice 2: Frequency Control - High-Byte
                    jsr music_data_handler
                    sta $d407            ; Voice 2: Frequency Control - Low-Byte
                    lda music_v2_ctrl
                    sty $d40b            ; Voice 2: Control Register
                    sta $d40b            ; Voice 2: Control Register
                    jmp music_cmd_dispatch
; Cmd $3E: read [v1_hi,v1_lo,v2_hi,v2_lo] -> both Voice 1+2 freq+retrigger simultaneously.
cmd_v12_freq        jsr music_data_handler ; Cmd $3E: set both Voice 1+2 frequencies simultaneously.
                    sta $d401            ; Voice 1: Frequency Control - High-Byte
                    jsr music_data_handler
                    sta $d400            ; Voice 1: Frequency Control - Low-Byte
                    jsr music_data_handler
                    sta $d408            ; Voice 2: Frequency Control - High-Byte
                    jsr music_data_handler
                    sta $d407            ; Voice 2: Frequency Control - Low-Byte
                    sty $d404            ; Voice 1: Control Register
                    sty $d40b            ; Voice 2: Control Register
                    lda a_music_v1_gate_mirror
                    sta $d404            ; Voice 1: Control Register
                    lda music_v2_ctrl
                    sta $d40b            ; Voice 2: Control Register
                    jmp music_cmd_dispatch
; Cmd $6B: read [tempo_byte] -> $02AA. Next $74 command will restore this to music_state ($02). Ends IRQ immediately.
cmd_set_tempo       .byte $20, $e9, $61, $8d, $aa, $02, $4c, $31 ; Cmd $6B: read [tempo_byte] -> music_tempo ($02AA). Ends IRQ immediately.
                    .byte $ea
; Cmd $74: restore tick counter from $02AA into music_state ($02), then JMP $EA31 (end IRQ tick). Wait command.
cmd_end_tick        lda music_tempo      ; Cmd $74: restore tick counter from music_tempo -> music_state, JMP kernal_irq (wait command). ; x-ref: $610b
                    sta music_state
                    jmp j_EA31
; Cmd $7C: increment section/pattern counter ($02FF), end IRQ. Tracks which section of the tune is playing.
cmd_inc_section     inc music_section_ctr ; Cmd $7C: increment music_section_ctr ($02FF), end IRQ.
; Cmd $7F: read [v1_ctrl, v2_ctrl] -> $02AC/$02AD (voice gate/waveform control bytes used on next freq command). Continue dispatching.
cmd_set_ctrl        jmp j_EA31
                    jsr music_data_handler
cmd_reset_v1_ptr =*+$02
                    sta a_music_v1_gate_mirror
                    jsr music_data_handler
                    sta music_v2_ctrl
                    jmp music_cmd_dispatch
                    jsr music_data_handler
                    sta $d405            ; Voice 1: Attack / Decay Cycle Control
                    jsr music_data_handler
                    sta $d40c            ; Voice 2: Attack / Decay Cycle Control
                    jsr music_data_handler
                    sta $d406            ; Voice 1: Sustain / Release Cycle Control
                    jsr music_data_handler
                    sta $d40d            ; Voice 2: Sustain / Release Cycle Control
                    jmp music_cmd_dispatch
                    .byte $20, $e9, $61, $8d, $02, $d4, $20, $e9
                    .byte $61, $8d, $03, $d4, $20, $e9, $61, $8d
                    .byte $09, $d4, $20, $e9, $61, $8d, $0a, $d4
                    .byte $4c, $00, $61, $a5, $b9, $85, $b7, $a5
                    .byte $ba, $85, $b8, $4c, $31, $ea
; === Disable stage music and restore KERNAL interrupts ===
; Zeros out the 32 SID hardware registers to silence sound output, disables CPU interrupts, and restores the standard C64 IRQ handler vector ($EA31).
disable_music       lda #$00             ; x-ref: $5dae
                    ldx #$20
b_silence_sid_loop  sta $d400,x          ; x-ref: $61da; Voice 1: Frequency Control - Low-Byte
                    dex 
                    bne b_silence_sid_loop
                    sei                  ; Disable interrupts to safely swap IRQ vector
                    lda #$31
                    sta irq_vector_lo    ; Set standard KERNAL IRQ vector low-byte ($31); IRQ
                    lda #$ea
                    sta irq_vector_hi    ; Set standard KERNAL IRQ vector high-byte ($EA); IRQ
                    cli                  ; Enable interrupts
                    rts 
; === Music Data Stream Read Helper ===
; Increments the 16-bit music data pointer (snd_ptr1_lo/hi) and reads the next byte in the active stream.
music_data_handler  inc snd_ptr1_lo      ; x-ref: $610e, $6114, $6126, $612c, $613e, ...
                    bne b_read_next_byte
                    inc snd_ptr1_hi
b_read_next_byte    lda (snd_ptr1_lo),y  ; x-ref: $61eb
                    rts 
