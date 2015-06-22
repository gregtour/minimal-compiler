CC = gcc

all:
	$(CC) autostubs.c grammar.c lexer.c main.c parser.c \
			parser_t.c preprocess.c tables.c typechecker.c \
			-o compiler.bin
	$(CC) assembler.c -o linker.bin

clean:
	rm compiler.bin
	rm linker.bin
