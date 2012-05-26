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

int esProduccionValida(gramatica_t gramatica, char pIz, char elem1, char elem2);

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
	g->producciones = NULL;
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

void agregarProduccionDesdeCadena(gramatica_t gramatica, char * cadena) {
	char pIz = cadena[0];
	char term = 0, noTerm = 0;
	int termino = 0, indice = 3;

	if (pIz == '/') {
		Error("Lambda no puede ser la parte izquierda de una produccion\n");
	}
	if (cadena[1] != '-') {
		Error(
				"Debe haber un solo simbolo en la parte izquierda de una produccion\n");
	}

	while (!termino) {
		if (cadena[indice] != NULL
				&& (isalpha(cadena[indice]) || cadena[indice] == '\\')) {
			term = (cadena[indice++]);
		}
		if (cadena[indice] != NULL
				&& (isalpha(cadena[indice]) || cadena[indice] == '\\')) {
			noTerm = (cadena[indice++]);
		}
		if (cadena[indice] != NULL && cadena[indice] == '|') {
			agregarProduccion(gramatica, pIz, term, noTerm);
			term = 0, noTerm = 0;
			indice++;
		} else if (cadena[indice] == NULL) {
			termino = 1;
			agregarProduccion(gramatica, pIz, term, noTerm);
			term = 0, noTerm = 0;
		} else {
			Error(
					"Hay mas de dos elementos en la parte derecha de la produccion\n");
		}
	}
}

void agregarProduccion(gramatica_t gramatica, noTerminal_t pIz, terminal_t t,
		noTerminal_t nT) {
	if (!esProduccionValida(gramatica, pIz, t, nT)) {
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

/*
 * Retorna 1 si hubo algun cambio en el conjunto de producciones
 * */
int eliminarProduccionesUnitarias(gramatica_t gramatica) {

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

int esProduccionValida(gramatica_t gramatica, char pIz, char elem1, char elem2) {

	//parte izquierda debe ser no terminal
	if (!isNoTerminal(gramatica, pIz))
		return 0;
	//los elementos de la parte derecha o bien son terminal o no terminal o lambda
	if (!isNoTerminal(gramatica, elem1) && !isTerminal(gramatica, elem1) && elem1 != '\\')
		return 0;
	if (!isNoTerminal(gramatica, elem2) && !isTerminal(gramatica, elem2) && elem2 != '\\')
		return 0;
	if(elem1 == 0 && elem2 == 0)
		return 0;
	if(elem1 == 0 || elem2 == 0)
		return 1;
	if(elem1 == '\\' || elem2 == '\\')
		return 1;
	//ambos elementos de la parte derecha no pueden ser terminales a la vez
	if(isTerminal(gramatica, elem1) && isTerminal(gramatica, elem2))
		return 0;
	//ambos elementos de la parte derecha no pueden ser no terminales a la vez
	if(isNoTerminal(gramatica, elem1) && isNoTerminal(gramatica, elem2))
		return 0;

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
