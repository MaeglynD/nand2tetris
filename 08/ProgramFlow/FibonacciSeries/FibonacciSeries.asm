@256
		D=A
		@SP
		M=D
	 // argument1
@ARG
D=M
@1
A=D+A
D=M
@SP
M=M+1
A=M-1
M=D
 // pointer1
@THIS
D=A
@1
D=D+A
@translator_temp
M=D
@SP
M=M-1
A=M
D=M
@translator_temp
A=M
M=D
 // constant0
@0
D=A
@SP
M=M+1
A=M-1
M=D
 // that0
@THAT
D=M
@0
D=D+A
@translator_temp
M=D
@SP
M=M-1
A=M
D=M
@translator_temp
A=M
M=D
 // constant1
@1
D=A
@SP
M=M+1
A=M-1
M=D
 // that1
@THAT
D=M
@1
D=D+A
@translator_temp
M=D
@SP
M=M-1
A=M
D=M
@translator_temp
A=M
M=D
 // argument0
@ARG
D=M
@0
A=D+A
D=M
@SP
M=M+1
A=M-1
M=D
 // constant2
@2
D=A
@SP
M=M+1
A=M-1
M=D
 // sub
@SP
M=M-1
A=M
D=M
A=A-1
M=M-D
 // argument0
@ARG
D=M
@0
D=D+A
@translator_temp
M=D
@SP
M=M-1
A=M
D=M
@translator_temp
A=M
M=D
(FibonacciSeries$MAIN_LOOP_START)
 // argument0
@ARG
D=M
@0
A=D+A
D=M
@SP
M=M+1
A=M-1
M=D
@SP
M=M-1
A=M
D=M
@FibonacciSeries$COMPUTE_ELEMENT
D;JNE
@FibonacciSeries$END_PROGRAM
0;JMP

(FibonacciSeries$COMPUTE_ELEMENT)
 // that0
@THAT
D=M
@0
A=D+A
D=M
@SP
M=M+1
A=M-1
M=D
 // that1
@THAT
D=M
@1
A=D+A
D=M
@SP
M=M+1
A=M-1
M=D
 // add
@SP
M=M-1
A=M
D=M
A=A-1
M=D+M
 // that2
@THAT
D=M
@2
D=D+A
@translator_temp
M=D
@SP
M=M-1
A=M
D=M
@translator_temp
A=M
M=D
 // pointer1
@THIS
D=A
@1
A=D+A
D=M
@SP
M=M+1
A=M-1
M=D
 // constant1
@1
D=A
@SP
M=M+1
A=M-1
M=D
 // add
@SP
M=M-1
A=M
D=M
A=A-1
M=D+M
 // pointer1
@THIS
D=A
@1
D=D+A
@translator_temp
M=D
@SP
M=M-1
A=M
D=M
@translator_temp
A=M
M=D
 // argument0
@ARG
D=M
@0
A=D+A
D=M
@SP
M=M+1
A=M-1
M=D
 // constant1
@1
D=A
@SP
M=M+1
A=M-1
M=D
 // sub
@SP
M=M-1
A=M
D=M
A=A-1
M=M-D
 // argument0
@ARG
D=M
@0
D=D+A
@translator_temp
M=D
@SP
M=M-1
A=M
D=M
@translator_temp
A=M
M=D
@FibonacciSeries$MAIN_LOOP_START
0;JMP

(FibonacciSeries$END_PROGRAM)
