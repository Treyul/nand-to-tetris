// multiply  number in RO and R1 and store value in R2

// initialize count and i variables
@R1
D=M
@i
M=D
@count
M=0
@R2
M=0
//create loop: load i then minus count i equivalent to 0 ump to end o program
(LOOP)
@i
D=M
@count
D=D-M
@END
D;JEQ

// do summation o R0 to R2
@R0
D=M
@R2
M=D+M

//increment count
@count
M=M+1
@LOOP
0;JMP

(END)
@END
0;JMP