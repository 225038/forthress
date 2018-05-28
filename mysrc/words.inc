; Stack manipulation block
;Show full stack
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

; Show top of stack
native '.', dot			
	cmp rsp, [stack_head]
	jge error_underflow
	pop rdi
	call print_int
	jmp next



; (ab - ba)
native "swap", swap
  pop rax
  pop rcx
  push rax
  push rcx
  jmp next

; (a - aa)
native "dup", dup
	pop rax 
	push rax 
	push rax 
	jmp next 

; drop top of stack
native 'drop', drop		
	cmp rsp, [stack_head]
	jge error_underflow
	pop rax
	jmp next



; rstak manipulations block
native "r>", push_r
  pop rax
  rpush rax
  jmp next

native ">r", pop_rs
  rpop rax
  push rax
  jmp next

native "r@", copy_rs
    mov rax, [rstack]
    push rax
    jmp next

; arifmetics block

native "+", plus
  pop rax
  add [rsp], rax
  jmp next

native "-", min
  pop rax
  sub [rsp], rax
  jmp next

native "*", mult
  pop rax
  pop rdx
  imul rax, r8
  push rax
  jmp next

native "/", div
  pop rax
  pop r8
  mov rdx, 0
  idiv r8
  push rax
  jmp next

native "%", div_
  pop rax
  pop r8
  mov rdx, 0
  idiv r8
  push rdx
  jmp next

; logic block
native "=", equals 
	pop rax
	pop rcx
	cmp rax, rcx
	sete al
	movzx rax, al
	push rax
	jmp next

native "<", less 
	pop rax
	pop rcx
	cmp rcx, rax
	setl al
	movzx rax, al
	push rax
	jmp next

native "and", and
    pop rax
    and [rsp], rax
    jmp next

native "or", or
    pop rax
    or [rsp], rax
    jmp next

native "land", land
    pop rax
    pop rdx
    test rax, rax
    jz .no
    push rdx
    jmp next
.no:
    push rax
    jmp next

native "lor", lor
    pop rax
    pop rdx
    test rax, rax
    jnz .yes
    push rdx
    jmp next
.yes:
    push rax
    jmp next


; utils block
native "find", find
  pop rdi
  call find_word
  push rax
  jmp next
native "cfa", cfa_
  pop rdi
  call cfa
  push rax
  jmp next
native "syscall", syscall
  pop r9
  pop r8
  pop r10
  pop rdx
  pop rsi
  pop rdi
  pop rax
  syscall
  push rax
  push rdx
  jmp next

error_underflow:
	mov rdi, underflow
	call print_string
	jmp next



; colon words block

native "docol", docol
  rpush pc
  add w, 8
  mov pc, w
  jmp next

native "exit", exit
  rpop pc
  jmp next

native "lit", lit
  push qword [pc]
  add pc, 8
  jmp next

native "branch", branch, is_branch
  mov pc, [pc]
  jmp next

native "0branch", branch0, is_branch
  pop rax
  test rax, rax
  jz i_branch
  add pc, 8
  jmp next

native ",", comma
  mov rax, [here]
  pop qword [rax]
  add qword [here], 8
  jmp next

native "create", create
  ; link
  mov rdx, [last_word]
  mov rsi, [here]
  mov [rsi], rdx
  mov [last_word], rsi
  add rsi, 8
  mov byte [rsi], 0

  ; name
  pop rdi
  push rsi
  call string_copy
  mov rdi, qword [rsp]
  call string_length
  pop rsi
  add rsi, rax

  ; flags
  inc rsi
  pop rax
  mov [rsi], al
  inc rsi
  mov [here], rsi
  jmp next

colon ":", colon
  .repeat:
  dq xt_inbuf, xt_word
  branch0 .repeat
  dq xt_lit, 0, xt_inbuf, xt_create
  dq xt_lit, 1, xt_state, xt_put_int
  dq xt_lit, i_docol, xt_comma
  dq xt_exit

colon ";", semicolon, is_immediate
  dq xt_lit, 0, xt_state, xt_put_int
  dq xt_lit, xt_exit, xt_comma
  dq xt_exit

; constants
const inbuf, current_word
const mem, user_memory
const state, state
const here, [here]
const last_word, last_word



