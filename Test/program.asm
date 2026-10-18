lw 1 , 0 , neg1          # load array size
addi 2 , 0 , 0           # r2 = index
loop sw 2 , 2 , array    # array[index] = index
addi 2 , 2 , 1           # index++
slt 3 , 2 , 1            # check if index < size
beq 3 , 1 , loop         # repeat while true
halt                     # stop execution
size .fill 4             # number of elements
array .space 4           # reserve array
neg1 .fill -1