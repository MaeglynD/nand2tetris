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
@SP
M=M-1
A=M
D=M-D
@StackTest.cmp.0.true
D;JEQ
@SP
A=M
M=0
@StackTest.cmp.0.end
0;JMP
(StackTest.cmp.0.true)
@SP
A=M
M=-1
(StackTest.cmp.0.end)
@SP
M=M+1
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
@SP
M=M-1
A=M
D=M-D
@StackTest.cmp.1.true
D;JEQ
@SP
A=M
M=0
@StackTest.cmp.1.end
0;JMP
(StackTest.cmp.1.true)
@SP
A=M
M=-1
(StackTest.cmp.1.end)
@SP
M=M+1
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
@SP
M=M-1
A=M
D=M-D
@StackTest.cmp.2.true
D;JEQ
@SP
A=M
M=0
@StackTest.cmp.2.end
0;JMP
(StackTest.cmp.2.true)
@SP
A=M
M=-1
(StackTest.cmp.2.end)
@SP
M=M+1
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
@SP
M=M-1
A=M
D=M-D
@StackTest.cmp.3.true
D;JLT
@SP
A=M
M=0
@StackTest.cmp.3.end
0;JMP
(StackTest.cmp.3.true)
@SP
A=M
M=-1
(StackTest.cmp.3.end)
@SP
M=M+1
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
@SP
M=M-1
A=M
D=M-D
@StackTest.cmp.4.true
D;JLT
@SP
A=M
M=0
@StackTest.cmp.4.end
0;JMP
(StackTest.cmp.4.true)
@SP
A=M
M=-1
(StackTest.cmp.4.end)
@SP
M=M+1
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
@SP
M=M-1
A=M
D=M-D
@StackTest.cmp.5.true
D;JLT
@SP
A=M
M=0
@StackTest.cmp.5.end
0;JMP
(StackTest.cmp.5.true)
@SP
A=M
M=-1
(StackTest.cmp.5.end)
@SP
M=M+1
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
@SP
M=M-1
A=M
D=M-D
@StackTest.cmp.6.true
D;JGT
@SP
A=M
M=0
@StackTest.cmp.6.end
0;JMP
(StackTest.cmp.6.true)
@SP
A=M
M=-1
(StackTest.cmp.6.end)
@SP
M=M+1
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
@SP
M=M-1
A=M
D=M-D
@StackTest.cmp.7.true
D;JGT
@SP
A=M
M=0
@StackTest.cmp.7.end
0;JMP
(StackTest.cmp.7.true)
@SP
A=M
M=-1
(StackTest.cmp.7.end)
@SP
M=M+1
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
@SP
M=M-1
A=M
D=M-D
@StackTest.cmp.8.true
D;JGT
@SP
A=M
M=0
@StackTest.cmp.8.end
0;JMP
(StackTest.cmp.8.true)
@SP
A=M
M=-1
(StackTest.cmp.8.end)
@SP
M=M+1
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
@SP
A=M-1
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
@SP
A=M-1
M=M-D
@SP
M=M-1
A=M
M=-M
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@SP
A=M-1
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
@SP
A=M-1
M=D|M
@SP
M=M-1
A=M
M=!M
@SP
M=M+1
