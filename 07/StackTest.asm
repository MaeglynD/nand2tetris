@256
D=A
@SP
M=D
@17
D=A
@SP
M=M+1
A=M-1
M=D
@17
D=A
@SP
M=M+1
A=M-1
M=D
@SP
M=M-1
A=M
D=M
A=A-1
D=D-M
@EQ_TRUE_0
D;JEQ
@SP
A=M-1
M=0
@EQ_END_0
0;JMP
(EQ_TRUE_0)
@SP
A=M-1
M=-1
(EQ_END_0)
@17
D=A
@SP
M=M+1
A=M-1
M=D
@16
D=A
@SP
M=M+1
A=M-1
M=D
@SP
M=M-1
A=M
D=M
A=A-1
D=D-M
@EQ_TRUE_0
D;JEQ
@SP
A=M-1
M=0
@EQ_END_0
0;JMP
(EQ_TRUE_0)
@SP
A=M-1
M=-1
(EQ_END_0)
@16
D=A
@SP
M=M+1
A=M-1
M=D
@17
D=A
@SP
M=M+1
A=M-1
M=D
@SP
M=M-1
A=M
D=M
A=A-1
D=D-M
@EQ_TRUE_0
D;JEQ
@SP
A=M-1
M=0
@EQ_END_0
0;JMP
(EQ_TRUE_0)
@SP
A=M-1
M=-1
(EQ_END_0)
@892
D=A
@SP
M=M+1
A=M-1
M=D
@891
D=A
@SP
M=M+1
A=M-1
M=D
@SP
M=M-1
A=M
D=M
A=A-1
D=M-D
@LT_TRUE_0
D;JLT
@SP
A=M-1
M=0
@LT_END_0
0;JMP
(LT_TRUE_0)
@SP
A=M-1
M=-1
(LT_END_0)
@891
D=A
@SP
M=M+1
A=M-1
M=D
@892
D=A
@SP
M=M+1
A=M-1
M=D
@SP
M=M-1
A=M
D=M
A=A-1
D=M-D
@LT_TRUE_0
D;JLT
@SP
A=M-1
M=0
@LT_END_0
0;JMP
(LT_TRUE_0)
@SP
A=M-1
M=-1
(LT_END_0)
@891
D=A
@SP
M=M+1
A=M-1
M=D
@891
D=A
@SP
M=M+1
A=M-1
M=D
@SP
M=M-1
A=M
D=M
A=A-1
D=M-D
@LT_TRUE_0
D;JLT
@SP
A=M-1
M=0
@LT_END_0
0;JMP
(LT_TRUE_0)
@SP
A=M-1
M=-1
(LT_END_0)
@32767
D=A
@SP
M=M+1
A=M-1
M=D
@32766
D=A
@SP
M=M+1
A=M-1
M=D
@SP
M=M-1
A=M
D=M
A=A-1
D=M-D
@GT_TRUE_0
D;JGT
@SP
A=M-1
M=0
@GT_END_0
0;JMP
(GT_TRUE_0)
@SP
A=M-1
M=-1
(GT_END_0)
@32766
D=A
@SP
M=M+1
A=M-1
M=D
@32767
D=A
@SP
M=M+1
A=M-1
M=D
@SP
M=M-1
A=M
D=M
A=A-1
D=M-D
@GT_TRUE_0
D;JGT
@SP
A=M-1
M=0
@GT_END_0
0;JMP
(GT_TRUE_0)
@SP
A=M-1
M=-1
(GT_END_0)
@32766
D=A
@SP
M=M+1
A=M-1
M=D
@32766
D=A
@SP
M=M+1
A=M-1
M=D
@SP
M=M-1
A=M
D=M
A=A-1
D=M-D
@GT_TRUE_0
D;JGT
@SP
A=M-1
M=0
@GT_END_0
0;JMP
(GT_TRUE_0)
@SP
A=M-1
M=-1
(GT_END_0)
@57
D=A
@SP
M=M+1
A=M-1
M=D
@31
D=A
@SP
M=M+1
A=M-1
M=D
@53
D=A
@SP
M=M+1
A=M-1
M=D
@SP
M=M-1
A=M
D=M
A=A-1
M=D+M
@112
D=A
@SP
M=M+1
A=M-1
M=D
@SP
M=M-1
A=M
D=M
A=A-1
M=M-D
@SP
A=M-1
M=-M
@SP
M=M-1
A=M
D=M
A=A-1
M=D&M
@82
D=A
@SP
M=M+1
A=M-1
M=D
@SP
M=M-1
A=M
D=M
A=A-1
M=D|M
@SP
A=M-1
M=!M
