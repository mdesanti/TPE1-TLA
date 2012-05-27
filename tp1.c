#include "automata/automata.h"
#include "gramatica/gramatica.h"
#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

extern automata;
extern gramatica;
extern tipoDeArchivo;

int analizarEntrada(int argc, char ** argv) {
	gramatica_t gram;
	automata_t autom;
	if (argc == 2) {
		char * archivo = argv[1];
		tipoDeArchivo = 0;
		if (strstr(archivo, ".gr") != NULL) {
			FILE * pFile = fopen(archivo, "r");
			stdin = pFile;
			tipoDeArchivo = GRAMATICA;
			gramatica = nuevaGramatica();
		} else if (strstr(archivo, ".dot") != NULL) {
			FILE * pFile = fopen(archivo, "rx3");
			stdin = pFile;
			tipoDeArchivo = AUTOMATA;
			automata = nuevoAutomata();
		} else {
			printf(
					"El archivo ingresado no corresponde con un formato esperado. Debe ingresar o un .dot o un .gr\n");
			exit(EXIT_FAILURE);
		}

		yylex();
		switch (tipoDeArchivo) {
		case AUTOMATA:
			gram = crearGramatica(automata);
			imprimirAutomata(automata);
			imprimirGramatica(gram);
			break;
		case GRAMATICA:
			autom = convertiraAutomata(gramatica);
			imprimirAutomata(automata);
			break;
		}
		return EXIT_SUCCESS;
	} else {
		printf(
				"Los argumentos no son validos. Debe ingresar: ejecutable <archivo.gr/archivo.dot>\n");
		exit(EXIT_FAILURE);
	}
}
