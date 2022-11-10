0000 set R0, #46
0004 set R1, #1
0008 set R4, #0
0012 set R5, #1
0016 sub R2, R0, R1
0020 bgez R2, #0028
0024 ret
0028 add R3, R4, R5
0032 add R4, R5, #0
0036 add R5, R3, #0
0040 add R1, R1, #1
0044 set R6, #0
0048 blez R6, #0016
