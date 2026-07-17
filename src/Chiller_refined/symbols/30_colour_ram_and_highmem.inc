;==============================================================================
; COLOUR RAM, I/O-ADJACENT ALIASES, AND HIGH-MEMORY DATA
;==============================================================================
;
; $D800 colour RAM aliases are used by rendering and title/menu setup. The
; $DDDD/$E000/$FBFD aliases are high-memory data references touched by the
; loader/vector tail and late gameplay code.

f_D800 = $d800                           ; x-ref: $72f7, $c2dc
a_D805 = $d805                           ; x-ref: $573e
a_D81A = $d81a                           ; x-ref: $2c5d
a_D824 = $d824                           ; x-ref: $c4d7
a_D825 = $d825                           ; x-ref: $c4da
a_D828 = $d828                           ; x-ref: $c412, $c429
f_D829 = $d829                           ; x-ref: $c426
a_D84C = $d84c                           ; x-ref: $c4e6
a_D84D = $d84d                           ; x-ref: $c4e3
a_D84F = $d84f                           ; x-ref: $c4c1
a_D874 = $d874                           ; x-ref: $c4ef
a_D875 = $d875                           ; x-ref: $c4ec
a_color_mastertronic_char = $d8ba        ; x-ref: $72e7
f_D8D6 = $d8d6                           ; x-ref: $5c79
f_D8F9 = $d8f9                           ; x-ref: $c43c
f_D8FA = $d8fa                           ; x-ref: $c439
f_D900 = $d900                           ; x-ref: $c2df
f_D9EF = $d9ef                           ; x-ref: $2c51, $2c60, $2c99
f_D9F3 = $d9f3                           ; x-ref: $c44f
f_D9F4 = $d9f4                           ; x-ref: $c44c
f_DA00 = $da00                           ; x-ref: $c2e2
a_DA5C = $da5c                           ; x-ref: $75e0
f_DAED = $daed                           ; x-ref: $c462
f_DAEE = $daee                           ; x-ref: $c45f
f_DB00 = $db00                           ; x-ref: $c2e5
f_DBCA = $dbca                           ; x-ref: $5741
f_DDDD = $dddd                           ; x-ref: $5dc0, $5dc3, $5dc6, $5dc9, $5dcc, ...
level_screen_maps = $e000
f_E31C = $e31c                           ; x-ref: $ce46
f_FBFD = $fbfd                           ; x-ref: $c9e1
