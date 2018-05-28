global _start
global last_word

%include "util.inc"
%include "macro.inc"

%define w r15
%define pc r14
%define rstack r13

%define word_size 8
%define is_immediate 1
%define is_branch 2


section .data
program_stub: dq 0
xt_interpreter: dq .interpreter
.interpreter: dq interpreter_loop
current_word: times 256 db 0, 0
stack_head: dq 0
p_cmp_flag: dq 0
state: dq 0
here: dq user_dict
last_word: dq _lw

section .rodata
undefined:          db 'No such word',10, 0

section .bss

user_memory: resq 65536
user_dict: resq 65536
return_stack: resq 1024


section .text
%include "words.inc"

section .text
next:
  mov w, pc
  add pc, word_size
  mov w, [w]
  jmp [w]

interpreter_loop:
  mov rsi, 256
  mov rdi, current_word
  call read_word

  test rdx, rdx
  jz .exit

  mov rdi, rax
  call find_word

  cmp byte [state], 0
  jne .compile

  test rax, rax
  jz .not_word

  mov rdi, rax
  call cfa

  .interpret_word:
  mov [program_stub], rax
  mov pc, program_stub

  jmp next

  .not_word:
  mov rdi, current_word
  call parse_int

  test rdx, rdx
  jz .not_found

  push rax

  jmp interpreter_loop

  .not_found:
  mov rdi, undefined
  call print_string

  jmp interpreter_loop


 .compile:
  test rax, rax
  jz .not_word_compile
  mov rdi, rax
  call cfa
  lea rdx, [rax - 1]
  movzx rdx, byte [rdx]
  mov [p_cmp_flag], rdx
  cmp rdx, is_immediate
  je .interpret_word
  mov rdx, [here]
	mov [rdx], rax
  add qword [here], word_size
  jmp interpreter_loop

  .not_word_compile:
  mov rdi, current_word
  call parse_int
  test rdx, rdx
  jz .not_found
  cmp byte[p_cmp_flag], is_branch
  je .branch

  mov rcx, xt_lit
  mov rdx, [here]
  mov qword[rdx], rcx
  add qword[here], word_size
  mov rdx, [here]
  mov [rdx], rax
  add qword[here], word_size
  jmp interpreter_loop

  .branch:
  mov rdx, [here]
  mov [rdx], rax
  add qword [here], word_size
  jmp interpreter_loop

  .exit:
  xor rdi, rdi
  call exit

_start:
  mov rstack, return_stack
  mov [stack_head], rsp
  mov pc, xt_interpreter
  jmp next

