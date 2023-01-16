%include "word.inc"
%include "lib.inc"
%include "dict.inc"
%define BUFFER_CAP 256
section .bss
buffer: resb BUFFER_CAP 
section .data
bufferError: db "Buffer Overflow!!", 0
noSuchElementError: db "There is not such element in your list!!", 0
section .text

global _start
_start:
        mov rdi, buffer
        mov rsi, BUFFER_CAP
        call read_word
        cmp rax, 0 
        je .bufferOverFlow
        mov rdi, buffer
        mov rsi, next
        push rsi
        call find_word
        pop rsi
        cmp rax, 0 
        je .noSuchElement
        add rax, 8
        mov rdi, rax
        push rdi
        call string_length
        pop rdi
        add rdi, rax
        inc rdi
        call print_string
        call print_newline
        jmp exit
        .bufferOverFlow:
                mov rdi, bufferError
                call print_error

        .noSuchElement:
                mov rdi, noSuchElementError
                call print_error

          
print_error:
        push rdi
        call string_length
        mov rdx, rax
        mov rax, 1
        pop rsi
        mov rdi, 2
        syscall 
        call print_newline
        jmp exit