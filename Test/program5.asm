lw 1 , 0 , data1
lw 2 , 0 , data2
or 3 , 1 , 2
nand 4 , 1 , 2
slti 5 , 3 , 20
beq 5 , 0 , label
add 6 , 3 , 4
label sub 7 , 4 , 3
halt
data1 .fill 15
data2 .fill 240