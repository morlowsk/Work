.ORIG x3000

AND R0 R0 0
ADD R0 R0 1
LD  R1 N
BRZ storethis
Loop
ADD R0 R0 R0
ADD R1 R1 -1
BRP Loop
storethis
ST R0 result

N   .FILL 2
result .BLKW 1

.END
