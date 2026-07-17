a_sprite_frame_update_index
                    .byte $03            ; x-ref: $cc12, $cc19, $cc1f, $cc57, $cc6c, ...
; === Active Sprite Animation Frame Manager ===
; Evaluates individual active frame-rate speed counters to increment animation offsets.
; Reads the low/high table parameters to dynamically assign new frame pointers.
s_update_active_sprite_frame
                    sta a_sprite_frame_update_index ; x-ref: $ccbc
                    tax 
                    lda f_sprite_anim_frame_pointers,x
                    ldx a_sprite_frame_update_index
                    jmp j_CC81
s_advance_sprite_anim_counter
                    ldx a_sprite_frame_update_index ; x-ref: $cca4
                    inc f_sprite_anim_frame_counters,x
                    lda f_sprite_anim_frame_counters,x
                    cmp #$08
                    beq b_sprite_anim_frame_advance
                    rts 
b_sprite_anim_frame_advance
                    lda #$00             ; x-ref: $cc2a
                    sta f_sprite_anim_frame_counters,x
                    lda a_enemy_anim_loop_frame,x
                    cmp #$00
                    beq b_sprite_anim_standard_loop
                    jmp s_sprite_anim_custom_loop
b_sprite_anim_standard_loop
                    lda f_enemy_anim_table_lo,x ; x-ref: $cc37
                    sta ptr_lo
                    lda f_sprite_anim_table_hi,x
                    sta ptr_hi
                    lda f_sprite_anim_step_index,x
                    tay 
                    inc f_sprite_anim_step_index,x
                    lda (ptr_lo),y
                    nop 
                    nop 
                    cmp #$ff
                    bne b_sprite_anim_store_frame
                    lda #$01
                    ldx a_sprite_frame_update_index
                    sta f_sprite_anim_step_index,x
                    ldy #$00
                    lda (ptr_lo),y
b_sprite_anim_store_frame
                    sta f_sprite_anim_frame_pointers,x ; x-ref: $cc53
                    rts 
s_sprite_anim_custom_loop
                    jsr s_increment_watermark ; x-ref: $cc39
                    tya 
                    and #$3f
                    tay 
                    ldx a_sprite_frame_update_index
                    lda f_enemy_anim_table_lo,x
                    sta ptr_lo
                    lda f_sprite_anim_table_hi,x
                    sta ptr_hi
                    lda (ptr_lo),y
                    sta f_sprite_anim_frame_pointers,x
                    rts 
                    .byte $ea, $ea
j_CC81              tay                  ; x-ref: $cc1c
                    ldx a_sprite_frame_update_index
                    inx 
                    inx 
                    jsr s_update_sprite_motion_if_active
                    cpy #$ff
                    bne b_enemy_sequence_tick
                    ldx a_sprite_frame_update_index
                    lda a_enemy_anim_end_frame,x
                    cmp #$00
                    bne b_enemy_sequence_tick
                    jsr s_CEEE
                    eor #$ff
                    and $d015            ; Sprite display Enable
                    sta $d015            ; Sprite display Enable
                    rts 
b_enemy_sequence_tick
                    jmp s_advance_sprite_anim_counter ; x-ref: $cc8c, $cc96
                    .byte $ea, $ea, $ea, $aa
; === Enemy Sprite Frame Sequence Looper ===
; Ticks active duration counters and resets frame indexes once limits are reached.
; Loops enemy sprite hardware pointer indexes ($07FA-$07FE) to cycle walking/attacking frames.
s_sprite_anim_frame_tick
                    inc f_sprite_anim_duration_counters,x ; x-ref: $cdf1
                    lda f_sprite_anim_duration_counters,x
                    cmp f_enemy_anim_max_ticks,x
                    bne b_CCBF
                    lda #$00
                    sta f_sprite_anim_duration_counters,x
                    txa 
                    jsr s_update_active_sprite_frame
b_CCBF              inc f_enemy_anim_tick_counters,x ; x-ref: $ccb4
                    lda f_enemy_anim_tick_counters,x
                    cmp #$10
                    bne r_anim_tick_rts
                    lda #$00
                    sta f_enemy_anim_tick_counters,x
                    ldx a_CEFF
                    inc f_sprite_anim_ticks,x
                    lda f_sprite_anim_ticks,x
                    cmp f_enemy_speed_configs,x
                    bne r_anim_tick_rts
                    lda #$00
                    sta f_sprite_anim_ticks,x
                    lda enemy_sprite_ptrs,x
                    inc enemy_sprite_ptrs,x
                    cmp f_sprite_anim_end_frames,x
                    bne r_anim_tick_rts
                    lda f_sprite_anim_loop_frames,x
                    sta enemy_sprite_ptrs,x
r_anim_tick_rts     rts                  ; x-ref: $ccc7, $ccda, $ccea
                    .byte $ea, $ea
; === Player Projectile Spawning/Update Handler ===
; Validates if hardware Sprite 7 (player projectile) is active, checks range limit thresholds, and dispatches the projectile spawner if the fire button is pressed.
