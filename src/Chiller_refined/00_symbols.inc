;==============================================================================
; GLOBAL SYMBOL INDEX
;==============================================================================
;
; This file is intentionally non-emitting: it defines the addresses used across
; the modular disassembly before any bytes are assembled. The grouped includes
; make it easier to find whether a name refers to zero page scratch, low-memory
; runtime state, game assets, colour RAM, or high-memory loader data.
;
.include "symbols/00_header_and_zero_page.inc"
.include "symbols/10_low_memory_runtime.inc"
.include "symbols/20_game_asset_addresses.inc"
.include "symbols/30_colour_ram_and_highmem.inc"
