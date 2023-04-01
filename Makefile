all: jogo

jogo: mines.o pilha.o
	gcc -o jogo mines.o pilha.o

mines.o: mines.c mines.h
	gcc -o mines.o -c mines.c

pilha.o: pilha.c pilha.h
	gcc -o pilha.o -c pilha.c

clean:
	@rm -f *~ *.bak

purge:   clean
	@rm -f *.o core a.out
	@rm -f jogo
