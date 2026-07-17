;==============================================================================
; $5350-$54FF - PLAYER FALL / JUMP CHECKS AND SID VOICE 3 SFX
;==============================================================================
;
; Handles player Y-position snapshots, fall/return easing, landing/stall
; detection, and the falling-pitch SID voice 3 sound effect.
;
a_player_prev_y     .byte $e3            ; x-ref: $5366, $5373
a_player_fall_dist  .byte $0c            ; x-ref: $535d
a_fall_check_ticks  .byte $03, $ea       ; x-ref: $5355, $5360
a_fall_step_counter .byte $00, $ea       ; x-ref: $2af1, $5381, $53e9, $53f7, $5408, ...
b_player_fall_check lda a_level_config_reg_4 ; x-ref: $5394
                    beq r_fall_rts
                    dec a_fall_check_ticks
                    beq b_player_y_changed
r_fall_rts          rts                  ; x-ref: $5353
                    .byte $ea, $ea
b_player_y_changed  lda a_player_fall_dist ; x-ref: $5358
                    sta a_fall_check_ticks
                    lda $d001            ; Sprite 0 Y Pos
                    sta a_player_prev_y
                    ldx #$00
                    jsr s_rope_fall_or_climb_step
                    bcs r_fall_rts
                    lda $d001            ; Sprite 0 Y Pos
                    cmp a_player_prev_y
                    bne b_player_descending
                    jmp sfx_sweep_init
                    .byte $ea, $ea, $ea
; MOD: silence the jump/fall sweep. Keep instruction width stable.
b_player_descending jsr s_set_fall_jump_sprite_base ; x-ref: $5376
                    inc a_fall_step_counter
                    lda #$01
                    sta a_col_active_left ; LEFT collision active flag.
                    sta a_var_on_rope    ; Rope contact flag: non-zero = player is on a rope tile.
                    rts 
                    .byte $ea
; Per-frame update for Sprite 0's two-phase fall/return animation.
; Increments the global tick counter, then bails early if sprite movement is
; inactive (a_C19C=0). Decrements the step-delay timer (a_2A7F); when it
; reaches zero, advances the easing phase (step index 0->$18 out, $18->0 return)
; using delays from table a_544A, checking sprite/char collision and Y-stall.
; Once the step index rewinds to 0, clears a_C19C (animation complete).
; 
; Inputs:  a_C19C (movement-active flag), a_2A7F (step delay timer),
;          a_2A7E (step index into easing table a_544A), a_C19B (phase: 0=out, 1=return)
; Outputs: a_2A7F (updated timer), a_2A7E (updated step index)
; Side Effects: Increments a_CF02; calls check_sprite0_char_collision, s_5467
sprite0_fall_step_update
                    inc a_global_tick_counter ; x-ref: $7fae
                    lda a_C19C
                    beq b_player_fall_check
undefined           jsr s_rope_air_climb_gate ; Jump step delay timer: loaded from easing table l_544A,X; counts down before advancing step index.
                    beq b_fall_advance_tick
                    rts 
                    .byte $ea, $ea
b_fall_advance_tick jsr sprite0_snapshot_y_ret_anim_flag ; x-ref: $5399
                    bne begin_fall
                    lda #$00
                    jsr s_player_position
                    jsr sprite0_check_y_stall_advance_step
                    ldx a_var_jump_phase ; Jump phase: 0=ascending/peak, 1=returning to ground. Controls easing table direction.
                    cpx #$12
                    bne b_fall_continue_descent
                    jmp fall_phase_flip
b_fall_continue_descent
                    lda f_player_jump_easing_table,x ; x-ref: $53b0
                    sta a_var_jump_timer ; Jump step delay timer: loaded from easing table l_544A,X; counts down before advancing step index.
                    jmp fall_continue_down
fall_phase_flip     lda #$01             ; x-ref: $53b2
                    sta a_C19B
                    jmp begin_fall
; Begin player fall animation: set falling/jumping flags, initialize easing step index.
begin_fall          lda #$01             ; x-ref: $53a1
                    jsr sprite0_check_collision_clear_stall_if_y_stable
                    dec a_var_jump_phase ; Jump phase: 0=ascending/peak, 1=returning to ground. Controls easing table direction.
                    ldx a_var_jump_phase ; Jump phase: 0=ascending/peak, 1=returning to ground. Controls easing table direction.
                    cpx #$00
                    bne b_fall_continue_ascent
                    jmp fall_phase_done
b_fall_continue_ascent
                    lda f_player_jump_easing_table,x ; x-ref: $53d3
                    sta a_var_jump_timer ; Jump step delay timer: loaded from easing table l_544A,X; counts down before advancing step index.
                    jmp fall_continue_up
fall_phase_done     lda #$00             ; x-ref: $53d5
                    sta a_C19C
                    nop 
                    nop 
                    nop 
                    lda #$18
                    sta a_fall_step_counter
                    rts 
fall_easing_step    lda #$01             ; x-ref: $54a8
                    sta a_col_active_left ; LEFT collision active flag.
                    sta a_var_on_rope    ; Rope contact flag: non-zero = player is on a rope tile.
                    lda a_fall_step_counter
                    cmp a_level_anim_config
                    bpl b_fall_easing_done
b_fall_easing_done  nop                  ; x-ref: $53fd
                    nop 
                    nop 
                    nop 
                    lda #$00
                    sta a_double_jump_state
                    sta a_fall_step_counter
                    rts 
                    nop 
                    lda #$00
                    sta a_fall_step_counter
                    jsr s_halt_irq_and_reset_sprites
                    rts 
                    .byte $ea, $ea
fall_anim_check     lda a_fall_step_counter ; x-ref: $c1b0
                    bne b_fall_check_active
                    lda #$01
                    sta a_C19C
                    jmp s_reset_player_jump_phase
b_fall_check_active jmp b_check_movement_delay ; x-ref: $541b
                    .byte $ea, $ea
a_player_jump_vel   .byte $e3            ; x-ref: $542e, $543a
; Saves Sprite 0's current Y position as a pre-collision snapshot and returns
; the animation-complete flag in A.
; 
; Inputs:  $D001 (Sprite 0 Y position), a_C19B (animation-complete flag)
; Outputs: A = a_C19B (0 = animating, 1 = complete); a_542A = snapshot of Y pos
; Side Effects: None
sprite0_snapshot_y_ret_anim_flag
                    lda $d001            ; x-ref: $539e; Sprite 0 Y Pos
                    sta a_player_jump_vel
                    lda a_C19B
                    rts 
                    .byte $ea, $ea
; Checks whether Sprite 0's Y position has stalled (not changed since the last
; collision snapshot). If Y unchanged, sets animation-complete flag ($C19B) = 1,
; signalling sprite has landed. Regardless, increments fall-animation step counter.
; 
; Inputs:  $D001 (Y position), a_542A (saved Y snapshot), a_2A7E (step index)
; Outputs: a_C19B set to 1 if Y unchanged; a_2A7E incremented unconditionally
; Side Effects: None
sprite0_check_y_stall_advance_step
                    lda $d001            ; x-ref: $53a8; Sprite 0 Y Pos
                    cmp a_player_jump_vel
                    bne b_player_y_moving
                    lda #$01
                    sta a_C19B
b_player_y_moving   inc a_var_jump_phase ; x-ref: $543d; Jump phase: 0=ascending/peak, 1=returning to ground. Controls easing table direction.
                    rts 
                    nop 
                    nop 
f_player_jump_easing_table
                    .byte $05, $05, $06, $06, $08, $08, $0a, $0a ; x-ref: $2a75, $53b5, $53d8, $c185
                    .byte $0b, $0b, $0d, $0d, $0e, $0e, $10, $10
                    .byte $12, $12, $13, $13, $15, $16, $18, $1a
                    .byte $2d, $ea, $ea, $ea
a_collision_check_temp
                    .byte $e3            ; x-ref: $546a, $5472
; Snapshots Sprite 0's Y position, performs char collision check (mode 1),
; then verifies Y hasn't moved. If stable, clears Y-stall flag (a_C19C).
; If Y changed during the check, returns early leaving a_C19C untouched.
; 
; Inputs:  $D001 (Sprite 0 Y position)
; Outputs: a_C19C cleared to $00 when Y position is stable
; Side Effects: Calls check_sprite0_char_collision
sprite0_check_collision_clear_stall_if_y_stable
                    lda $d001            ; x-ref: $53c8; Sprite 0 Y Pos
                    sta a_collision_check_temp
                    lda #$01
                    jsr s_player_position
                    lda a_collision_check_temp
                    cmp $d001            ; Sprite 0 Y Pos
                    beq b_player_landed_stable
                    rts 
b_player_landed_stable
                    lda #$00             ; x-ref: $5478
                    sta a_C19C
                    rts 
                    .byte $ea, $ea
; Trigger a new note on SID Voice 3. Clears the gate bit to stop any current
; note, programs the envelope (Attack/Decay=$0D, Sustain/Release=$00), then
; re-gates Voice 3 with the waveform/control byte from a_45B7 (triangle+gate).
; Falls through to j_54EA which resets the frequency sliding counters.
; 
; Inputs:  a_45B7 = waveform+control byte for Voice 3 ($11 = triangle + gate on)
; Outputs: SID $D412/$D413/$D414 (Voice 3 ctrl/envelope); a_54F9/a_54FA (freq state)
; Side Effects: Jumps to j_C819 (sequencer frame tick) after resetting freq counters
sid_v3_note_on      lda #$00             ; x-ref: $54b9, $58b0
                    sta $d412            ; Voice 3: Control Register
                    lda #$0d
                    sta $d413            ; Voice 3: Attack / Decay Cycle Control
                    lda #$00
                    sta $d414            ; Voice 3: Sustain / Release Cycle Control
                    lda f_enemy_enable_flags
                    sta $d412            ; Voice 3: Control Register
                    lda #$00
                    nop 
                    nop 
                    jmp sfx_sweep_continue
a_sfx_sweep_pitch   .byte $00            ; x-ref: $54a5, $54ac, $54b6
sfx_sweep_init      lda #$00             ; x-ref: $5378
                    sta $d412            ; Voice 3: Control Register
                    sta a_sfx_sweep_pitch
                    jmp fall_easing_step
                    .byte $ea
; SID Voice 3 downward frequency sweep effect driver.
; Called once per game tick. On the first call (a_549F=0) initialises Voice 3
; (ADSR/waveform via s_5483, initial frequency $8080) and marks the channel active.
; On every subsequent call it subtracts $80 from the 16-bit frequency counter
; a_54BF:a_54C0 and writes the new value to $D40E/$D40F, producing a falling-pitch sweep.
; 
; Inputs:  a_549F (Voice 3 active flag), a_54BF/a_54C0 (freq hi/lo working registers)
; Outputs: $D40E/$D40F (SID Voice 3 frequency registers updated each tick)
; Side Effects: On first call: Voice 3 ADSR/waveform initialised, active flag set to 1.
sfx_voice3_sweep    lda a_sfx_sweep_pitch ; x-ref: $537e
                    beq b_sfx_sweep_init
b_sfx_sweep_init =*+$01
                    jmp sfx_sweep_tick
                    lda #$01
                    sta a_sfx_sweep_pitch
                    jsr sid_v3_note_on
                    jmp sfx_sweep_freq_init
a_sfx_freq_step_lo  .byte $73            ; x-ref: $54cf, $54d2, $54de
a_sfx_freq_step_hi  .byte $80            ; x-ref: $54c1, $54c7, $54e1
sfx_sweep_tick      lda a_sfx_freq_step_hi ; x-ref: $54b1
                    sec 
                    sbc #$80
                    sta a_sfx_freq_step_hi
                    sta $d40e            ; Voice 3: Frequency Control - Low-Byte
                    bcs b_sfx_no_borrow
                    dec a_sfx_freq_step_lo
b_sfx_no_borrow     lda a_sfx_freq_step_lo ; x-ref: $54cd
                    sta $d40f            ; Voice 3: Frequency Control - High-Byte
                    rts 
                    .byte $ea, $ea, $ea
sfx_sweep_freq_init lda #$80             ; x-ref: $54bc
                    sta a_sfx_freq_step_lo
                    sta a_sfx_freq_step_hi
                    rts 
