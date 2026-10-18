lw 1 , 0 , count
addi 2 , 0 , 0
addi 3 , 0 , 1
loop add 2 , 2 , 3
addi 1 , 1 , -1
beq 1 , 0 , exit
j loop
exit sw 2 , 0 , result
halt
count .fill 5
result .fill 0