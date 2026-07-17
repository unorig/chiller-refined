;==============================================================================
; $5800-$5AFF - MAIN PER-FRAME GAMEPLAY BODY
;==============================================================================
;
; Called once per IRQ. Patches game-state behavior, handles joystick/rope
; movement, Boy/Girl transition setup, health tick logic, and collision side
; effects before entering broader game-state processing.
;
; === sub_5800 : Main per-frame game body ===
; Called once per IRQ. Increments $CF02 tick counter.
; Calls Sub_GoSlowRedZone (health red-zone slowdown) and Sub_HealthBarUpdates.
; Reads $5A00/$5A01 collision-direction flags -> stores to $C1ED.
; If player is NOT jumping: SMC-patches Sub_GetInputs and Sub_CheckSlidingOnRope
; to LDA mode (read joystick+rope state).
; Handles Boy/Girl sprite swap via branch_ChangeToGirl / jump_ChangeToBoy.
main_game_body      inc a_global_tick_counter ; x-ref: $ca03
                    jsr go_slow_red_zone
                    jsr s_health_then_double_jump
                    nop 
                    nop 
                    lda collision_dir_flag
                    cmp #$00
                    beq b_game_state_init_check
                    lda collision_dir_value
                    sta a_input_delta_x
b_game_state_init_check
                    lda a_C19C           ; x-ref: $5810
                    bne b_airborne_jump_tick
                    lda a_fall_step_counter
                    beq b_check_passive_fall
b_airborne_jump_tick
                    jsr s_try_double_jump
                    jmp b_player_action_check
b_check_passive_fall
                    lda #$a9
                    sta a_C84D
                    lda #$ad
                    sta check_sliding_on_rope
                    jsr check_sliding_on_rope_b
a_level_config_index =*+$01
                    lda #$e8
                    sta a_C79C
a_player_action_temp =*+$01
                    lda #$ec
                    sta a_C7C6
b_player_action_check
                    jmp s_tick_active_movement_health_reduction ; x-ref: $581d
                    nop 
                    nop 
sub_MainGameEntry   lda sprite_ptr0      ; x-ref: $57ba, $57f4, $58d8, $58e0
a_key_scanner_temp_1 =*+$01
                    cmp #$f1
                    bne b_key_scanner_done
a_key_scanner_temp_2 =*+$01
                    lda #$f6
                    sta sprite_ptr0
b_key_scanner_done  ldx $d000            ; x-ref: $5849; Sprite 0 X Pos
                    lda player_x_prev
                    stx player_x_prev
                    cmp player_x_prev
                    beq b_joystick_idle
                    lda #$00
                    sta a_collision_temp
check_x_movement    rts                  ; x-ref: $586c
b_joystick_idle     inc a_collision_temp ; x-ref: $585c
                    lda a_collision_temp
                    cmp #$90
                    bne check_x_movement
                    lda sprite_ptr0
                    and #$04
                    bne b_joystick_y_check
a_joystick_x_delta =*+$01
                    lda #$f2
                    jmp update_lr_sprite
a_joystick_y_delta =*+$01
b_joystick_y_check  lda #$f6             ; x-ref: $5873
update_lr_sprite    sta sprite_ptr0      ; x-ref: $5877
                    rts 
update_player_direction
                    lda #$00             ; x-ref: $2a7b
                    sta collision_dir_flag
                    lda a_input_delta_x
                    sta collision_dir_value
                    lda #$60
                    sta a_C84D
                    sta check_sliding_on_rope
                    lda sprite_ptr0
                    and #$04
                    bne b_rope_slide_done
a_rope_slide_timer =*+$01
                    lda #$f0
                    nop 
                    sta sprite_ptr0
                    jmp b_rope_slide_setup
a_rope_slide_offset =*+$01
b_rope_slide_done   lda #$f1             ; x-ref: $5898
                    sta sprite_ptr0
a_rope_slide_speed =*+$01
b_rope_slide_setup  lda #$f0             ; x-ref: $58a0
                    sta a_C79C
                    sta a_C7C6
                    jmp sfx_sweep_continue
; Advance animation frame for all active enemy sprites via sprite pointer update.
check_sliding_on_rope_b
                    lda a_var_on_rope    ; x-ref: $5829; Rope contact flag: non-zero = player is on a rope tile.
                    bne b_check_sliding_next
                    rts 
                    .byte $ea, $ea
b_check_sliding_next
                    ldx #$00             ; x-ref: $58b6
                    lda $dc00            ; CIA1: Data Port Register A
                    and #$10
                    bne b_check_sliding_left
                    nop 
                    ldx #$01
b_check_sliding_left
                    lda zp_joystick_port2_state ; x-ref: $58c2
                    cmp #$37
                    bne b_check_sliding_right
                    ldx #$01
b_check_sliding_right
                    cpx #$00             ; x-ref: $58cb
                    bne fire_button_event
                    lda #$00
                    sta fire_button_flag
                    jmp sub_MainGameEntry
fire_button_event   lda fire_button_flag ; x-ref: $58d1
                    beq b_sprite_pos_setup_done
                    jmp sub_MainGameEntry
b_sprite_pos_setup_done
                    jmp j_jmp_to_7280    ; x-ref: $58de
; Update sprite colors for all active slots from the game color tables ($CF30+ area).
setup_sprite_positions
                    lda #$01             ; x-ref: $5770
                    sta fire_button_flag
                    ldx $d000            ; Sprite 0 X Pos
                    lda $d002            ; Sprite 1 X Pos
                    sta $d000            ; Sprite 0 X Pos
                    stx $d002            ; Sprite 1 X Pos
                    ldx $d001            ; Sprite 0 Y Pos
                    jmp cont_setup_sprite_positions
                    .byte $8d, $f8, $07
; Game start/level init: resets score, positions Sprite 1 off-screen ($D002/$D003=$E0),
; sets border blue ($D020=6), sets Sprite 1 pointer to $EE ($07F9).
reset_girl          jsr s_reset_score_display ; x-ref: $c646
                    lda #$e0
                    sta $d002            ; Sprite 1 X Pos
                    sta $d003            ; Sprite 1 Y Pos
                    lda #$06
                    nop 
                    sta $d020            ; Border Color
                    lda #$ee
                    sta sprite_ptr1
                    rts 
                    .byte $ea, $ea
cont_setup_sprite_positions
                    lda $d003            ; x-ref: $58fa; Sprite 1 Y Pos
                    sta $d001            ; Sprite 0 Y Pos
                    stx $d003            ; Sprite 1 Y Pos
                    lda $d010            ; Sprites 0-7 MSB of X coordinate
                    and #$01
                    tax 
                    lda $d010            ; Sprites 0-7 MSB of X coordinate
                    and #$02
                    tay 
                    lda $d010            ; Sprites 0-7 MSB of X coordinate
                    and #$fc
                    sta rope_state
                    cpy #$00
                    beq b_check_rope_above
                    lda #$01
                    ora rope_state
                    sta rope_state
b_check_rope_above  cpx #$00             ; x-ref: $5938
                    beq b_check_rope_below
                    lda #$02
                    ora rope_state
                    sta rope_state
b_check_rope_below  lda rope_state       ; x-ref: $5944
                    sta $d010            ; Sprites 0-7 MSB of X coordinate
                    ldx sprite_ptr0
                    lda sprite_ptr1
                    sta sprite_ptr0
                    stx sprite_ptr1
                    rts 
; Scan screen row $042E for character $A9 (reverse '@'); on match call j_CA27 (boss action);
; check $450C (level), if 4 trigger boss state via j_5A88.
go_slow_red_zone    lda a_0430           ; x-ref: $5803
                    cmp #$a9
                    beq b_rope_collision_done
                    lda #$80
                    sta a_smc_physics_speed_limit
                    lda #$04
                    sta a_flag_slow_movement
                    rts 
b_rope_collision_done
                    lda a_flag_slow_movement ; x-ref: $5966
                    cmp #$04
                    beq b_rope_collision_active
                    rts 
b_rope_collision_active
                    jmp reset_movement_speed ; x-ref: $5978
                    .byte $ea, $ea
; === Sub_CheckSlidingOnRope ===
; Checks if player sprite 0 is overlapping a rope character on screen.
; SMC-patched by sub_5800 at runtime (first byte toggled between LDA $AD and other)
; to enable/disable when player is airborne.
; Sets Var_SlidingOnRope ($4503) if on rope.
check_sliding_on_rope
                    lda sprite_ptr0      ; x-ref: $5826, $5890, $c837
                    and #$03
                    cmp #$03
                    beq b_rope_slide_active
                    inc sprite_ptr0
                    jmp r_rope_slide_rts
b_rope_slide_active lda sprite_ptr0      ; x-ref: $5987
                    and #$fc
                    sta sprite_ptr0
r_rope_slide_rts    rts                  ; x-ref: $598c
; Scan screen row $042E from right; find rightmost non-$A8 character and return its X index.
get_current_health_bar
                    ldx #$ff             ; x-ref: $59a3, $59c3
b_health_bar_reduce_done
                    inx                  ; x-ref: $59a0
                    lda f_042E,x
                    cmp #$a9
                    beq b_health_bar_reduce_done
                    rts 
; Decrement score character at the position found by scan_score_row;
; if wraps past $A8, cascade to next digit position.
reduce_health_bar   jsr get_current_health_bar ; x-ref: $59f5, $5a39, $72ba
                    lda f_042E,x
                    cmp #$a1
                    beq b_health_bar_reduce_loop
                    dec f_042E,x
                    rts 
                    .byte $ea, $ea
b_health_bar_reduce_loop
                    cpx #$00             ; x-ref: $59ab
                    beq no_health_left
                    dex 
                    lda #$a8
                    sta f_042E,x
                    rts 
                    .byte $ea, $ea
no_health_left      jmp s_set_slow_movement_params ; x-ref: $59b5
; Increment score character; if wraps past $0A, cascade carry to next digit
; and write $0A to $CF5A/$CF5B (score carry flags).
increase_health_block
                    jsr get_current_health_bar ; x-ref: $5b12
                    cpx #$21
                    beq b_health_inc_done
                    inc f_042E,x
                    rts 
                    .byte $ea, $ea
b_health_inc_done   lda #$0a             ; x-ref: $59c8
                    sta a_score_carry_flag
                    lda #$00
                    sta a_score_block_param_b
                    lda #$11
                    sta a_score_block_param_c
                    jsr s_sfx_trigger
                    rts 
                    .byte $ea, $ea, $ea
s_tick_active_movement_health_reduction
                    lda a_C19C           ; x-ref: $5836
                    beq b_movement_speed_check
                    dec char_select_x_init
                    bne r_health_tick_rts
                    lda #$00
                    sta char_select_x_init
                    jsr reduce_health_bar
r_health_tick_rts   rts                  ; x-ref: $59ee
b_movement_speed_check
                    jmp s_tick_player_y_collision_sfx ; x-ref: $59e9
                    nop 
                    nop 
                    nop 
                    nop 
collision_dir_flag  brk                  ; Collision direction flag ($5A00): non-zero = active collision detected this frame. ; x-ref: $580b, $5882
player_x_prev =*+$01
collision_dir_value ora (zpp_70,x)       ; Collision direction value ($5A01): direction code (0-3) of active collision. ; x-ref: $5812, $5888
col_active_flags =*+$01
a_collision_temp    cmp (zp_wrap_pointer_lo,x) ; x-ref: $5860, $5864, $5867
                    brk 
fire_button_flag    brk                  ; x-ref: $58d5, $58db, $58e8
var_character_select =*+$01
char_select_x_init =*+$02
rope_state          jsr s_6401           ; x-ref: $5933, $593c, $593f, $5948, $594b, ...
a_sfx_check_counter =*+$01
char_select_y_init  stx music_state,y    ; x-ref: $5a25, $5a2c
damage_flag         .byte $12
; Health/pickup and damage/effect queues consumed by the $5B00 level-flow code.
; These are safe early mod targets because they are byte-sized counters/timers.
pending_health_restore_count
                    .byte $00            ; queued health blocks from mushroom pickups; x-ref: $5b00, $5b0f, $5b20, $5db3
health_restore_tick_timer
                    .byte $10            ; delay between spending queued health restores; x-ref: $5b05, $5b0c
bridge_erosion_tick_counter
                    .byte $01            ; counts down before bridge tile erosion advances; x-ref: $5aa5, $5aaf
pending_health_bonus_counter
                    .byte $00            ; loop counter used while queuing health restores; x-ref: $5b18, $5b1b
queued_damage_effect_count
                    .byte $00            ; queued damage/effect routine calls; x-ref: $5b2c, $5b3b, $5b4c, $5db6, $75b0
damage_effect_tick_timer
                    .byte $12            ; delay between queued damage/effect routine calls; x-ref: $5b31, $5b38
queued_damage_add_counter
                    .byte $00            ; loop counter used while queuing damage/effects; x-ref: $5b44, $5b47
tile_collision_repeat_counter
                    .byte $01            ; repeat gate for a subset of collision tiles; x-ref: $5ba0, $5ba7
var_magic_crosses   .byte $0a            ; x-ref: $7603, $7f00
a_magic_cross_collected
                    .byte $09            ; x-ref: $7f80, $7f87
a_double_jump_state
                    .byte $00            ; 0=waiting, 1=armed after up release, 2=spent until landing.
                    .fill 8, $ff
s_tick_player_y_collision_sfx
                    lda a_input_delta_x  ; x-ref: $59f9
                    beq r_y_collision_rts
                    dec char_select_y_init
                    bne r_y_collision_rts
                    lda #$ff
                    sta char_select_y_init
                    dec a_sfx_check_counter
                    bne r_y_collision_rts
                    lda #$03
                    sta a_sfx_check_counter
                    jsr reduce_health_bar
r_y_collision_rts   rts                  ; x-ref: $5a23, $5a28, $5a32
                    .byte $ff, $ff, $ff, $f6
; Main game sub-state dispatcher: reads state flag a_4500 and game input,
; routes to appropriate animation/movement handlers.
game_substate_main  jsr level_data_load  ; x-ref: $5e25
                    lda var_character_select
                    bne b_game_state_active
                    lda #$06
                    sta $d020            ; Border Color
                    rts 
b_game_state_active lda #$0a             ; x-ref: $5a47
                    sta $d020            ; Border Color
                    rts 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
                    brk 
