;==============================================================================
; $C600-$C8EF - ENEMY TIMERS, HUD DRAW, PHYSICS DISPATCH, JOYSTICK CHECKS
;==============================================================================
;
; Handles enemy respawn timing, HUD drawing, color updates, delay loops,
; Sprite 1 coordinate handling, player physics dispatch, and joystick idle /
; movement boundary checks.
;
; === Enemy respawn/logic loop timer ===
; Decrements logic loop frames, updates active enemy movement indices, and sweeps sprite visibility configuration.
s_tick_enemy_path_delay
                    dec f_enemy_move_delay_counters,x ; x-ref: $cdf4
                    lda f_enemy_move_delay_counters,x
                    cmp #$00
                    bne r_enemy_delay_rts
                    lda #$0a
                    sta f_enemy_move_delay_counters,x
                    nop 
                    nop 
                    nop 
                    nop 
                    nop 
                    dec f_sprite_movement_delays,x
                    lda f_sprite_movement_delays,x
                    cmp #$00
                    bne r_enemy_delay_rts
                    inc enemy_sprite_ptrs,x
                    lda enemy_sprite_ptrs,x
                    cmp #$fc
                    bne b_reset_enemy_movement_delay
                    lda f_sprite_enable_bits,x
                    eor #$ff
                    and $d015            ; Sprite display Enable
                    sta $d015            ; Sprite display Enable
                    lda #$00
                    sta f_sprite_active_status,x
b_reset_enemy_movement_delay
                    lda f_enemy_init_movement_delay,x ; x-ref: $c62d
                    sta f_sprite_movement_delays,x
r_enemy_delay_rts   rts                  ; x-ref: $c60f, $c623
s_reset_cutscene_girl
                    jsr reset_girl       ; x-ref: $2dd3, $2ee9
; === Draw player on screen stats handler ===
; Sets screen color configuration, prints stats parameters via KERNAL CHROUT, and calls the high score comparator.
s_draw_player_hud   jsr draw_player_on_screen ; x-ref: $c4d0
                    lda a_scroll_enable_flag
                    sta $d021            ; Background Color 0
                    lda #$08
                    jsr KERNAL_CHROUT    ; $FFD2 - output character
                    nop 
                    lda #$00
                    sta $d015            ; Sprite display Enable
                    nop 
                    nop 
                    nop 
                    ldx #$00
                    jsr high_score_handler
                    nop 
                    nop 
                    nop 
                    nop 
                    nop 
                    nop 
                    nop 
                    nop 
                    nop 
                    nop 
                    nop 
                    nop 
                    nop 
                    ldx #$c0
                    ldy #$56
                    jsr s_copy_data
                    jmp j_C694
s_grounded_up_move_gate
                    lda #$00
                    rts
s_rope_maybe_move_up
                    lda a_global_tick_counter
                    and #$7f
                    bne b_rope_move_skip
                    dec $d001
b_rope_move_skip    lda #$01
                    rts
                    .fill 8, $ea
j_C694              lda f_enemy_respawn_counts ; x-ref: $2ca1, $2eee, $2ef6, $ca4e
                    bne b_hud_setup_done
                    lda #$00
                    sta a_CEFF
b_activate_enemies_loop
                    jsr s_activate_vertical_enemies ; x-ref: $c6aa
                    inc a_CEFF
                    lda a_CEFF
                    cmp a_enemy_wave_max_limit
                    bne b_activate_enemies_loop
b_hud_setup_done    jsr s_set_player_y   ; x-ref: $c697
                    lda #$1c
                    sta $d018            ; VIC Memory Control Register
                    lda #$0f
                    sta $ffff            ; IRQ
                    lda #$f8
                    sta border_misc
                    lda a_sprite1_init_color
                    sta $d028            ; Sprite 1 Color
                    ldx #$00
                    nop 
                    lda #$00
b_reset_enemy_arrays_loop
                    sta f_sprite_anim_frame_counters,x ; x-ref: $c6d8
                    sta f_sprite_anim_step_index,x
                    sta f_sprite_anim_duration_counters,x
                    sta f_sprite_active_status,x
                    inx 
                    cpx #$05
                    bne b_reset_enemy_arrays_loop
                    lda $d01e            ; Sprite to Sprite Collision Detect
                    jmp s_init_enemy_counts
s_update_enemy_colour
                    lda a_enemy_sprite_config,x ; x-ref: $c0fd
                    jmp s_set_enemy_colour
                    .byte $60
s_delay_wait_loop   tya                  ; x-ref: $2db3, $559a, $5645, $cac5
                    pha 
                    txa 
                    pha 
b_delay_outer_loop  lda a_delay_loop_param_lo ; x-ref: $c6f9
                    sta a_delay_loop_working_ctr
b_delay_inner_loop  dec a_delay_loop_working_ctr ; x-ref: $c6f4
                    bne b_delay_inner_loop
                    dec a_delay_loop_param_hi
                    bne b_delay_outer_loop
                    pla 
                    tax 
                    pla 
                    tay 
                    rts 
s_update_sprite1_position
                    lda sprite_ptr1      ; x-ref: $ca0f
                    clc 
                    sbc #$df
                    ldx #$01
                    jsr s_update_sprite_coordinates
                    cpy #$00
                    beq b_sprite1_limit_done
                    rts 
                    .byte $ea, $ea, $ea, $ea, $ea, $ea, $ea
b_sprite1_limit_done
                    nop                  ; x-ref: $c70d
                    rts 
                    .byte $ea, $ea
; === Player physics / collision dispatcher ===
; Checks joystick/movement states against screen row cell boundary parameters to handle player jumping, falling, and rope slides.
s_player_physics_dispatch
                    lda #$00             ; x-ref: $ca21
                    sta a_physics_collision_flags
                    jsr s_airborne_horizontal_input_wrapper
                    lda a_input_delta_y
                    cmp #$ff
                    bne b_C74C
                    nop
                    nop
                    nop
                    lda a_col_active_up  ; UP collision flag.
                    beq b_C74C
                    lda #$00
                    tax 
                    jsr s_player_position
                    inc a_physics_collision_flags
                    lda a_col_frame_up
                    beq b_C74C
                    nop
                    lda sprite_ptr0
                    and #$01
                    clc 
                    adc #$d8
                    sta sprite_ptr0
b_C74C              lda a_input_delta_y  ; x-ref: $c728, $c72f, $c73f
                    cmp #$01
                    bne b_C776
                    lda a_col_active_down ; DOWN collision active flag.
                    cmp #$00
                    beq b_C776
                    lda #$01
                    ldx #$00
                    jsr s_player_position
                    inc a_physics_collision_flags
                    lda a_col_frame_down
                    cmp #$00
                    beq b_C776
                    lda sprite_ptr0
                    and #$01
                    clc 
                    adc #$da
                    sta sprite_ptr0
b_C776              lda a_input_delta_x  ; x-ref: $c751, $c758, $c769
                    cmp #$ff
                    bne b_C7A0
                    lda a_col_active_left ; LEFT collision active flag.
                    cmp #$00
                    beq b_C7A0
                    lda #$02
                    ldx #$00
                    jsr s_player_position
                    inc a_physics_collision_flags
                    lda a_col_frame_left
                    cmp #$00
                    beq b_C7A0
                    lda sprite_ptr0
                    and #$03
                    clc 
a_C79C =*+$01
                    adc #$e8
                    jsr s_store_horizontal_sprite_maybe_airborne
b_C7A0              lda a_input_delta_x  ; x-ref: $c77b, $c782, $c793
                    cmp #$01
                    bne b_C7CA
                    lda a_var_on_rope    ; Rope contact flag: non-zero = player is on a rope tile.
                    cmp #$00
                    beq b_C7CA
                    lda #$03
                    ldx #$00
                    jsr s_player_position
                    inc a_physics_collision_flags
                    lda a_col_frame_right
                    cmp #$00
                    beq b_C7CA
                    lda sprite_ptr0
                    and #$03
                    clc 
a_C7C6 =*+$01
                    adc #$ec
                    jsr s_store_horizontal_sprite_maybe_airborne
b_C7CA              lda a_input_fire_button ; x-ref: $c7a5, $c7ac, $c7bd
                    cmp #$00
                    beq b_check_movement_delay
                    jmp s_eval_level_movement_type
s_spawn_player_attack_flash
                    and #$02             ; x-ref: $c1a5
                    cmp #$00
                    bne b_check_movement_delay
                    lda sprite_ptr0
                    clc 
                    sbc #$d7
                    lsr a
                    tax 
                    lda f_dir_index_table,x
                    cmp #$ff
                    beq b_check_movement_delay
                    clc 
                    adc #$e0
                    jsr s_init_sprite
                    lda #$02
                    ora $d015            ; Sprite display Enable
                    sta $d015            ; Sprite display Enable
                    lda $d000            ; Sprite 0 X Pos
                    sta $d002            ; Sprite 1 X Pos
                    lda $d001            ; Sprite 0 Y Pos
                    sta $d003            ; Sprite 1 Y Pos
                    lda $d010            ; Sprites 0-7 MSB of X coordinate
                    and #$01
                    tay 
                    lda $d010            ; Sprites 0-7 MSB of X coordinate
                    and #$fd
                    sta $d010            ; Sprites 0-7 MSB of X coordinate
                    tya 
                    asl a
                    ora $d010            ; Sprites 0-7 MSB of X coordinate
                    jsr s_set_xmsb_retrigger_v3
b_check_movement_delay
                    lda a_movement_timer_active ; x-ref: $5425, $54f4, $c1ad, $c7cf, $c7d8, ...
                    cmp #$01
                    beq b_apply_movement_delay
                    lda a_physics_collision_flags
                    cmp #$00
                    beq r_physics_rts
b_apply_movement_delay
                    inc a_slow_down_counter ; x-ref: $c81e
                    lda a_slow_down_counter
                    cmp a_flag_slow_movement
                    bne r_physics_rts
                    lda #$00
                    sta a_slow_down_counter
                    jmp check_sliding_on_rope
                    .byte $29, $03, $aa, $e8, $8a
r_physics_rts       rts                  ; x-ref: $c825, $c830
                    nop 
                    nop 
                    nop 
                    lda a_033C
                    ldx a_033D
                    jsr s_update_sprite_coordinates
                    rts 
a_C84D              lda #$00             ; x-ref: $5821, $588d, $c720
                    sta a_input_fire_button
                    lda a_movement_mode
                    cmp #$01
                    beq b_C861
                    lda #$00
                    sta a_input_delta_y
                    sta a_input_delta_x
b_C861              lda zp_joystick_port2_state ; x-ref: $c857
                    cmp #$40
                    beq b_C8AB
                    lda #$00
                    sta a_input_delta_y
                    sta a_input_delta_x
                    lda zp_joystick_port2_state
                    cmp a_col_right_bound
                    bne b_C87B
                    lda #$ff
                    sta a_input_delta_y
b_C87B              lda zp_joystick_port2_state ; x-ref: $c874
                    cmp a_col_right_bound
                    bne b_C887
                    lda #$ff
                    sta a_input_delta_y
b_C887              lda zp_joystick_port2_state ; x-ref: $c880
                    cmp a_col_left_bound
                    bne b_C893
                    lda #$01
                    sta a_input_delta_y
b_C893              lda zp_joystick_port2_state ; x-ref: $c88c
                    cmp a_col_top_bound
                    bne b_C89F
                    lda #$ff
                    sta a_input_delta_x
b_C89F              lda zp_joystick_port2_state ; x-ref: $c898
                    cmp a_col_bottom_bound
                    bne b_C8AB
                    lda #$01
                    sta a_input_delta_x
b_C8AB              lda music_v1_ctrl    ; x-ref: $c865, $c8a4
                    cmp #$00
                    beq b_C8B7
                    lda #$01
                    sta a_input_fire_button
b_C8B7              jsr s_check_joystick_idle ; x-ref: $c8b0
                    and #$01
                    cmp #$01
                    beq b_C8C5
                    lda #$01
                    sta a_input_fire_button
b_C8C5              lda $dc00            ; x-ref: $c8be; CIA1: Data Port Register A
                    and #$02
                    cmp #$02
                    beq b_C8D3
                    lda #$01
                    sta a_input_delta_y
b_C8D3              lda $dc00            ; x-ref: $c8cc; CIA1: Data Port Register A
                    and #$04
                    cmp #$04
                    beq b_C8E1
                    lda #$ff
                    sta a_input_delta_x
b_C8E1              lda $dc00            ; x-ref: $c8da; CIA1: Data Port Register A
                    and #$08
                    cmp #$08
                    beq r_C8EF
                    lda #$01
                    sta a_input_delta_x
r_C8EF              rts                  ; x-ref: $c8e8
