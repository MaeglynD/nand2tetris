@256
		D=A
		@SP
		M=D
		@Sys.init$return.0
		D=A
		@SP
		M=M+1
		A=M-1
		M=D
		@LCL
		D=M
		@SP
		M=M+1
		A=M-1
		M=D
		@ARG
		D=M
		@SP
		M=M+1
		A=M-1
		M=D
		@THIS
		D=M
		@SP
		M=M+1
		A=M-1
		M=D
		@THAT
		D=M
		@SP
		M=M+1
		A=M-1
		M=D
		@SP
		D=M
		@5
		D=D-A
		@ARG
		M=D
		@SP
		D=M
		@LCL
		M=D
		@Sys.init
		0;JMP
		(Sys.init$return.0)
		//
(Sys.init)
//
 // constant4000
@4000
D=A
@SP
M=M+1
A=M-1
M=D
//
 // pointer0
@THIS
D=A
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
//
 // constant5000
@5000
D=A
@SP
M=M+1
A=M-1
M=D
//
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
//
@Sys.main$return.12
D=A
@SP
M=M+1
A=M-1
M=D
@LCL
D=M
@SP
M=M+1
A=M-1
M=D
@ARG
D=M
@SP
M=M+1
A=M-1
M=D
@THIS
D=M
@SP
M=M+1
A=M-1
M=D
@THAT
D=M
@SP
M=M+1
A=M-1
M=D
@SP
D=M
@0
D=D-A
@5
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Sys.main
0;JMP
(Sys.main$return.12)
//
 // temp1
@5
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
//
(Sys.init$LOOP)
//
@Sys.init$LOOP
0;JMP

//
(Sys.main)
@SP
M=M+1
A=M-1
M=0
@SP
M=M+1
A=M-1
M=0
@SP
M=M+1
A=M-1
M=0
@SP
M=M+1
A=M-1
M=0
@SP
M=M+1
A=M-1
M=0
//
 // constant4001
@4001
D=A
@SP
M=M+1
A=M-1
M=D
//
 // pointer0
@THIS
D=A
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
//
 // constant5001
@5001
D=A
@SP
M=M+1
A=M-1
M=D
//
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
//
 // constant200
@200
D=A
@SP
M=M+1
A=M-1
M=D
//
 // local1
@LCL
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
//
 // constant40
@40
D=A
@SP
M=M+1
A=M-1
M=D
//
 // local2
@LCL
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
//
 // constant6
@6
D=A
@SP
M=M+1
A=M-1
M=D
//
 // local3
@LCL
D=M
@3
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
//
 // constant123
@123
D=A
@SP
M=M+1
A=M-1
M=D
//
@Sys.add12$return.37
D=A
@SP
M=M+1
A=M-1
M=D
@LCL
D=M
@SP
M=M+1
A=M-1
M=D
@ARG
D=M
@SP
M=M+1
A=M-1
M=D
@THIS
D=M
@SP
M=M+1
A=M-1
M=D
@THAT
D=M
@SP
M=M+1
A=M-1
M=D
@SP
D=M
@1
D=D-A
@5
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Sys.add12
0;JMP
(Sys.add12$return.37)
//
 // temp0
@5
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
//
 // local0
@LCL
D=M
@0
A=D+A
D=M
@SP
M=M+1
A=M-1
M=D
//
 // local1
@LCL
D=M
@1
A=D+A
D=M
@SP
M=M+1
A=M-1
M=D
//
 // local2
@LCL
D=M
@2
A=D+A
D=M
@SP
M=M+1
A=M-1
M=D
//
 // local3
@LCL
D=M
@3
A=D+A
D=M
@SP
M=M+1
A=M-1
M=D
//
 // local4
@LCL
D=M
@4
A=D+A
D=M
@SP
M=M+1
A=M-1
M=D
//
 // add
@SP
M=M-1
A=M
D=M
A=A-1
M=D+M
//
 // add
@SP
M=M-1
A=M
D=M
A=A-1
M=D+M
//
 // add
@SP
M=M-1
A=M
D=M
A=A-1
M=D+M
//
 // add
@SP
M=M-1
A=M
D=M
A=A-1
M=D+M
//
@LCL
D=M
@FRAME
M=D
@5
A=D-A
D=M
@RET
M=D
@SP
M=M-1
A=M
D=M
@ARG
A=M
M=D
@ARG
D=M+1
@SP
M=D
@FRAME
A=M-1
D=M
@THAT
M=D
@2
D=A
@FRAME
A=M-D
D=M
@THIS
M=D
@3
D=A
@FRAME
A=M-D
D=M
@ARG
M=D
@4
D=A
@FRAME
A=M-D
D=M
@LCL
M=D
@RET
A=M
0;JMP
//
(Sys.add12)
//
 // constant4002
@4002
D=A
@SP
M=M+1
A=M-1
M=D
//
 // pointer0
@THIS
D=A
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
//
 // constant5002
@5002
D=A
@SP
M=M+1
A=M-1
M=D
//
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
//
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
//
 // constant12
@12
D=A
@SP
M=M+1
A=M-1
M=D
//
 // add
@SP
M=M-1
A=M
D=M
A=A-1
M=D+M
//
@LCL
D=M
@FRAME
M=D
@5
A=D-A
D=M
@RET
M=D
@SP
M=M-1
A=M
D=M
@ARG
A=M
M=D
@ARG
D=M+1
@SP
M=D
@FRAME
A=M-1
D=M
@THAT
M=D
@2
D=A
@FRAME
A=M-D
D=M
@THIS
M=D
@3
D=A
@FRAME
A=M-D
D=M
@ARG
M=D
@4
D=A
@FRAME
A=M-D
D=M
@LCL
M=D
@RET
A=M
0;JMP