;==============================================================================
; $CFBE-$EA32 - HIGH-MEMORY OVERLAY AND ENTRY BYTE
;==============================================================================
;
; Broad external block ending just before the KERNAL-adjacent loader area. The
; single decoded byte at $EA31 is referenced from music/loader-adjacent code and
; is kept here to preserve the exact memory order.

.binary "../../chiller_refined_asset_cfbe_ea30_overlay.bin"
j_EA31              ora (zp_sound_param_ptr,x) ; x-ref: $60c7, $6179, $617f
