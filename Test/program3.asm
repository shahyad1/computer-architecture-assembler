lw 1 , 0 , count          # load array size into r1
addi 2 , 0 , 0            # r2 = index = 0
addi 3 , 0 , 5            # r3 = value to store

loop sw 3 , 2 , numbers   # numbers[index] = 5
addi 2 , 2 , 1            # index++
slti 4 , 2 , 1             # r4 = (index < count)
beq 4 , 1 , loop          # if true, continue loop

addi 5 , 0 , 100          # load an unused constant
addi 6 , 5 , -50          # r6 = 50
addi 7 , 6 , 10           # r7 = 60
halt                      # end program

count .fill 4             # array size
numbers .space 4          # array of 4 elements