#format is target-name: target dependencies
#{-tab-}actions

# All Targets
all: main

# Tool invocations
# Executable "program" depends on the files program.o and run.o.
main: main.o calc.o
	gcc -g -Wall -o main main.o calc.o

# Depends on the source and header files
main.o: main.c
	gcc -g -Wall -c -o main.o main.c 
	
calc.o: calc.s
	nasm -g -f elf64 -w+all -o calc.o calc.s
#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o main
	
	
# mul:                      
#         push rbp
#         mov rbp, rsp
#         call prep_mul
#         
#     prep_mul:
#         mov r9, 1
#         mov r10, 0
#         mov r11, 10
#         mov qword [carry], r10
#         mov qword [i], rdx
#         mov qword [j], rdx
#         mov qword [temp], r10
#         mov qword [first], 0
#         mov qword [second], 0
#         mov qword [numdigits], rdx
#         jmp actual_mult
#         
#     i_iteration:
#         sub rcx, r9
#         sub [i], r9
#         dec rdi
#         jmp actual_mult
#         
#     actual_mult:
#         mov ah, [rdi]
#         mov [first], ah
#         mov ah, [rsi]
#         mov [second], ah
#         mov rax, 0
#         mov rax, [first]
#         sub rax, '0'
#         mov rdx, 0
#         mov rdx, [second]
#         sub rdx, '0'
#         mul rdx
#         add rax, [carry]
#         mov rdx, 0
#         div r11
#         mov [carry], r10
#         mov [carry], rax
#         mov [temp], rdx
#         mov al, [temp]
#         add [rcx], al
#         mov al, [rcx]
#         cmp al, '9'
#         jg more_carry
#         jmp iteration_mul
#         
#     after_mul:
#         sub rcx, r9
#         mov  al, [carry]
#         add [rcx], al
#         add rcx, [numdigits]
#         add rdi, [numdigits]
#         mov al, [numdigits]
#         mov [i], al
#         jmp big_iteration
#         
#     iteration_mul:
#         cmp [i], r10
#         jg i_iteration
#         jmp after_mul
#     
#     big_iteration:    
#         cmp [j], r10
#         je finish_mul
#         dec rsi
#         sub [j], r9
#         jmp actual_mult
#     i_loop:
#         inc rdi
#         sub [i], r9
#         cmp [i], r10
#         jnz i_loop
#         jmp actual_mult
#         
#         
#     more_carry:
#         sub al, 10
#         add [carry], r9
#         mov [rcx], al
#         jmp iteration_mul
#         
#     finish_mul:
#         mov rax, [numdigits]
#         mov rsp, rbp
#         pop rbp
#         ret