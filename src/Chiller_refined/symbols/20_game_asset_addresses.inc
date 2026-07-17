;==============================================================================
; GAME DATA, SPRITE, MUSIC, LEVEL, AND TITLE ASSET ADDRESSES
;==============================================================================
;
; These aliases point into the decompressed PRG image and external asset blobs.
; Several are still broad names because the exact data format is only partly
; mapped.

sprites_enemies = $0b00
a_100F = $100f                           ; x-ref: $5455
f_zero_init_buffer = $3000               ; x-ref: $5723
f_zero_init_buf_30A0 = $30a0             ; x-ref: $c07a
f_zero_init_buf_3100 = $3100             ; x-ref: $5726
f_zero_init_buf_3200 = $3200             ; x-ref: $5729
a_zero_init_buf_3245 = $3245             ; x-ref: $c1f4
f_zero_init_buf_3300 = $3300             ; x-ref: $572c
f_custom_font_data = $3400
f_custom_font_data_34E8 = $34e8          ; x-ref: $5683
f_active_sprites_start = $39c0           ; x-ref: $2b21
f_sprite_0_active_bitmap = $3c00
f_sprite_0_mirror_buffer = $3c40         ; x-ref: $c152, $c167, $c54b, $c5f9
f_sprite_movement_paths = $4000          ; x-ref: $c386, $c3ce
music_data_stream = $61f2
s_6401 = $6401                           ; x-ref: $5a07
level_cinema_config = $7000              ; Cinema level config: charset $8000-$83FF, screen map $8400, colour $8800. Loaded via bankswitch_block_copy. ExtraBG1=$01, ExtraBG2=$07. ; x-ref: $7290
level_ghetto_config = $7080              ; x-ref: $7292
level_graveyard_config = $7100           ; x-ref: $7294
level_haunted_house_config = $7180       ; x-ref: $7296
level_forest_config = $7200              ; x-ref: $7298
level_forest_cf69 = $7210
level_forest_sprite0y = $7211
a_shadow_bg_color2 = $7523               ; x-ref: $759e
f_hud_ui_layout_data = $7800
a_7EF0 = $7ef0                           ; x-ref: $7ef1, $7ef9
level_data_all = $8000
f_8422 = $8422                           ; x-ref: $7783, $779a
s_A659 = $a659                           ; x-ref: $fff1
level_data_contd = $a7b1
a_A914 = $a914                           ; x-ref: $c9a0
f_BBBB = $bbbb                           ; x-ref: $a7b0
a_BDCF = $bdcf                           ; x-ref: $cdd4
s_CFC0 = $cfc0                           ; x-ref: $5d98
