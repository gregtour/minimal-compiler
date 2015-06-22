CC = gcc

all:
	$(CC) autostubs.c grammar.c intermediate.c lexer.c \
			linker.c main.c parser.c parser_t.c platform.c \
			preprocess.c tables.c typechecker.c -o compiler.bin
	$(CC) assembler.c -o linker.bin

clean:
	rm compiler.bin
	rm linker.bin
