native '.S', show_stack		
	mov rax, rsp
	.loop:
		cmp rax, [stack_head]
		jge next	
		mov rdi, [rax]
		push rax
		call print_int
		call print_newline
		pop rax
		add rax, 8
		jmp .loop


native "+", plus
  pop rax
  add [rsp], rax
  jmp next

native "-", minus
  pop rax
  sub [rsp], rax
  jmp next

native "*", multiply
  pop rax
  pop r8
  imul rax, r8
  push rax
  jmp next

