.ORIG x3000
AND R0 R0 0 ; R0 is the spill value
LD R1 orgbit ; R1 is the original bit string 
LD R2 mask ; R2 is the mask 
LD R4 numsft  ;R4 holds the number of shifts
aloop
AND R3 R1 R2
BRP pos
BRN pos
;left-most was zero
ADD R0  R0  R0
BR done
pos
;left-most was 1
ADD R0  R0  R0 ;left-shifting spillover bitstring
ADD R0  R0  1
done
;we've finished using oribit for spillover, so shift origbit
ADD R1 R1 R1
ADD R4 R4 -1  ;decrease the remaining shifts
BRP aloop
;now, the results in R0 and R1 are complete...store them
ST  R0 spill
ST  R1 modorig

orgbit .FILL 45056
numsft .FILL 4   ;specify the number of shifts
mask   .FILL x8000
spill      .BLKW 1
modorig    .BLKW 1

.END