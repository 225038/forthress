section .data
    stack_head: dq 0

; stack manipulations block
native "drop", drop
    add rsp, 8
    jmp next

native "swap", swap
    pop rax
    pop rdx
    push rax
    push rdx
    jmp next

native "dup", dup
    pop rax
    push rax 
    push rax
    jmp next
    
native ".", dot
    pop rdi
    call print_int
    jmp next
    
native '.S', show_stack ; Show full stack
	mov rax, rsp
	.loop:
		cmp rax, [stack_head]
		jge next ;
		mov rdi, [rax]
		push rax
		call print_int
		call print_newline
		pop rax
		add rax, 8
		jmp .loop


