%define TAB 0x9
%define NEW_LINE 0xA
%define SPACE 0x20
global print_newline
global print_char
global exit
global string_length
global print_string
global print_int
global print_uint
global string_equals
global read_char
global read_word
global parse_int
global parse_uint
global string_copy


print_newline:
    mov rdi, NEW_LINE
print_char:
    mov rax,1
    push rdi
    mov rsi, rsp
    pop rdi
    mov rdi, 1 ; stdout file descriptor
    mov rdx, 1
    syscall
    ret

; Принимает код возврата и завершает текущий процесс
exit: 
    mov     rax, 60         
    syscall

; Принимает указатель на нуль-терминированную строку, возвращает её длину
string_length:
    xor rax, rax
    .loop:
        cmp byte [rdi+rax], 0
        je .end
        inc rax
    jmp .loop
    .end:
    ret

; Принимает указатель на нуль-терминированную строку, выводит её в stdout
print_string:
        push rdi
    call string_length
    mov rdx, rax
    mov rax, 1
        pop rsi
    mov rdi, 1
    syscall 
    ret
; Выводит знаковое 8-байтовое число в десятичном формате 
print_int:
    cmp rdi, 0
    jl .minus 
    jmp print_uint
    .minus:
        push rdi
        mov rdi, '-'
        call print_char
        pop rdi
        neg rdi 
        
print_uint:
    mov rax,rdi
    mov rdi,10
    mov r8, rsp
    push 0
    .getDigitsLoop:
        xor rdx, rdx
        div rdi
        add rdx, '0'
        dec rsp
        mov byte[rsp], dl
        cmp rax,0
        je .nextStep
        jmp .getDigitsLoop
    .nextStep:
        mov rdi, rsp
        push r8
        call print_string
        pop rsp
    ret
; Принимает два указателя на нуль-терминированные строки, возвращает 1 если они равны, 0 иначе
string_equals:
    xor r8, r8
    .byteLoop:
        mov al, byte[rsi+r8]
        cmp byte [rdi+r8], al
        jne .notEqual
        cmp rax, 0
        je .ending
        inc r8
        jmp .byteLoop
    .ending:
        mov rax, 0x1
    ret
    .notEqual:
        mov rax, 0x0  
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
    xor rcx, rcx
    push rdi
    .loopSpace:
            push rdi
            push rsi
            push rcx
        call read_char
            pop rcx
            pop rsi
            pop rdi
        cmp rax, 0
        je .end
        cmp rax, SPACE
        je .continue
        cmp rax, TAB  
        je .continue
        cmp rax, NEW_LINE
        je .continue
        mov byte [rdi], al
        inc rcx
        cmp rcx, rsi
        je .endOfBuffer
        inc rdi
    jmp .loopSpace
    .continue:
        inc rdi
        cmp rcx,0
        ja .end
    jmp .loopSpace
    .end:
        mov byte[rdi], 0
        mov rdx, rcx
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
    xor rdx,rdx
    xor rcx,rcx
    xor r8, r8
    mov rsi, 10
    .loop:
        xor rax, rax
        mov  al, byte [rdi+rcx]
        cmp rax, '0'
        jge .moreThanOne
        mov rax, r8
        div rsi
        mov rdx, rcx
   
    ret
    .moreThanOne:
        cmp rax, '9'
        jle .isDigit
        xor rax,rax
        mov rax, r8
        div rsi
        mov rdx, rcx
        ret
    .isDigit:
        sub rax, '0'
        add rax, r8
        mul rsi
        mov r8, rax
        inc rcx
    jmp .loop




; Принимает указатель на строку, пытается
; прочитать из её начала знаковое число.
; Если есть знак, пробелы между ним и числом не разрешены.
; Возвращает в rax: число, rdx : его длину в символах (включая знак, если он был) 
; rdx = 0 если число прочитать не удалось
parse_int:
    xor r8, r8
    xor rax, rax
    mov  al, byte [rdi]
    cmp rax, '-'
    jne .moreThanZero
    inc r8
    inc rdi
    .moreThanZero:
            push r8
        call parse_uint
            pop r8
        cmp rdx, 0
        je .end
        cmp r8, 1
        jne .end
        neg rax
        inc rdx
    .end:
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
