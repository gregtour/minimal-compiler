printf PROTO C :VARARG 

.data
string_fmt db "%s", 0
integer_fmt db "%i", 10, 0

message db "The fifth fibonacci number is ", 0

.code
main proc
start:
  ; prep stack
  push rdi
  push rbx
  ;sub rsp, 32
  sub rsp, 80h
  mov rdi, rsp
  
  ; result = fibonacci(3) + fibonacci(4);
  
  ; fibonacci(3)
  mov rcx, 3
  call fibonacci_i
  mov rbx, rax
  
  ; fibonacci(4)
  mov rcx, 4
  call fibonacci_i
  add rbx, rax
  
  ; result =
  push rbx
  
  ; prints("The fifth fibonacci number is ");
  lea rcx, [message]
  call lib_prints
  
  ; printi(result);
  ;mov rcx, 0
  ;mov rcx, rdi
  pop rcx
  call lib_printi
  
  ; clean stack
  ;add rsp, 32
  add rsp, 80h
  pop rbx
  pop rdi
  
  ; return 0
  ;mov rax, 0
  ret

lib_prints:
  sub rsp, 32
  mov rdx, rcx
  lea rcx, [string_fmt]
  call printf
  add rsp, 32
  ret

lib_printi:
  sub rsp, 32
  mov rdx, rcx
  lea rcx, [integer_fmt]
  call printf
  add rsp, 32
  ret
  
fibonacci_i:
 push rdi
 push rcx ; i
 ; a, b: int
 sub rsp, 16
 
 ; if (i > 1)
 mov rax, 1
 cmp [rsp+16], rax
 
 jle fib_i_else_block
 
 ; a = fibonacci(i-1)
 mov rcx, [rsp+16]
 sub rcx, 1
 sub rsp, 32
 call fibonacci_i
 add rsp, 32
 
 mov [rsp+8], rax
 
 ; b = fibonacci(i-2)
 mov rcx, [rsp+16]
 sub rcx, 2
 sub rsp, 32
 call fibonacci_i
 add rsp, 32
 
 mov [rsp], rax
 
 ; return a + b;
 mov rax, [rsp+8]
 add rax, [rsp]
 
 add rsp, 16
 pop rcx
 pop rdi
 ret
 
 fib_i_else_block:
 ; return 1;
 add rsp, 16
 pop rcx
 pop rdi
 mov rax, 1
 ret
  
fibonacci_i_static:
  push rdi
  push rcx ; i
  ; accum_a, accum_b, swap: int
  sub rsp, 24
  ; accum_a = 0
  mov rax, 0
  mov [rsp+16], rax
  ; accum_b = 1
  mov rax, 1
  mov [rsp+8], rax
  
  ; while (i > 0)
 fis_while_lbl:
   mov rax, 0
   ;cmp [rsp+24], rax
   add rax, [rsp+24]
   ;jle fis_while_end
   jz fis_while_end
   
   ; swap = accum_b
   mov rax, [rsp+8]
   mov [rsp], rax
   ; accum_b = accum_a + accum_b
   mov r10, [rsp+16]
   mov r11, [rsp+8]
   add r10, r11
   mov [rsp+8], r10
   ; accum_a = swap
   mov rax, [rsp]
   mov [rsp+16], rax
   ; i = i -1
   mov rax, [rsp+24]
   sub rax, 1
   mov [rsp+24], rax
   
   ; while loop
   jmp fis_while_lbl
   
 fis_while_end:
 
 ; return accum_b
 mov rax, [rsp+8]
 
 ; [accum_a, accum_b, swap : int]
 add rsp, 24
 pop rcx
 pop rdi
 ret
  
main endp
END
