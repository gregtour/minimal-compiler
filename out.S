printf PROTO C :VARARG 
;extern printf:PROC

.data
fmt db " - %s", 0
msg db "Hello world", 0

.code
main proc
start:
  push        rdi 
  sub         rsp, 80h 
  mov         rdi, rsp

  lea rcx, [fmt]
  lea rdx, [msg]
  call printf
    
  add         rsp, 80h 
  pop         rdi 
  ret
main endp
END
