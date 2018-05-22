section .data                    	
        an: DQ 0 
        first: DQ 0
        second: DQ 0
        index: DQ 0
        output: DQ 0
        borrow: DQ 0
        carry:  DQ 0             	
        loop_index: DQ 0               
        number_of_digits: DQ 0 
        output_mul: DQ 0 
        temp: DQ 0

section .text                    	
        global add
        global subt
        global mul
        
    prep_add:
        mov r9, 1       ; using r9 to decrementing/incrementing by 1
        mov r10, 0      ; using r10 to assign 0 
        mov [carry], r10
        add rdi, rsi    ; getting the lsb of first number
        add rdx, rcx    ; getting the lsb of second number
        mov [index], rsi    ; moving index to the right value
        add [index], r9
        add [index], r9
        jmp actual_adding
    prep_sub:
        mov r9, 1       ; using r9 to decrementing/incrementing by 1
        mov r10, 0      ; using r10 to assign 0 
        mov [borrow], r10
        add rdi, rsi    ; getting the lsb of first number
        add rdx, rcx    ; getting the lsb of second number
        mov [index], rsi    ; moving index to the right value
        add [index], r9
        add [index], r9
        jmp actual_subtracting
    add:                      
        push rbp
        mov rbp, rsp
        mov qword [index], 0
        mov qword [output], 0
        call prep_add
    subt:
        push rbp
        mov rbp, rsp
        mov qword [index], 0
        mov qword [output], 0
        call prep_sub
    actual_adding:
        mov qword [first], 0    ; reset
        mov qword [second], 0   ; reset
        mov ah, [rdi]           ; getting the first value
        mov [first], ah
        mov ah, [rdx]           ; getting the second value
        mov [second], ah
        mov al, [first]
        sub al, '0'             ; getting the num value
        add al, [second]
        sub al, '0'
        add al, [carry]         ; adding the carry from the last operation
        jmp check_carry
    actual_subtracting:
        mov qword [first], 0    ; reset
        mov qword [second], 0   ; reset
        mov ah, [rdi]           ; getting the first value
        cmp [borrow], r9        ; if borrow is 1, the digit needs to decrease by 1
        jl no_decrease
    decrease:
        sub ah, 1               ; decrease by 1
    no_decrease:
        mov [first], ah         
        mov ah, [rdx]           ; getting the second value
        mov [second], ah
        mov al, [first]         
        sub al, '0'             ; getting the num value
        sub al, [second]
        add al, '0'
        jmp check_borrow
    finish_adding:
        add al, '0'         ; getting the ASCII value
        mov [r8], al        ; putting the result in r8
        add [output], r9    ; counting the number of digits of the resultun
        jmp iteration_add
    finish_subtracting:
        add al, '0'         ; getting the ASCII value
        mov [r8], al        ; putting the result in r8
        add [output], r9    ; counting the number of digits of the resultun
        jmp iteration_sub
    iteration_add:
        dec rsi             ; moving to the next digit of all of the pointers
        dec rdi
        dec rdx
        dec rcx
        sub r8, 1
        sub [index], r9
        cmp [index], r10    ; checking if reached to the last digit
        jg actual_adding
        jmp finish          ; finished adding
    iteration_sub:
        dec rsi             ; moving to the next digit of all of the pointers
        dec rdi
        dec rdx
        dec rcx
        sub r8, 1
        sub [index], r9
        cmp [index], r10    ; checking if reached the last digit
        jg actual_subtracting
        jmp finish
    check_carry:
        cmp al, 10          ; if result is greater than 10  needs carry
        jge needs_carry
        mov [carry], r10
        jmp finish_adding
    check_borrow:
        cmp al, 0           ; if result is negative borrow is needed
        jl needs_borrow
        mov [borrow], r10   ; reset the borrow
        jmp finish_subtracting
    needs_carry:
        sub al, 10          ; getting the lsb
        mov [carry], r9     ; moving 1 to the carry for the next operation
        jmp finish_adding
    needs_borrow:
        add al, 10          ; after getting borrow result increased by 10
        mov [borrow], r9    ; moving 1 to the borrow for the next operation
        jmp finish_subtracting
    finish:
        mov rax, [output]   ; moving num of digits to the return value
        sub rax, 1
        mov rsp, rbp
        pop rbp
        ret
        
        
mul:                        	  
        push    rbp              	
        mov     rbp, rsp         	
        mov	qword [loop_index], 2      
        mov	qword [output_mul], 0   
        mov	qword [carry], 0   
        mov	qword [number_of_digits], rsi   
        jmp     prep_mul

loop_first:
        mov qword [second], '1'       
        cmp     rcx, 0          ; checking if reached the end of the number             
        jl      finish_loop_first ; if reached the end so finish the loop        
        mov     al, [r11]           ; else , store the next digit in al       
        mov     [second], al 
        
finish_loop_first:                  ; for the correct order of the program
        mov     qword [carry], 0    ; reset the carry
        
loop_second:
        mov qword [first], '1'        
        cmp     rsi, 0             ; if reached the end of the number
        jl      finish_loop_second     ; if reached the end, finish the loop      
        mov     ah, [rdi]           ; else , store the next digit in ah
        mov     [first], ah
        
finish_loop_second:
        mov     rax, 0      ; resetting rax
        mov     rax, [first]  ; moving the first digit to rax         
        sub     rax, '0'    ; getting num value       
        mov     rdx, 0      ; resetting rdx        
        mov     rdx, [second]  ; moving the second digit to rdx        
        sub     rdx, '0'    ; getting num value          
        mul     rdx         ; multiply the digits
        add     rax, [carry]   ; adding the carry if exists from last operation    
        mov     rdx, 0           ; reset rdx  
        mov     rbx, 10         ;   moving 10 for div operation
        div     rbx             ; divide the product by 10
        mov     [carry], rax    ; carry carries the msb of the division
        mov     [temp], rdx     ; temp carries the modolu of the divison
        mov     al, [temp]      ; moving the lsb to al
        add     [r8], al        ; storing the lsb in the result  
        mov     al, [r8]        
        cmp     al, '9'         ; checking if extra carry is needed
        jg      more_carry
          
finish_mul:
        dec     rsi     ; decrease the pointer of first number          
        cmp     rsi, 0    ; checking if reached the end of number         
        jl     first_iteration        
        dec     rdi     ; if reached end of first number second number is decreased aswell
        
first_iteration:
        sub     r8, 1      ; decrease the result to the next digit        
        sub     r12, r9    ; decrease the first number   
        cmp     r12, 0  
        jge     loop_second     ; if didnt reach end of first number start over  
        mov     al, [carry]     ; adding the carry to the result
        add     [r8], al
        add     r8, [number_of_digits]  ; getting to the right digit of the result
        mov     r12,[number_of_digits]  ; resetting the indexes of both numbers    
        mov     rsi, [number_of_digits] 
        add     rdi, rsi       ; go to the first digit of the first number
        dec     rcx             ; moving to the next digit of the second number
        cmp     rcx, 0         ; checking if second number is finished
        jl     second_iteration        
        dec     r11            
        
second_iteration:        
        add     [output_mul], r9    ; add 1 digit to the result   
        sub     r13, r9         ; decreasing 1 to the num of digits
        cmp     r13, 0          ; checking if reached the end of the number
        jge     loop_first      ; if not, keep looping
        jmp     end             ; finish the mult if reached the end

prep_mul:
        mov     r9, 1          ; using r9 to decrementing/incrementing by 1        
        add     rdi, rsi       ; getting to the lsb  
        mov     r11, rdx       ; storing rdx in r11 - rdx used for math operations     
        add     r11, rcx       ; getting to the lsb         
        mov     r12, rsi        ; storing rsi for later use           
        mov     r13, rcx        ; storing rcx for later use       
        add     [loop_index], rsi   ; resetting both indexes for the numbers
        add     [loop_index], rdx
        jmp     loop_first               

more_carry:
        sub     al, 10          ; extra carry is needed so decreasing 10 
        add     [carry], r9     ; carry is increased by 1
        mov     [r8], al        ; mov the lsb in the result register
        jmp     finish_mul

end:
        add     r8, 1           ; for the correct index
        mov     al, [r8]        ; making sure the correct number of digits is returned
        cmp     al, '0'
        jne     finish_operation
        sub     [output_mul], r9
        
finish_operation:
        mov     rax, [output_mul]   ; move result number of digits to the return value      
        mov     rsp, rbp
        pop     rbp
        ret