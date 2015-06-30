.global main

.data
string_fmt:
  .ascii "%s\0"
integer_fmt: 
  .ascii "%i.\n\0"
message: 
  .ascii "The fifth number in the Fibonacci sequence is \0"

.text
main:
  pushl %edi
  pushl %ebx

  subl $0x20, %esp
  movl %esp, %edi
  
  movl $0x3, %ecx
  call fibonacci_i
  movl %eax, %ebx
  
  movl $0x4, %ecx
  call fibonacci_i
  addl %eax, %ebx
  
  push %ebx
  
  leal message, %ecx
  call lib_prints
  
  popl %ecx
  call lib_printi
  
  addl $0x20, %esp
  popl %ebx
  popl %edi
  
  movl $0x0, %eax
  ret

lib_prints:
  subl $0x10, %esp
  movl %ecx, %edx
  leal string_fmt, %ecx
  pushl %edx
  pushl %ecx
  call printf
  popl %edx
  popl %edx
  
  addl $0x10, %esp
  ret

lib_printi:
  subl $0x10, %esp
  movl %ecx, %edx
  leal integer_fmt, %ecx
  pushl %edx
  pushl %ecx
  call printf
  popl %edx
  popl %edx
  addl $0x10, %esp
  ret
  
fibonacci_i:
 pushl %edi
 pushl %ecx

 subl $0x10, %esp
 
 movl $0x1, %eax
 cmp %eax, 0x10(%esp)
 
 jle fib_i_else_block
 
 movl 0x10(%esp), %ecx
 subl $0x1, %ecx
 subl $0x20, %esp
 call fibonacci_i
 addl $0x20, %esp
 
 movl %eax, 0x8(%esp)
 
 movl 0x10(%esp), %ecx
 subl $0x2, %ecx
 subl $0x20, %esp
 call fibonacci_i
 addl $0x20, %esp
 
 movl %eax, (%esp)
 
 movl 0x8(%esp), %eax
 addl (%esp), %eax
 
 addl $0x10, %esp
 popl %ecx
 popl %edi
 ret
 
 fib_i_else_block:
 addl $0x10, %esp
 popl %ecx
 popl %edi
 movl $0x1, %eax
 ret
  
fibonacci_i_static:
  pushl %edi
  pushl %ecx 
  subl $0x18, %esp
  
  movl $0x0, %eax
  movl %eax, 0x10(%esp)
  
  movl $0x1, %eax
  movl %eax, 0x8(%esp)
  
 fis_while_lbl:
   movl $0x0, %eax

   addl 0x18(%esp), %eax
   jz fis_while_end
   
   movl 0x8(%esp), %eax
   movl %eax, (%esp)
   
   movl 0x10(%esp), %ecx
   movl 0x8(%esp), %edx
   addl %edx, %ecx
   movl %ecx, 0x8(%esp)

   movl (%esp), %eax
   movl %eax, 0x10(%esp)

   movl 0x18(%esp), %eax
   subl $0x1, %eax
   movl %eax, 0x18(%esp)
   
   jmp fis_while_lbl
 fis_while_end:
 
 movl 0x8(%esp), %eax
 
 addl $0x18, %esp
 popl %ecx
 popl %edi
 ret
 
