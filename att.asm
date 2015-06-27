# gcc style assembly
.global _start

.data
message:
    .ascii "Hello world\0"
format:
    .ascii " - %s - \n\0"

.text
_start:
    pushq %rdi
    subq  0x80, %rsp
    movq  %rsp, %rdi

    leaq format, %rcx
    leaq message, %rdx
    call printf

    addq 0x80, %rsp
    popq %rdi
    ret
# END
