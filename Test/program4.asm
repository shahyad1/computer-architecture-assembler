lui 1 , 2
addi 2 , 0 , 10
lw 4 , 0 , funcPtr
jalr 3 , 4
add 5 , 1 , 2
halt
funcPtr .fill myFunc
myFunc sub 1 , 1 , 2
addi 2 , 2 , 5
jalr 0 , 3