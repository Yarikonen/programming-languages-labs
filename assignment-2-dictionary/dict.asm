global find_word
extern string_equals
section .text

;- В файле `dict.asm` создать функцию `find_word`. Она принимает два аргумента:
;  - Указатель на нуль-терминированную строку.
;  - Указатель на начало словаря.
;  `find_word` пройдёт по всему словарю в поисках подходящего ключа. Если подходящее вхождение найдено, вернёт адрес *начала вхождения в   словарь* (не значения), иначе вернёт 0. 
find_word:
        .listIter:    
                push rdi
                push rsi
                add rsi, 8              ;get 2nd argument
                call string_equals
                pop rsi
                pop rdi
                cmp rax, 1
                je .found
                mov rsi, [rsi]          ;get next elem
                cmp rsi, 0              ;last elem pointer to the next = 0
                jne .listIter
        xor rax,rax
        jmp .end
        .found:
                mov rax, rsi 
        .end:
                ret
        
        

