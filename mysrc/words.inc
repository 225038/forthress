

const inbuf, current_word
const mem, user_memory
const last_word, last_word
const state, state
const here, [here]



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

native "/", divide
  pop rax
  pop r8
  mov rdx, 0
  idiv r8
  push rax
  jmp next


; COLON

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

