s_init_sid_music    jsr sid_setup        ; x-ref: $2e91
                    rts 
;==============================================================================
; $2F50-$2FFF - INDIRECT SPRITE TABLE READS, RESET, ENEMY RESPAWN TICKER
;==============================================================================
;
; Reads zero-page indirect data into sprite state tables, performs IRQ/sprite
; reset, and periodically activates enemy slots based on respawn counts and
; randomized Y thresholds.
;
; MOD: modern air control wrapper.
; Grounded path delegates to the original input reader. Airborne path leaves
; the original vertical jump state alone, but refreshes horizontal direction and
; facing sprite from the joystick so releasing/pressing left-right works mid-jump.
s_airborne_horizontal_input_wrapper
                    lda a_C19C
                    bne b_airborne_horizontal_only
                    lda a_fall_step_counter
                    bne b_airborne_horizontal_only
                    lda #$20
                    sta a_smc_physics_speed_limit
                    jmp a_C84D
b_airborne_horizontal_only
                    jsr s_try_double_jump
                    lda #$00
                    sta a_input_delta_x
                    lda $dc00
                    and #$0c
                    cmp #$08
                    beq b_air_control_left
                    cmp #$04
                    beq b_air_control_right
                    rts 
b_air_control_left  lda #$ff
                    sta a_input_delta_x
                    lda #$e0
                    sta sprite_ptr0
                    rts 
b_air_control_right lda #$01
                    sta a_input_delta_x
                    lda #$e1
                    sta sprite_ptr0
                    rts 
                    nop
b_read_zp_loop2     lda (ptr_lo),y       ; x-ref: $2f93
                    sta $ffff,y          ; IRQ
                    iny 
                    cpy #$07
                    bne b_read_zp_loop2
                    lda a_sprite_enable_mask_cf5e
                    eor #$ff
                    and $d015            ; Sprite display Enable
                    sta $d015            ; Sprite display Enable
                    ldx a_sfx_index_cf5f
                    jsr s_sfx_helper_1
                    rts 
s_halt_irq_and_reset_sprites
                    lda #$00             ; x-ref: $5412
                    sta $ffff            ; IRQ
                    jsr s_stop_fall_and_sfx
                    jmp s_reset_enemy_ticker
; Periodic sprite respawn ticker.
; Called every frame from the main game loop. Increments a throttle counter at
; $CF75 and exits immediately unless it has reached $FF (i.e. full logic runs
; only once every 256 calls, ~4 seconds at 60 Hz).
; 
; When the timer fires, loops through all 5 sprite slots (0-4):
;   - Skip if the sprite is currently enabled in $D015 (already visible).
;   - Skip if the pending respawn count for this slot ($CF76,X) is zero.
;   - Activate via s_C0E4 if candidate Y < per-slot threshold.
; 
; Inputs:  None (reads global state)
; Outputs: None (updates $CF75, $CF76,X, $CF29,X, $CF1A,X)
; Side Effects: May enable up to 5 sprites per invocation via s_C0E4.
s_enemy_update_start
                    inc a_level_tick_timer ; x-ref: $ca54
                    lda a_level_tick_timer
                    cmp #$ff
                    beq j_enemy_pos_loop
                    rts 
j_enemy_pos_loop    lda #$00             ; x-ref: $2fba
                    sta a_level_tick_timer
                    ldx #$00
b_next_enemy_loop   lda f_enemy_x_msb_init_table,x ; x-ref: $2ff8
                    and $d015            ; Sprite display Enable
                    cmp #$00
                    bne b_next_enemy_done
                    lda f_level_enemy_type_seeds,x
                    cmp #$00
                    beq b_next_enemy_done
                    txa 
                    pha 
                    jsr s_increment_watermark
                    pla 
                    tax 
                    tya 
                    and #$7f
                    cmp f_enemy_respawn_y_threshold,x
                    bpl b_next_enemy_done
                    txa 
                    jsr s_activate_vertical_enemies
                    dec f_level_enemy_type_seeds,x
                    lda #$01
                    sta f_sprite_anim_step_index,x
                    lda #$00
                    sta f_sprite_anim_frame_counters,x
b_next_enemy_done   inx                  ; x-ref: $2fcc, $2fd3, $2fe2
                    cpx #$05
                    bne b_next_enemy_loop
                    rts 
