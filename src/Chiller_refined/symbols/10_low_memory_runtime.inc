;==============================================================================
; LOW-MEMORY RUNTIME STATE AND SCREEN RAM ALIASES
;==============================================================================
;
; $0200-$07FF contains screen buffer aliases, C64 vectors/trampolines, temporary
; animation state, sprite pointers, and a few state cells whose exact role still
; needs proving.

screen_buffer = $0200                    ; x-ref: $764b
music_v1_ctrl = $028d                    ; x-ref: $2e74, $c1ef, $c8ab
music_tempo = $02aa                      ; x-ref: $6174
a_music_v1_gate_mirror = $02ac           ; x-ref: $60bb, $611a, $615c, $6185
music_v2_ctrl = $02ad                    ; x-ref: $6132, $6162, $618b
music_section_ctr = $02ff                ; x-ref: $5da9, $60ce, $617c, $7610
irq_vector_lo = $0314                    ; IRQ ; x-ref: $2cb7, $60eb, $61df
irq_vector_hi = $0315                    ; IRQ ; x-ref: $2cbc, $60f0, $61e4
stop_key_check_byte = $0328              ; $0328 (ind) - check stop key ; x-ref: $75a9
irq_trampoline = $0334                   ; x-ref: $ff57, $ff5d
a_033C = $033c                           ; x-ref: $c843
a_033D = $033d                           ; x-ref: $c846
f_0345 = $0345                           ; x-ref: $2c4e, $2c90
f_034E = $034e                           ; x-ref: $2c54, $2c96
a_intro_path_step_ctr = $0380            ; x-ref: $5604, $c380, $c383, $c3cb
a_intro_sprite_x = $0381                 ; x-ref: $5232, $523c, $524b, $52a8, $52af, ...
a_intro_sprite_y = $0382                 ; x-ref: $5237, $5241, $524e, $5303, $5319
a_intro_sprite_xmsb = $0383              ; x-ref: $522b, $5244, $5247, $5255
a_0384 = $0384                           ; x-ref: $2b0a, $2b7d, $2b80, $2ed2, $52e8, ...
a_0385 = $0385                           ; x-ref: $2b14, $2b66, $2b69
a_0386 = $0386                           ; x-ref: $2b0f, $2b1e, $2b63
a_0387 = $0387                           ; x-ref: $2b19, $2b27, $2b4b, $2b5c
a_0388 = $0388                           ; x-ref: $2b24, $2b2d
block_copy_end_lo = $0390                ; x-ref: $c2af, $c2d0
block_copy_end_hi = $0391                ; x-ref: $c2b4, $c2c9
screen_row0 = $0400                      ; x-ref: $72f2, $7668, $c2ea
hiscore_screen_ref = $0406               ; x-ref: $7780, $7797
magic_cross_left_digit = $041c           ; x-ref: $5b6c, $5b78, $5b83
magic_cross_right_digit = $041d          ; x-ref: $5b5f, $5b66, $5b75, $5b80
screen_row1_right_save = $0428           ; x-ref: $c418, $c423
screen_row1 = $0429                      ; x-ref: $c420
f_042E = $042e                           ; x-ref: $599b, $59a6, $59ad, $59ba, $59ca
a_0430 = $0430                           ; x-ref: $5961
scroll_corner_save = $044f               ; x-ref: $c4c7
screen_mastertronic_char = $04ba         ; x-ref: $72e2
f_04D6 = $04d6                           ; x-ref: $5c74
screen_row3 = $04f9                      ; x-ref: $c436
f_04FA = $04fa                           ; x-ref: $c433
f_0500 = $0500                           ; x-ref: $c2ed
cia1_dd_port_a = $0503                   ; x-ref: $ce8e
a_0524 = $0524                           ; x-ref: $5200
f_053C = $053c                           ; x-ref: $c17a
f_05EE = $05ee                           ; x-ref: $c174
irq_handler_addrs = $05ef                ; x-ref: $2c4b, $2c5a, $2c93
f_05F3 = $05f3                           ; x-ref: $c449
f_05F4 = $05f4                           ; x-ref: $c446
f_0600 = $0600                           ; x-ref: $c2f0
screen_row5_misc = $0617
screen_row6_end = $06ed                  ; x-ref: $c45c
f_06EE = $06ee                           ; x-ref: $c177, $c459
f_0700 = $0700                           ; x-ref: $c2f3
sprite_ptr_config = $07ca                ; x-ref: $573b
sprite_ptr0 = $07f8                      ; x-ref: $55b4, $55dc, $560e, $567d, $5844, ...
sprite_ptr1 = $07f9                      ; x-ref: $2d8d, $5611, $5913, $5957, $595d, ...
enemy_sprite_ptrs = $07fa                ; x-ref: $2f16, $c0fa, $c625, $c628, $cce1, ...
sprite_ptr5 = $07fd
border_misc = $07ff                      ; x-ref: $c6bb
