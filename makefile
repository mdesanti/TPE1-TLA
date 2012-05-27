tp1 : tp
	gcc -g -m32 -w -o  salida aux/tp.o aux/lex.o \
	 aux/libreria.o aux/gramatica.o aux/automata.o -lm
tp : clean
	mkdir aux
	flex --outfile=lex.gen.c parser.l
	gcc -g -m32 -w -o aux/lex.o -c lex.gen.c 
	gcc -g -m32 -w -o aux/gramatica.o -c gramatica/gramatica.c 
	gcc -g -m32 -w -o aux/automata.o -c automata/automata.c 
	gcc -g -m32 -w -o aux/tp.o -c tp1.c 
	gcc -g -m32 -w -o aux/libreria.o -c libreria.c -lm
clean:
	rm -rf aux
	rm -f lex.gen.c
all : tp
	gcc -g -m32 -w -o salida aux/lex.o aux/tp.o aux/cstring.o \
	aux/map.o aux/list.o aux/includes.o aux/tree.o \
	aux/grammar.o aux/automatha.o