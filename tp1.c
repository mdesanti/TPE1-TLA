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
			if (pFile == NULL) {
				printf(
						"El archivo ingresado no pudo ser leido. Verifique que este realmente exista\n");
				exit(EXIT_FAILURE);
			}
			stdin = pFile;
			tipoDeArchivo = GRAMATICA;
			gramatica = nuevaGramatica();
		} else if (strstr(archivo, ".dot") != NULL) {
			FILE * pFile = fopen(archivo, "rx3");
			if (pFile == NULL) {
				printf(
						"El archivo ingresado no pudo ser leido. Verifique que este realmente exista\n");
				exit(EXIT_FAILURE);
			}
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
			if(!validarEstadoInicial(automata)) {
				printf("El automata no tiene estado inicial");
				exit(EXIT_FAILURE);
			}
			gram = crearGramatica(automata);
			imprimirAutomata(automata);
			imprimirGramatica(gram);
			crearArchivoGR(gram);
			break;
		case GRAMATICA:
			imprimirGramatica(gramatica);
			autom = convertiraAutomata(gramatica);
			imprimirAutomata(autom);
			crearArchivoDOT(autom);
			system("dot -Tpng automata.dot -o salida.png");
			system("rm automata.dot");
			break;
		}
		return EXIT_SUCCESS;
	} else {
		printf(
				"Los argumentos no son validos. Debe ingresar: ejecutable <archivo.gr/archivo.dot>\n");
		exit(EXIT_FAILURE);
	}
}
