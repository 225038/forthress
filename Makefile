
ASM			= nasm
ASMFLAGS	= -felf64 -g -Imysrc/ 


NATIVE_CALLS_SUPPORT=1

ifdef NATIVE_CALLS_SUPPORT
LINKERFLAGS = -nostdlib 
LIBS        = -ldl 
LINKER 		= gcc 
ASMFLAGS	+= -DNATIVE_CALLS
else
LINKER 		= ld
LINKERFLAGS = 
LIBS        = 
endif


all: bin/forthress
	
bin/forthress: obj/forthress.o obj/util.o
	mkdir -p bin 
	$(LINKER) -o bin/forthress  $(LINKERFLAGS) -o bin/forthress obj/forthress.o obj/util.o $(LIBS)

obj/forthress.o: mysrc/forthress.asm mysrc/macro.inc mysrc/words.inc mysrc/util.inc
	mkdir -p obj
	$(ASM) $(ASMFLAGS) mysrc/forthress.asm -o obj/forthress.o

obj/util.o: mysrc/util.inc mysrc/util.asm
	mkdir -p obj
	$(ASM) $(ASMFLAGS) mysrc/util.asm -o obj/util.o

clean: 
	rm -rf build obj

.PHONY: clean

