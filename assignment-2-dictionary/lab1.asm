section .data
arg1: db 'ashdb asdhabs dahb', 0
arg2: times 19 db  66

section .text

global _start
_start:
    
mov rdi, -1
mov rsi, -1
mov rax, -1
mov rcx, -1
mov rdx, -1
mov r8, -1
mov r9, -1
mov r10, -1
mov r11, -1
push rbx
push rbp
push r12 
push r13 
push r14 
push r15 

    mov rdi, arg1
    mov rsi, arg2
    mov rdx, 19
    call string_copy
    call exit


print_char:
    push rcx
    push rax
    mov rax,1
    push rdi
    mov rsi, rsp
    mov rdi, 1 ; stdout file descriptor
    mov rdx, 1
    syscall
    pop rdi
    pop rax
    pop rcx
    ret

; Принимает код возврата и завершает текущий процесс
exit: 
    mov     rax, 60         
    xor     rdi, rdi
    syscall

; Принимает указатель на нуль-терминированную строку, возвращает её длину
string_length:
    push r13
    xor r13, r13
    .loop:
    cmp byte [rdi+r13], 0
    je .end
    inc r13
    jmp .loop
    .end:
    mov rax, r13
    pop r13
    ret

; Принимает указатель на нуль-терминированную строку, выводит её в stdout
print_string:
    call string_length
    mov rdx, rax
    mov rax, 1
    mov rsi, rdi
    mov rdi, 1
    syscall 
    ret

; Принимает код символа и выводит его в stdout

; Переводит строку (выводит символ с кодом 0xA)
print_newline:
    mov rdi, 0xA
    call print_char
    ret
    

; Выводит беззнаковое 8-байтовое число в десятичном формате 
; Совет: выделите место в стеке и храните там результаты деления
; Не забудьте перевести цифры в их ASCII коды.
print_uint:
    xor rcx, rcx
    mov rax,rdi
    mov rdi,0xA
    .getDigitsLoop:
        xor rdx, rdx
        div rdi
        inc rcx
        push rdx 
        cmp rax,0
        je .nextStep
        jmp .getDigitsLoop
    .nextStep:
    .addLoop:
        dec rcx
        pop rdi
        add rdi, 0x30
        call print_char
        cmp rcx, 0
        je .ending
        jmp .addLoop
    .ending:
    ret

; Выводит знаковое 8-байтовое число в десятичном формате 
print_int:
    cmp rdi, 0
    jnl .minus 
    call print_uint
    ret
    .minus:
    push rdi
    mov rdi, 45
    call print_char
    pop rdi
    neg rdi 
    call print_uint
    xor rax, rax
    ret

; Принимает два указателя на нуль-терминированные строки, возвращает 1 если они равны, 0 иначе
string_equals:
    call string_length
    mov rcx,rax
    push rdi
    mov rdi, rsi
    call string_length
    pop rdi
    cmp rcx, rax
    push r13
    jne .notEqual
    xor r13,r13
    .byteLoop:
    mov al, byte[rsi+r13]
    cmp byte [rdi+r13], al
    jne .notEqual
    inc r13
    cmp rcx,r13
    jle .ending
    jmp .byteLoop
    .ending:
    mov rax, 0x1
    pop r13
    ret
    .notEqual:
    mov rax, 0x0
    pop r13
    ret

; Читает один символ из stdin и возвращает его. Возвращает 0 если достигнут конец потока
read_char:
    xor rax, rax
    mov rdx,1
    mov rdi, 0
    push rdi
    mov rsi,rsp
    syscall 
    pop rax
    ret 

; Принимает: адрес начала буфера, размер буфера
; Читает в буфер слово из stdin, пропуская пробельные символы в начале, .
; Пробельные символы это пробел 0x20, табуляция 0x9 и перевод строки 0xA.
; Останавливается и возвращает 0 если слово слишком большое для буфера
; При успехе возвращает адрес буфера в rax, длину слова в rdx.
; При неудаче возвращает 0 в rax
; Эта функция должна дописывать к слову нуль-терминатор

read_word:
    xor rax, rax
    xor rcx, rcx
    mov r9, rdi
    mov r8, rsi
    push rdi
    .loopSpace:
    call read_char
    cmp rax, 0
    je .nullString
    cmp rax, 0x20
    jne .notWhite_space
    je .continue
    .notWhite_space:
    cmp rax, 0x9
    jne .notTab_space
    je .continue
    .notTab_space:
    cmp rax, 0xA
    jne .normalWord
    .continue:
    inc r9
    jmp .loopSpace
    
    .normalWord:
    xor rcx, rcx
    inc rcx
    mov [r9], rax
    inc r9
    .loop:
        push rcx
        call read_char
        pop rcx
        mov [r9], rax
        cmp rax, 0
        je .end
        cmp rax, 0x20
        je .end
        cmp rax, 0x9
        je .end
        cmp rax, 0xA
        je .end
        inc rcx
        inc r9
        cmp rcx, r8
        je .endOfBuffer
        jmp .loop
    .end:
        inc r9
        mov byte[r9], 0
        mov rdx, rcx
        pop rax
        ret
    .nullString:
    xor rdx, rdx
    pop rax
    ret
    .endOfBuffer:
    pop rax
    xor rax, rax
    ret
 
 

; Принимает указатель на строку, пытается
; прочитать из её начала беззнаковое число.
; Возвращает в rax: число, rdx : его длину в символах
; rdx = 0 если число прочитать не удалось
parse_uint:
    push r13
    push r12
    xor r13, r13
    xor r12, r12
    xor rdx,rdx
    mov rsi, 0xa
    .loop:
    xor rax, rax
    mov  al, byte [rdi+r13]
    cmp rax, 0x30
    jge .moreThenOne
    xor rax,rax
    mov rax, r12
    div rsi
    mov rdx, r13
    pop r12
    pop r13 
    ret
    .moreThenOne:
    cmp rax, 0x39
    jle .isDigit
    xor rax,rax
    mov rax, r12
    div rsi
    mov rdx, r13
    pop r12
    pop r13
    ret
    .isDigit:
    sub rax, 0x30
    add rax, r12
    mul rsi
    mov r12, rax
    inc r13
    jmp .loop

    ret




; Принимает указатель на строку, пытается
; прочитать из её начала знаковое число.
; Если есть знак, пробелы между ним и числом не разрешены.
; Возвращает в rax: число, rdx : его длину в символах (включая знак, если он был) 
; rdx = 0 если число прочитать не удалось
parse_int:
    push r13
    xor r13, r13
    xor rax, rax
   
    mov  al, byte [rdi]
    cmp rax, 0x2D
    jne .moreThenZero
    inc r13
    inc rdi
    .moreThenZero:
    call parse_uint
    cmp rdx, 0
    je .end
    cmp r13, 1
    jne .end
    neg rax
    inc rdx
    .end:
    pop r13
    ret 

; Принимает указатель на строку, указатель на буфер и длину буфера
; Копирует строку в буфер
; Возвращает длину строки если она умещается в буфер, иначе 0
string_copy: 
    xor rcx, rcx
    xor rax, rax
    .loop:
    cmp rcx, rdx
    jge .endOfBuffer
    mov al, [rdi+rcx]
    mov  byte [rsi+rcx],  al
    inc rcx
    cmp byte[rdi+rcx],0
    je .end
    jmp .loop
    .endOfBuffer:
    xor rax, rax
    ret
    .end:
    mov byte [rsi+rcx],0
    mov rax, rcx
    ret
