;==============================================================================
; CHILLER REFINED - MODULAR BUILDABLE DISASSEMBLY
;==============================================================================
;
; Buildable source entrypoint for Chiller Refined, a gameplay-focused refinement
; of Chiller (1985)(Mastertronic).
;
; Goals:
;   - keep the project navigable by subsystem / memory region
;   - preserve fixed-address behavior unless a routine is proven safe to move
;   - make future movement, map, enemy, and balance changes easier to review
;
; Build:
;   ../tools/build.sh
;
; Layout:
;   Chiller_refined/00_symbols.inc
;     Global labels/equates for zero page, screen RAM, hardware aliases,
;     exported assets, state tables, and known routines.
;
;   Chiller_refined/10_bootstrap_player.inc
;     BASIC stub, bootstrap, ANTISOFT watermark, initial $CE/$CF data seeds,
;     sprite asset include, graphics include, and player movement/collision.
;
;   Chiller_refined/20_midgame_music.inc
;     Zero-init/custom font include, level tables, game state, health/score,
;     object logic, SID setup, and music IRQ command dispatcher.
;
;   Chiller_refined/30_level_title_assets.inc
;     Music/level external include, level setup, title/menu/high-score flow,
;     HUD assets, and level graphics external blocks.
;
;   Chiller_refined/40_gameplay_core_c000.inc
;     Dense $C000 gameplay core: screen helpers, collision, input, block copy,
;     scrolling, sprite/enemy/projectile logic, and $CFxx live state tables.
;
;   Chiller_refined/50_loader_vector_tail.inc
;     External $CFBE-$EA30 data, IRQ/KERNAL-adjacent block, and loader/vector
;     overlay tail.
;
; Refactor discipline:
;   This first modular pass keeps emitted bytes in the same memory order. Future
;   passes can move small routines into more thematic files only when they have
;   no fixed-address dependency, no SMC operand dependency, no fall-through
;   dependency, and no copied-code dependency.
;
;==============================================================================

.include "Chiller_refined/00_symbols.inc"
.include "Chiller_refined/10_bootstrap_player.inc"
.include "Chiller_refined/20_midgame_music.inc"
.include "Chiller_refined/30_level_title_assets.inc"
.include "Chiller_refined/40_gameplay_core_c000.inc"
.include "Chiller_refined/50_loader_vector_tail.inc"
