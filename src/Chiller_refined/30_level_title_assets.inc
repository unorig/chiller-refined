;==============================================================================
; $61F2-$BFFF - LEVEL/TITLE FLOW AND EXTERNAL LEVEL ASSETS GUIDE
;==============================================================================
;
; This region handles level config pointers, title/top-row setup, menu color
; behavior, level screen setup, high-score copying, scrolling ticks, the main
; game-loop tick entry, and the large level graphics/map external assets.
;
; Files:
;   level_title/00_level_config_damage_titlebar.inc
;     External $61F2-$727F block, level config pointer table, bankswitch copy,
;     damage border flashing, and top-row title text setup.
;
;   level_title/10_menu_color_irq_entry.inc
;     External $7300-$757F block, menu/background color toggles, stop-key patch,
;     border color routing, and level-cross initialization.
;
;   level_title/20_screen_level_setup.inc
;     Level screen setup, screen buffer copy, level asset copy, sprite setup, and
;     default border/sprite enabling.
;
;   level_title/30_highscore_title_text.inc
;     High-score temp bytes, background color driver, high-score compare/update,
;     and top-row title text.
;
;   level_title/40_scroll_loop_level_assets.inc
;     HUD/text external blocks, sprite position setters, magic-cross checks,
;     level scroll tick, game_loop_tick, and external level graphics assets.
;
;==============================================================================

.include "level_title/00_level_config_damage_titlebar.inc"
.include "level_title/10_menu_color_irq_entry.inc"
.include "level_title/20_screen_level_setup.inc"
.include "level_title/30_highscore_title_text.inc"
.include "level_title/40_scroll_loop_level_assets.inc"
