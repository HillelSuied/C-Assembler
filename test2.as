.entry LENGTH
.extern W
MAIN: mov @r3 ,LENGTH
LOOP: jmp L5
pprn -5
bne W
sb @r1, @r4
bne L3
L1: inc K
.entry LOOP
jmp W
E/D: stop
STR: .string "abcdef"
LENGTH: .data A,-9,15
K: .data 22
.extern L3
