ASM = nasm
ASMFLAGS = -f elf64 
LD = ld
RM = rm
.PHONY: clean
main: main.o dict.o lib.o
	$(LD) -o $@ $^

main.o: main.asm colon.inc lib.inc
	$(ASM) $(ASMFLAGS)  $<
%.o :%.asm
	$(ASM) $(ASMFLAGS) $^
clean:
	$(RM) *.o main
