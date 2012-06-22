#include "gramatica/gramatica2.h"
#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

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
		} else {
			printf(
					"El archivo ingresado no corresponde con un formato esperado. Debe ingresar o un .dot o un .gr\n");
			exit(EXIT_FAILURE);
		}

		yylex();
		switch (tipoDeArchivo) {
		case GRAMATICA:
			crearAnalizador(gramatica);
			system("gcc -g -o ASDR analizador.c");
			break;
		}
		return EXIT_SUCCESS;
	} else {
		printf(
				"Los argumentos no son validos. Debe ingresar: ejecutable <archivo.gr/archivo.dot>\n");
		exit(EXIT_FAILURE);
	}
}
