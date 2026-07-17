;==============================================================================
; $5B00-$6000 - GAME STATE, LEVEL FLOW, DAMAGE, SCORE, HIGH SCORE
;==============================================================================
;
; The high-level gameplay state machine: collectibles, health/damage, projectile
; hits, magic cross checks, level transitions, enemy spawning/drawing, score
; refresh, high-score handling, and level config loading.
;
; ====================================================
;    GAME STATE AND LEVEL INITIALIZATION ENGINE
; ====================================================
;
; This is the high-level game-state switchboard. It reacts to the current level
; status, initializes new levels, updates score/lives/health, checks enemy and
; projectile results, and decides when to transition between gameplay, death,
; level-complete, title/menu, and high-score states.
;
; Tile and timing constants in this block are good first modding knobs: pickup
; values, bridge erosion cadence, health restore pacing, and damage/effect
; pacing can be changed with small byte-sized edits once behaviour is verified.
;
TILE_INTERACTIVE_MIN = $4d
TILE_COLLISION_PASS_Y = $50
TILE_BRIDGE_FINAL = $53
TILE_MUSHROOM = $54
TILE_RED_FLOWER = $55
TILE_BASKET = $56
TILE_MAGIC_CROSS = $57
TILE_SOLID_RANGE_LOW = $62
TILE_SOLID_RANGE_HIGH = $66
TILE_BLANK_SPACE = $a0

BRIDGE_EROSION_INTERVAL = $08
HEALTH_RESTORE_INTERVAL = $10
DAMAGE_EFFECT_INTERVAL = $12
MUSHROOM_RESTORE_TICKS = $18
RED_FLOWER_EFFECT_TICKS = $19
BASKET_SCORE_CARRY = $64
BASKET_SCORE_PARAM_C = $11
COLLISION_REPEAT_INTERVAL = $03

main_game_start     lda #$02             ; x-ref: $75ac
                    sta a_flag_slow_movement
                    lda #$00
                    sta a_slow_down_counter
                    jmp s_enable_multicolor_and_disable_kernal
                    .byte $00, $00, $00
s_set_slow_movement_params
                    lda #$01             ; x-ref: $59c0
                    sta a_sprite_process_flag
                    lda #$02
                    sta a_flag_slow_movement
                    lda #$00
                    sta a_slow_down_counter
                    lda #$20
                    sta a_smc_physics_speed_limit
                    rts 
                    .byte $ff, $ff, $ff
reset_movement_speed
                    lda #$02             ; x-ref: $597b
                    sta a_flag_slow_movement
                    lda #$00
                    sta a_slow_down_counter
                    lda #$20
                    sta a_smc_physics_speed_limit
                    rts 
                    .byte $ea, $ea
; Interpret the tile under/near the player after the low-level collision probe.
; Tiles below TILE_INTERACTIVE_MIN are ignored here. A Y result of $50 means the
; player is on the bridge/interactive row, which also ticks bridge erosion.
check_interactive_or_solid_tile    cmp #TILE_INTERACTIVE_MIN ; x-ref: $2a6b
                    bmi r_interactive_tile_rts
                    cpy #TILE_COLLISION_PASS_Y
                    beq b_tile_allows_bridge_tick
                    jmp b_player_no_collision
b_tile_allows_bridge_tick
                    dec bridge_erosion_tick_counter ; x-ref: $5aa0
                    beq tick_bridge_tile_erosion
r_interactive_tile_rts
                    rts                  ; x-ref: $5a9c
                    .byte $ea, $ea
; Update bridge tile state every BRIDGE_EROSION_INTERVAL visits.
tick_bridge_tile_erosion       ldx #BRIDGE_EROSION_INTERVAL ; x-ref: $5aa8
                    stx bridge_erosion_tick_counter
                    nop 
                    cmp #TILE_BRIDGE_FINAL
                    bne advance_bridge_tile_damage
                    lda #TILE_BLANK_SPACE
                    sta (ptr_lo),y
                    rts 
                    .byte $ea, $ea, $60
; Erode bridge: advance the bridge tile character one step toward collapse.
advance_bridge_tile_damage        lda (ptr_lo),y       ; x-ref: $5ab5
                    tax 
                    inx 
                    txa 
                    sta (ptr_lo),y
                    rts 
; Mushroom pickup: queues a gradual health restore, then clears the map tile.
collect_mushroom        cmp #TILE_MUSHROOM   ; x-ref: $2a6e
                    bne collect_red_flower
                    ldx #MUSHROOM_RESTORE_TICKS
                    jsr queue_health_restore_ticks
                    lda #TILE_BLANK_SPACE
                    sta (ptr_lo),y
                    rts 
; Red flower pickup: queues repeated damage/effect routine calls, then clears the map tile.
collect_red_flower      cmp #TILE_RED_FLOWER ; x-ref: $5ac9
                    bne collect_basket
                    ldx #RED_FLOWER_EFFECT_TICKS
                    jsr queue_damage_effect_ticks
                    lda #TILE_BLANK_SPACE
                    sta (ptr_lo),y
                    rts 
; Basket pickup: primes score/effect parameters, plays SFX, then clears the map tile.
collect_basket          cmp #TILE_BASKET     ; x-ref: $5ad7
                    bne b_check_magic_cross_tile
                    lda #BASKET_SCORE_CARRY
                    sta a_score_carry_flag
                    lda #$00
                    sta a_score_block_param_b
                    lda #BASKET_SCORE_PARAM_C
                    sta a_score_block_param_c
                    jsr s_sfx_trigger
                    lda #TILE_BLANK_SPACE
                    sta (ptr_lo),y
                    rts 
                    .byte $ea, $ea
; Apply queued health restores gradually rather than all at once. The mushroom
; path adds to pending_health_restore_count; this routine spends one restore
; every HEALTH_RESTORE_INTERVAL ticks.
health_bar_updates  lda pending_health_restore_count ; x-ref: $5806
                    beq b_health_ticks_done
                    dec health_restore_tick_timer
                    bne b_health_ticks_done
                    lda #HEALTH_RESTORE_INTERVAL
                    sta health_restore_tick_timer
                    dec pending_health_restore_count
                    jsr increase_health_block
b_health_ticks_done jmp s_process_queued_damage_effects ; x-ref: $5b03, $5b08
; Queue N health-restore ticks, where X is the requested count.
queue_health_restore_ticks      stx pending_health_bonus_counter ; x-ref: $5acd
s_queue_health_restore_loop
                    dec pending_health_bonus_counter ; x-ref: $5b25
                    beq r_queue_health_restore_rts
                    inc pending_health_restore_count
                    beq r_queue_health_restore_rts
                    jmp s_queue_health_restore_loop
r_queue_health_restore_rts    rts                  ; x-ref: $5b1e, $5b23
                    .byte $ea, $ea, $ea
; Spend queued damage/effect work gradually. damage_routine currently toggles
; visual damage feedback and also touches the health/score bar path.
s_process_queued_damage_effects
                    lda queued_damage_effect_count ; x-ref: $5b15
                    beq r_damage_effect_process_rts
                    dec damage_effect_tick_timer
                    bne r_damage_effect_process_rts
                    lda #DAMAGE_EFFECT_INTERVAL
                    sta damage_effect_tick_timer
                    dec queued_damage_effect_count
                    jsr damage_routine
r_damage_effect_process_rts
                    rts                  ; x-ref: $5b2f, $5b34
                    .byte $ea, $ea
; Queue N damage/effect ticks, where X is the requested count. This is used by
; pickups and by projectile/enemy collision paths.
queue_damage_effect_ticks    stx queued_damage_add_counter   ; x-ref: $5adb, $cdc0, $cde0
s_queue_damage_effect_loop
                    dec queued_damage_add_counter   ; x-ref: $5b51
                    beq r_queue_damage_effect_rts
                    inc queued_damage_effect_count
                    beq r_queue_damage_effect_rts
                    jmp s_queue_damage_effect_loop
r_queue_damage_effect_rts  rts                  ; x-ref: $5b4a, $5b4f
                    .byte $ea, $ea
b_check_magic_cross_tile
                    cmp #TILE_MAGIC_CROSS ; x-ref: $5ae5
                    bne b_check_collision_height_range
                    jmp s_check_magic_cross_collect
; Increment the magic-cross-collected counter at screen position Adr_MagicCrossNumRight ($041D).
inc_magic_cross_right
                    nop                  ; x-ref: $7f77
                    lda magic_cross_right_digit
                    cmp #$b9
                    beq b_magic_cross_right_digit_wrapped
                    inc magic_cross_right_digit
                    jmp j_magic_cross_counter_check
b_magic_cross_right_digit_wrapped
                    lda magic_cross_left_digit ; x-ref: $5b64
                    cmp #$b9
                    bne b_magic_cross_left_digit_increment
                    lda #$b0
                    sta magic_cross_right_digit
                    sta magic_cross_left_digit
                    jmp j_magic_cross_counter_check
b_magic_cross_left_digit_increment
                    lda #$b0             ; x-ref: $5b71
                    sta magic_cross_right_digit
                    inc magic_cross_left_digit
j_magic_cross_counter_check
                    jmp magic_cross_counter_check ; x-ref: $5b69, $5b7b
b_check_collision_height_range cmp #TILE_SOLID_RANGE_LOW ; x-ref: $5b59
                    bpl b_check_upper_collision_range
jump_height_check   cpy #TILE_COLLISION_PASS_Y ; x-ref: $7677
                    beq r_collision_tile_rts
                    jmp b_player_no_collision
r_collision_tile_rts   rts                  ; x-ref: $5b8f, $5ba3
b_check_upper_collision_range    cmp #TILE_SOLID_RANGE_HIGH ; x-ref: $5b8b
                    bpl b_check_floor_collision_fallback
                    cpy #TILE_COLLISION_PASS_Y
                    beq b_collision_repeat_gate
                    jmp b_player_no_collision
b_collision_repeat_gate   dec tile_collision_repeat_counter ; x-ref: $5b9b
                    bne r_collision_tile_rts
                    lda #COLLISION_REPEAT_INTERVAL
                    sta tile_collision_repeat_counter
                    jmp b_player_no_collision
b_check_floor_collision_fallback  jmp s_check_floor_collision ; x-ref: $5b97
; Handle player death: disable sprite 0, decrement lives counter, trigger death animation.
wait_for_raster     lda $d012            ; x-ref: $576b, $5bb5, $5bbc; Raster Position
                    cmp #$10
                    bne wait_for_raster
                    lda $d011            ; VIC Control Register 1
                    and #$80
                    bne wait_for_raster
                    rts 
