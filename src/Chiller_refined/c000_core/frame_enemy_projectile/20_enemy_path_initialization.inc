a_enemy_init_index  .byte $01            ; x-ref: $cb24, $cb3a, $cb61, $cb7d, $cb8b, ...
; === VIC Enemy Sprite Wave Initializer ===
; Sets up initial hardware coordinates and direction matrices for the selected enemy sprite.
; Reads level configurations, initializes VIC coordinate registers ($D004-$D005, $D010), and loops to pre-calculate path step vectors.
s_initialize_enemy_sprite
                    sta a_enemy_init_index ; x-ref: $c0e7, $c3fb
                    tax 
                    asl a
                    tay 
                    lda f_enemy_init_coordinate_index,x ; Load initial coordinate array offset
                    tax 
                    lda a_enemy_xpos,x   ; Enemy initial X positions (5 entries). Set from level config, used by init_sprite_movement.
                    sta $d004,y          ; Store in VIC-II Sprite X coordinate register; Sprite 2 X Pos
                    lda a_enemy_ypos,x   ; Store in VIC-II Sprite Y coordinate register
                    sta $d005,y          ; Sprite 2 Y Pos
                    ldy a_enemy_init_index
                    lda f_enemy_x_msb_init_table,y
                    eor #$ff
                    and $d010            ; Sprites 0-7 MSB of X coordinate
                    sta $d010            ; Clear MSB bit in VIC $D010 for this sprite slot; Sprites 0-7 MSB of X coordinate
                    lda a_enemy_xmsb,x
                    ora $d010            ; Sprites 0-7 MSB of X coordinate
                    sta $d010            ; Set new X MSB coordinate bit based on configuration; Sprites 0-7 MSB of X coordinate
                    nop 
                    lda a_enemy_dir_count,x
                    cmp #$00
                    beq b_enemy_path_limit_check
                    jsr s_increment_watermark ; Query copy protection watermark checker
; === Pre-Calculate Enemy Path Steps Loop ===
; Pre-calculates the initial directional steps of the enemy wave by calling coordinate updates in a loop before the sprite is displayed.
s_init_enemy_path_steps
                    lda #$00             ; x-ref: $cbb8
                    sta a_enemy_init_slot_index
b_enemy_path_step_loop
                    ldx a_enemy_init_index ; x-ref: $cb88
                    lda f_enemy_init_coordinate_index,x
                    tax 
                    lda a_enemy_init_slot_index
                    cmp a_enemy_dir_count,x
                    beq b_enemy_path_step_done
                    dey 
                    cpy #$00
                    beq b_enemy_path_limit_check
                    inc a_enemy_init_slot_index
                    lda #$03
                    sty a_temp_reg_y
                    ldx a_enemy_init_index
                    inx 
                    inx 
                    jsr s_update_sprite_coordinates
                    ldy a_temp_reg_y
                    jmp b_enemy_path_step_loop
b_enemy_path_step_done
                    lda a_enemy_init_index ; x-ref: $cb6e
                    tax 
                    asl a
                    sty a_temp_reg_y
                    tay 
                    lda f_enemy_init_coordinate_index,x
                    tax 
                    lda a_enemy_xpos,x   ; Enemy initial X positions (5 entries). Set from level config, used by init_sprite_movement.
                    sta $d004,y          ; Sprite 2 X Pos
                    ldy a_enemy_init_index
                    lda f_enemy_x_msb_init_table,y
                    eor #$ff
                    and $d010            ; Sprites 0-7 MSB of X coordinate
                    sta $d010            ; Sprites 0-7 MSB of X coordinate
                    lda a_enemy_xmsb,x
                    ora $d010            ; Sprites 0-7 MSB of X coordinate
                    sta $d010            ; Sprites 0-7 MSB of X coordinate
                    ldy a_temp_reg_y
                    jmp s_init_enemy_path_steps
; === Enemy Secondary Phase Path Initializer ===
; Checks if the secondary wave path routing threshold is active.
; If enabled, hashes watermark signatures and loops to pre-calculate secondary offset step vectors.
b_enemy_path_limit_check
                    lda f_enemy_path_step_limit,x ; x-ref: $cb57, $cb73
                    cmp #$00
                    beq r_enemy_path_init_rts
                    jsr s_increment_watermark ; Query copy protection watermark checker
; === Pre-Calculate Enemy Secondary Path Steps Loop ===
; Pre-calculates the secondary directional steps of the enemy wave by calling coordinate updates in a loop before the sprite is displayed.
s_init_enemy_secondary_path_steps
                    lda #$00             ; x-ref: $cc0a
                    sta a_enemy_init_slot_index
b_enemy_secondary_path_step_loop
                    ldx a_enemy_init_index ; x-ref: $cbf1
                    lda f_enemy_init_coordinate_index,x
                    tax 
                    lda a_enemy_init_slot_index
                    cmp f_enemy_path_step_limit,x ; Compare with secondary path step limit threshold
                    beq b_enemy_secondary_path_step_done
                    dey 
                    cpy #$00
                    beq r_enemy_path_init_rts
                    inc a_enemy_init_slot_index
                    lda #$01
                    sty a_temp_reg_y
                    ldx a_enemy_init_index
                    inx 
                    inx 
                    jsr s_update_sprite_coordinates
                    ldy a_temp_reg_y
                    jmp b_enemy_secondary_path_step_loop
b_enemy_secondary_path_step_done
                    lda a_enemy_init_index ; x-ref: $cbd7
                    tax 
                    asl a
                    sty a_temp_reg_y
                    tay 
                    lda f_enemy_init_coordinate_index,x
                    tax 
                    lda a_enemy_ypos,x
                    sta $d005,y          ; Sprite 2 Y Pos
                    ldy a_temp_reg_y
                    jmp s_init_enemy_secondary_path_steps
r_enemy_path_init_rts
                    rts                  ; x-ref: $cbc0, $cbdc
                    .byte $ea, $ea, $ea
