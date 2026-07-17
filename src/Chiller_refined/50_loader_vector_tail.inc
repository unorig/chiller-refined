;==============================================================================
; LOADER / VECTOR TAIL GUIDE
;==============================================================================
;
; This final region contains external data and code deliberately placed in
; addresses that normally look like KERNAL space. Treat it as fixed-location
; loader/trampoline material unless a future pass proves otherwise.
;
.include "loader_tail/00_cfbe_to_ea32_overlay.inc"
.include "loader_tail/10_ea33_to_ff3a_overlay.inc"
.include "loader_tail/20_ff3b_loader_vector_tail.inc"
