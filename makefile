tp1 : tp
	gcc -g -m32 -o  salida aux/tp.o aux/lex.o \
	 aux/libreria.o aux/gramatica.o aux/automata.o
tp : clean
	mkdir aux
	flex --outfile=lex.gen.c parser.l
	gcc -g -m32 -o aux/lex.o -c lex.gen.c 
	gcc -g -m32 -o aux/gramatica.o -c gramatica/gramatica.c 
	gcc -g -m32 -o aux/automata.o -c automata/automata.c 
	gcc -g -m32 -o aux/tp.o -c tp1.c 
	gcc -g -m32 -o aux/libreria.o -c libreria.c
clean:
	rm -rf aux
	rm -f lex.gen.c
all : tp
	gcc -g -m32 -o salida aux/lex.o aux/tp.o aux/cstring.o \
	aux/map.o aux/list.o aux/includes.o aux/tree.o \
	aux/grammar.o aux/automatha.o