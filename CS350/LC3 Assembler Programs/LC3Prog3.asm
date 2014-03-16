.ORIG x3000

LEA R0 prompt ;print a prompt to a user to search for a char and read it in
PUTS
GETC
OUT 
ADD R5 R0 0 ; store R0 in R5
AND R4 R4 0 ;R4 is the counter
LEA R0 prompt1
PUTS

stringloop
LD R0 value ; continuously load R0 with 0
LD R6 value ; make R6 continuously 0, this register checks if the occurences is greater than 9
GETC     ; read char in from the string we are considering and store in R0
OUT      ; print out what we typed
NOT R1 R0 
ADD R1 R1 1 ; negate R0 and store in R1
ADD R7 R1 0 ; put it in R7
ADD R7 R7 R5    ; 0 if old R0 and R5 are the same chars
BRNP shit ; if R7 is 0, go to next line. otherwise, go to shit to check if it was the end of line char
ADD R4 R4 1  ; only executes if chars match         
ADD R6 R4 -9    ; if R6 is 0, then branch out (there were nine occurences)
BRZ bleh  
BRNP stringloop    
shit 
ADD R0 R0 -10  ; where NOT A (10, newline) is the negation of the end of line chacter
BRZ numTimes    ; branch to end to output because we reached end of line
BRNP stringloop ; true iff r0 was the end of line character, branches otherwise


numTimes
LEA R0 prompt2
PUTS
AND R0 R0 0
LD R3 value1
ADD R0 R4 R3
OUT 
BR done

bleh
LEA R0 prompt3
PUTS


done
HALT

prompt  .STRINGZ "Please enter a character to search for: "
prompt1 .STRINGZ "Please enter a string to search through: "
prompt2 .STRINGZ "The count was: "
prompt3 .STRINGZ "There were more than 9 occurences."
value .FILL 0
value1 .FILL 48
HALT
.END