; Externalized sprite bitmap data. The following small inline tail remains in
; the exported ASM because Regenerator identified it outside the external block.
;
.binary "../assets/chiller_refined_asset_0b00_0feb_sprites.bin"
                    .byte $00, $00, $a8, $00, $00, $aa, $00, $02
                    .byte $8a, $00, $02, $82, $80, $00, $80, $80
                    .byte $03, $c3, $c0, $ff
;==============================================================================
; $0B00-$29FF - ENEMY SPRITES AND EARLY GRAPHICS/DATA ASSET BLOCKS
;==============================================================================
;
; Externalized sprite and graphics/data blocks. Execution falls through into
; player collision code immediately after the $1000-$29FF include.
;
;==============================================================================
; $1000-$29FF - GRAPHICS/DATA ASSET BLOCK, THEN PLAYER COLLISION ENTRY
;==============================================================================
;
; Large externalized graphics/data region. Execution resumes immediately after
; this include at the player-position/collision code. This is one of the places
; where visual asset bytes and executable code sit next to each other in memory.
;
.binary "../assets/chiller_refined_asset_1000_29ff_graphics_data.bin"
