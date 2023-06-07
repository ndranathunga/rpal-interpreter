CC = g++
CFLAGS = -c

rpal20: main.o
	$(CC) main.o -o rpal20
	del /Q *.o

main.o: main.cpp Tree.h TreeNode.h Lexer.h Parser.h Token.h TokenStorage.h
	$(CC) $(CFLAGS) main.cpp

clean:
	del /Q *.o rpal20.exe
