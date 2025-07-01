

@KBD
//CREATE Loop to make screen black screen has 32 registers
@SCREEN
D=A
@baseaddr
M=D

//loop to all  32 addr making them black
@8192
D=A
@count
M=D
@black
M=-1


(LOOP)
@black
D=M
@baseaddr
A=M
M=D
//increment A and decrease count 
@baseaddr
M=M+1
@count
M=M-1
D=M
@LOOP
D;JGT

(END)
@END    
0;JMP