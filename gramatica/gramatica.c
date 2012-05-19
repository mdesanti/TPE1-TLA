/*
 * gramatica.c
 *
 */
#include "gramatica.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct produccion {
	noTerminal_t parteIzquierda;
	terminal_t terminal;
	noTerminal_t noTerminal;
};

struct gramatica {
	noTerminal_t *noTerminales;
	terminal_t *terminales;
	noTerminal_t simInicial;
	produccion_t *producciones;
	tipoG tipo;
};

static void Error(const char* s) {
	fprintf(stderr, s);
	exit(EXIT_FAILURE);
}

gramatica_t nuevaGramatica(void) {
	gramatica_t g;
	if ((g = malloc(sizeof(struct gramatica))) == NULL) {
		Error("No hay lugar para otro nodo\n");
	}
	g->noTerminales = NULL;
	g->terminales = NULL;
	g->simInicial = NULL;
	g->producciones = NULL;
	g->tipo = NULL;
	return g;
}

void agregarNoTerminal(gramatica_t gramatica, noTerminal_t elemento) {
	if (!isupper(elemento)) {
		Error("los elementos no terminales tienen que estar en mayusculas");
	}
	noTerminal_t *noTer = gramatica->noTerminales;
	char aux[] = { elemento, '\0' };
	if (noTer == NULL) {
		noTer = malloc(sizeof(noTerminal_t) * 2);
		noTer[0] = elemento;
		noTer[1] = '\0';
	} else {
		if ((noTer = realloc(noTer, (strlen(noTer) + 1) * sizeof(noTerminal_t)))
				== NULL) {
			Error("No hay lugar para otro nodo\n");
		}
		noTer = strcat(noTer, aux);
	}

	gramatica->noTerminales = noTer;
	printf("%s\n", gramatica->noTerminales);
}

void agregarTerminal(gramatica_t gramatica, terminal_t elemento) {
	if (!islower(elemento)) {
		Error("los elementos terminales tienen que estar en minusculas");
	}
	terminal_t *ter = gramatica->terminales;
	char aux[] = { elemento, '\0' };
	if (ter == NULL) {
		ter = malloc(sizeof(terminal_t) * 2);
		ter[0] = elemento;
		ter[1] = '\0';
	} else {
		if ((ter = realloc(ter, (strlen(ter) + 1) * sizeof(terminal_t)))
				== NULL) {
			Error("No hay lugar para otro nodo\n");
		}
		ter = strcat(ter, aux);
	}

	gramatica->terminales = ter;
	printf("%s\n", gramatica->terminales);
}

void agregarProduccion(gramatica_t gramatica, noTerminal_t pIz, terminal_t t,
		noTerminal_t nT) {
	if (!isNoTerminal(gramatica, pIz) || !isTerminal(gramatica, t)
			|| !isNoTerminal(gramatica, nT)) {
		Error(
				"Un elemento de una produccion no pertenece a los terminales o no terminales\n");
	}
	produccion_t *prod = gramatica->producciones;
	produccion_t aux;
	if ((aux = malloc(sizeof(struct produccion))) == NULL) {
		Error("No hay lugar para otro nodo\n");
	}
	aux->parteIzquierda = pIz;
	aux->terminal = t;
	aux->noTerminal = nT;
	//char aux[] = {elemento, '\0'};
	int cant = 0;
	int i;
	if (prod == NULL) {
		prod = malloc(sizeof(struct produccion) * 2);
		prod[0] = aux;
		prod[1] = NULL;
	} else {
		for (i = 0; prod[i] != NULL; i++) {
			cant++;
		}
		if ((prod = realloc(prod, (cant + 2) * sizeof(produccion_t))) == NULL) {
			Error("No hay lugar para otro nodo\n");
		}
		prod[cant] = aux;
		prod[cant + 1] = NULL;
	}

	gramatica->producciones = prod;
	//printf("%c -> %c%c\n", prod[cant]->parteIzquierda, prod[cant]->terminal, prod[cant]->noTerminal);
	printf("%c -> %c%c\n", gramatica->producciones[cant]->parteIzquierda,
			gramatica->producciones[cant]->terminal,
			gramatica->producciones[cant]->noTerminal);
}

void setearSimboloIncial(gramatica_t gramatica, noTerminal_t elemento) {
	gramatica->simInicial = elemento;
}

void setearTipoG(gramatica_t gramatica, tipoG tipo2) {
	gramatica->tipo = tipo2;
}

gramatica_t normalizar(gramatica_t gramatica) {
	return NULL;
}

gramatica_t pasarAFormaNDerecha(gramatica_t gramatica) {
	return NULL;
}

automata_t convertiraAutomata(gramatica_t gramatica) {
	return NULL;
}

int isNoTerminal(gramatica_t g, char c) {
	int aux;
	if ((aux = strchr(g->noTerminales, c)) == NULL) {
		return 0;
	}
	return 1;
}

int isTerminal(gramatica_t g, char c) {
	int aux;
	if ((aux = strchr(g->terminales, c)) == NULL) {
		return 0;
	}
	return 1;
}

void imprimirGramatica(gramatica_t g) {
	int i, j, cant;
	cant = 0;
	noTerminal_t auxNT;
	printf("G=({");
	for (i = 0; i < strlen(g->noTerminales) - 1; i++) {
		printf("%c,", g->noTerminales[i]);
	}
	printf("%c},{", g->noTerminales[i]);
	for (i = 0; i < strlen(g->terminales) - 1; i++) {
		printf("%c,", g->terminales[i]);
	}
	printf("%c},", g->terminales[i]);
	printf("%c,{", g->simInicial);
	for (i = 0; g->producciones[i] != NULL; i++) {
		cant++;
	}
	for (i = 0; i < strlen(g->noTerminales); i++) {
		auxNT = g->noTerminales[i];
		printf("%c -> ", auxNT);
		for (j = 0; j < cant; j++) {
			if (g->producciones[j]->parteIzquierda == auxNT) {
				if (g->producciones[j]->terminal == NULL
						&& g->producciones[j]->noTerminal == NULL) {
					printf("\\");
				} else {
					if (g->tipo == 1) {
						printf("%c%c | ", g->producciones[j]->terminal,
								g->producciones[j]->noTerminal);
					} else if (g->tipo == 2) {
						printf("%c%c | ", g->producciones[j]->noTerminal,
								g->producciones[j]->terminal);
					}
				}
			}
		}
		printf(", ");
	}
	printf("})\n");

}
