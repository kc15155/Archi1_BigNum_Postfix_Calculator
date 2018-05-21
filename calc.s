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
        global divs
        extern printf            	
    prep_add:
        mov r9, 1
        mov r10, 0
        mov [carry], r10
        add rdi, rsi
        add rdx, rcx
        mov [index], rsi
        add [index], r9
        add [index], r9
        jmp actual_adding
    prep_sub:
        mov r9, 1
        mov r10, 0
        mov [borrow], r10
        add rdi, rsi
        add rdx, rcx
        mov [index], rsi
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
        mov qword [first], 0
        mov qword [second], 0
        mov ah, [rdi]
        mov [first], ah
        mov ah, [rdx]
        mov [second], ah
        mov al, [first]
        sub al, '0'
        add al, [second]
        sub al, '0'
        add al, [carry]
        jmp check_carry
    actual_subtracting:
        mov qword [first], 0
        mov qword [second], 0
        mov ah, [rdi]
        cmp [borrow], r9
        jl no_decrease
    decrease:
        sub ah, 1
    no_decrease:
        mov [first], ah
        mov ah, [rdx]
        mov [second], ah
        mov al, [first]
        sub al, '0'
        sub al, [second]
        add al, '0'
        jmp check_borrow
    finish_adding:
        add al, '0'
        mov [r8], al
        add [output], r9
        jmp iteration_add
    finish_subtracting:
        add al, '0'
        mov [r8], al
        add [output], r9
        jmp iteration_sub
    iteration_add:
        dec rsi
        dec rdi
        dec rdx
        dec rcx
        sub r8, 1
        sub [index], r9
        cmp [index], r10
        jg actual_adding
        jmp finish
    iteration_sub:
        dec rsi
        dec rdi
        dec rdx
        dec rcx
        sub r8, 1
        sub [index], r9
        cmp [index], r10
        jg actual_subtracting
        jmp finish
    check_carry:
        cmp al, 10
        jge needs_carry
        mov [carry], r10
        jmp finish_adding
    check_borrow:
        cmp al, 0
        jl needs_borrow
        mov [borrow], r10
        jmp finish_subtracting
    needs_carry:
        sub al, 10
        mov [carry], r9
        jmp finish_adding
    needs_borrow:
        add al, 10
        mov [borrow], r9
        jmp finish_subtracting
    finish:
        mov rax, [output]
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
        cmp     rcx, 0             
        jl      finish_loop_first         
        mov     al, [r11]           
        mov     [second], al 
        
finish_loop_first:
        mov     qword [carry], 0
        
loop_second:
        mov qword [first], '1'        
        cmp     rsi, 0             
        jl      finish_loop_second       
        mov     ah, [rdi]          
        mov     [first], ah
        
finish_loop_second:
        mov     rax, 0
        mov     rax, [first]           
        sub     rax, '0'          
        mov     rdx, 0              
        mov     rdx, [second]          
        sub     rdx, '0'            
        mul     rdx
        add     rax, [carry]       
        mov     rdx, 0             
        mov     rbx, 10
        div     rbx
        mov     [carry], rax 
        mov     [temp], rdx
        mov     al, [temp]
        add     [r8], al    
        mov     al, [r8]
        cmp     al, '9'
        jg      more_carry
          
finish_mul:
        dec     rsi               
        cmp     rsi, 0             
        jl     first_iteration        
        dec     rdi  
        
first_iteration:
        sub     r8, 1              
        sub     r12, r10       
        cmp     r12, 0
        jge     loop_second       
        mov     al, [carry]
        add     [r8], al
        add     r8, [number_of_digits]
        mov     r12,[number_of_digits]      
        mov     rsi, [number_of_digits]
        add     rdi, rsi            
        dec     rcx                 
        cmp     rcx, 0              
        jl     second_iteration        
        dec     r11            
        
second_iteration:        
        add     [output_mul], r10    
        sub     r13, r10        
        cmp     r13, 0
        jge     loop_first
        jmp     end                 

prep_mul:
        mov     r10, 1             
        add     rdi, rsi           
        mov     r11, rdx            
        add     r11, rcx           
        mov     r12, rsi            
        mov     r13, rcx            
        add     [loop_index], rsi
        add     [loop_index], rdx
        jmp     loop_first               

more_carry:
        sub     al, 10
        add     [carry], r10
        mov     [r8], al
        jmp     finish_mul

end:
        add     r8, 1
        mov     al, [r8]
        cmp     al, '0'
        jne     finish_operation
        sub     [output_mul], r10
        
finish_operation:
        mov     rax, [output_mul]        
        mov     rsp, rbp
        pop     rbp
        ret