s_increment_watermark
                    inc a_watermark_counter ; x-ref: $2fd7, $7580, $7587, $7615, $761c, ...
                    ldx a_watermark_counter
                    lda s_sprite_overlap_handler,x
                    eor joystick_mask
                    tay 
                    ldx #$20
                    rts 
; === Core Game Frame Update Dispatcher ===
; Handles the border/background color changes, checks the game frame status registers, updates individual sprite coordinates/velocities on tick intervals, and routes physics checks every 32 frames.
s_game_frame_update_dispatch
                    jsr damage_border_colour ; x-ref: $2d28, $ca61
                    jsr main_game_body
a_smc_game_tick_limit =*+$01
                    cmp #$08
                    bne b_frame_counter_check
                    lda #$00
                    sta a_global_tick_counter
                    jsr s_update_sprite1_position
b_frame_counter_check
                    inc a_game_frame_timer ; x-ref: $ca08
                    lda a_game_frame_timer
a_smc_physics_speed_limit =*+$01
                    cmp #$20
b_frame_physics_check
                    bne b_frame_dispatch_done
                    lda #$00
                    sta a_game_frame_timer
                    jsr s_player_physics_dispatch
b_frame_dispatch_done
                    jmp b_enemy_loop_reset ; x-ref: $ca1a
; === Enemy & Projectile Physics Logic Loop ===
; Iterates across the five active enemy slots to update their animation offsets and coordinates, executes projectile range checks, and dispatches screen HUD refreshing checks.
s_update_enemies_and_projectiles
                    lda #$00             ; x-ref: $2eb3
                    sta a_CEFF
b_enemy_update_loop lda a_CEFF           ; x-ref: $ca3a
                    jsr s_enemy_sprite_anim_dispatcher
                    inc a_CEFF
                    lda a_CEFF
                    cmp #$05
                    bne b_enemy_update_loop
                    jsr s_update_player_projectile
                    jsr s_sfx_helper_2
                    lda a_logic_tick_flag
                    cmp #$01
                    bne b_skip_hud_refresh
                    lda #$00
                    sta a_logic_tick_flag
                    jmp j_C694
b_skip_hud_refresh  jsr s_update_sprite_animation ; x-ref: $ca47
                    jsr s_enemy_update_start
; === Main Game Run Check ===
; Loops the execution back to the frame update dispatcher until the stage music status flag changes to $FF (signaling level end or game over), at which point it exits.
                    lda a_music_play_status_flag
                    cmp #$ff
                    bne b_game_loop_continue
                    jmp s_set_irq_and_mem
b_game_loop_continue
                    jmp s_game_frame_update_dispatch ; x-ref: $ca5c
s_update_sprite_motion_if_active
                    lda a_physics_collision_flags,x ; x-ref: $cc87
                    and $d015            ; Sprite display Enable
                    beq r_motion_check_rts
                    jsr s_call_sprite_axis_update
r_motion_check_rts  rts                  ; x-ref: $ca6a
s_dispatch_sprite_spawn_or_update
                    lda f_sprite_active_status,x ; x-ref: $c00b
                    cmp #$01
                    beq b_sprite_update_draw
                    lda f_enemy_sprite7_index_list,x
                    jmp s_spawn_player_projectile
b_sprite_update_draw
                    jmp s_tick_projectile_step ; x-ref: $ca75
s_stop_fall_and_sfx jsr s_silence_voice3_and_stop_fall ; x-ref: $2fac
                    nop 
                    nop 
                    nop 
s_reset_sprite_frame_and_irq
                    lda #$fc             ; x-ref: $2c3a
                    sta sprite_ptr0
                    lda a_vic_irq_config_val1
                    sta $ffff            ; IRQ
                    lda a_vic_irq_config_val2
                    sta $ffff            ; IRQ
                    lda a_vic_irq_config_val3
                    sta $ffff            ; IRQ
                    lda #$00
                    sta a_flicker_counter
j_CAA2              lda #$00             ; x-ref: $cae2
                    sta a_flicker_slot_index
j_CAA7              lda a_flicker_counter ; x-ref: $cad5
                    tay 
                    lda f_vic_irq_offset_table,y
                    tay 
                    lda f_C300,y
                    sta $ffff            ; IRQ
                    lda f_C340,y
                    sta $ffff            ; IRQ
                    lda #$20
                    sta a_delay_loop_param_lo
                    lda #$40
                    sta a_delay_loop_param_hi
                    jsr s_delay_wait_loop
                    inc a_flicker_slot_index
                    lda a_flicker_slot_index
                    cmp #$08
                    beq b_CAD8
                    inc a_flicker_counter
                    jmp j_CAA7
b_CAD8              lda sprite_ptr0      ; x-ref: $cad0
                    cmp #$ff
                    beq b_CAE5
                    inc sprite_ptr0
                    jmp j_CAA2
b_CAE5              ldy a_vic_irq_config_val1 ; x-ref: $cadd
                    dey 
                    jmp j_CAF9
                    rts 
                    dec a_sprite_system_config_val
                    lda #$01
                    sta $d015            ; Sprite display Enable
                    jsr s_reset_enemy_counts_vars_wrapper
                    rts 
j_CAF9              sty $ffff            ; x-ref: $cae9; IRQ
a_smc_vector_projectile
                    jmp j_CEDA           ; x-ref: $2c37, $2c3f
                    nop 
                    rts 
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
                    nop 
                    nop 
                    nop 
                    nop 
                    nop 
                    nop 
                    nop 
                    rts 
