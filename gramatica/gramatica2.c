/*
 * gramatica2.c
 *
 *  Created on: Jun 20, 2012
 *      Author: mdesanti90
 */

#include "gramatica2.h"
#include "../libreria.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct produccion {
	noTerminal_t parteIzquierda;
	noTerminal_t* parteDerecha;
};

struct gramatica {
	noTerminal_t *noTerminales;
	terminal_t *terminales;
	noTerminal_t simInicial;
	produccion_t *producciones;
	tipoG tipo;
};

char ** split(char elem, char * str);
void crearMain(gramatica_t gramatica, FILE * analizador);
void crearProcedmiento(gramatica_t gramatica, char noTerminal);
void crearProcedimientos(gramatica_t gramatica, FILE * analizador);

static void Error(const char* s) {
	printf("%s\n", s);
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
	g->tipo = -1;
	return g;
}

void crearAnalizador(gramatica_t gramatica) {
	FILE * analizador = fopen("analizador.c", "w");
	crearMain(gramatica, analizador);
	crearProcedimientos(gramatica, analizador);
//	crearFuncionProcesar(gramatica, analizador);
	fclose(analizador);
}

void crearMain(gramatica_t gramatica, FILE * analizador) {
	fprintf(
			analizador,
			"#include <stdio.h>\n#include <stdlib.h>\n#include <ctype.h>\n#include <string.h>\n\n");
	fprintf(analizador, "int main(int argc, char ** argv) {\n");
	fprintf(analizador, "\tint index = 0;\n\tchar * word = argv[1];\n");
	fprintf(analizador, "\tif(funcion%c(&index, word)) {\n",
			gramatica->simInicial);
	fprintf(analizador, "\t\tif(word[index] == \'\\0\'){\n");
	fprintf(analizador, "\t\t\tprintf(\"La cadena pertenece\");\n");
	fprintf(analizador, "\t\t} else {\n");
	fprintf(analizador, "\t\t\tprintf(\"La cadena no pertenece\");\n");
	fprintf(analizador, "\t\t}\n");
	fprintf(analizador, "}");
}

void crearProcedimientos(gramatica_t gramatica, FILE * analizador) {
	char * noTerminales = gramatica->noTerminales;
	int index = 0;

	while(noTerminales[index] != '\0') {
		crearProcedimiento(gramatica, noTerminales[index]);
	}
}

void crearProcedmiento(gramatica_t gramatica, FILE * analizador, char noTerminal) {
	fprintf(analizador)
}

void agregarProduccionDesdeCadena(gramatica_t gramatica, char * cadena) {
	char pIz = cadena[0];
	char * parteDerecha;
	char term = 0, noTerm = 0, seteada = 0;
	int termino = 0, indice = 3;

	if (pIz == '/') {
		Error("Lambda no puede ser la parte izquierda de una produccion\n");
	}
	if (cadena[1] != '-' || cadena[2] != '>') {
		Error(
				"Debe haber un solo simbolo en la parte izquierda de una produccion\n");
	}

	parteDerecha = malloc((strlen(&(cadena[indice])) + 1) * sizeof(char));
	strcpy(parteDerecha, &(cadena[indice]));

	char ** producciones = split('|', parteDerecha);

	int i = 0;

	while (producciones[i] != '\0') {
		agregarProduccion(gramatica, producciones[i], pIz);
		printf("Agregue: %c -> %s\n", pIz, producciones[i]);
		i++;
	}

}

char ** split(char elem, char * str) {
	int i = 0;
	int prod = 0;
	char ** ret = malloc(5 * sizeof(char));

	while (str[i] != '\0') {
		if (prod % 5 == 0) {
			//lugar para 5 partes derechas adicionales
			realloc(ret, (prod + 5) * sizeof(char));
		}
		int index = 0;
		char * word = malloc(5 * sizeof(char));
		while (str[i] != '\0' && str[i] != elem) {
			if (index % 5 == 0) {
				realloc(word, (index + 5) * sizeof(char));
			}
			word[index++] = str[i++];
		}
		i++;
		if (index % 5 == 0) {
			realloc(word, (index + 1) * sizeof(char));
		}
		word[index] = '\0';
		ret[prod++] = word;
	}

	ret[prod] = '\0';

	return ret;
}

int agregarProduccion(gramatica_t gramatica, char * parteDerecha,
		noTerminal_t parteIzquierda) {

	if (!esProduccionValida(gramatica, parteIzquierda, parteDerecha)) {
		Error("La producci—n no es v‡lida");
	}
	produccion_t *prod = gramatica->producciones;
	produccion_t aux;
	if ((aux = malloc(sizeof(struct produccion))) == NULL) {
		Error("No hay lugar para otro nodo\n");
	}
	aux->parteIzquierda = parteIzquierda;
	aux->parteDerecha = parteDerecha;
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
		for (i = 0; i < cant; i++) {
			if (gramatica->producciones[i]->parteIzquierda == parteIzquierda
					&& !strcmp((gramatica->producciones[i])->parteDerecha,
							parteDerecha)) {
				return 0;
			}
		}
		if ((prod = realloc(prod, (cant + 2) * sizeof(produccion_t))) == NULL) {
			Error("No hay lugar para otro nodo\n");
		}
		prod[cant] = aux;
		prod[cant + 1] = NULL;
	}

	gramatica->producciones = prod;

	return 1;
}

void agregarNoTerminal(gramatica_t gramatica, noTerminal_t elemento) {
	if (!isupper(elemento)) {
		Error("los elementos no terminales tienen que estar en mayusculas\n");
	}

	noTerminal_t *noTer = gramatica->noTerminales;
	char aux[] = { elemento, '\0' };
	if (noTer == NULL) {
		noTer = malloc(sizeof(noTerminal_t) * 2);
		noTer[0] = elemento;
		noTer[1] = '\0';
	} else {
		if (esNoTerminal(gramatica, elemento)) {
			return;
		}
		if ((noTer = realloc(noTer, (strlen(noTer) + 1) * sizeof(noTerminal_t)))
				== NULL) {
			Error("No hay lugar para otro nodo\n");
		}
		noTer = strcat(noTer, aux);
	}

	gramatica->noTerminales = noTer;
}

void agregarTerminal(gramatica_t gramatica, terminal_t elemento) {
	if (!islower(elemento)) {
		Error("los elementos terminales tienen que estar en minusculas\n");
	}
	terminal_t *ter = gramatica->terminales;
	char aux[] = { elemento, '\0' };
	if (ter == NULL) {
		ter = malloc(sizeof(terminal_t) * 2);
		ter[0] = elemento;
		ter[1] = '\0';
	} else {
		if (esTerminal(gramatica, elemento)) {
			return;
		}
		if ((ter = realloc(ter, (strlen(ter) + 1) * sizeof(terminal_t)))
				== NULL) {
			Error("No hay lugar para otro nodo\n");
		}
		ter = strcat(ter, aux);
	}

	gramatica->terminales = ter;
}

void setearSimboloIncial(gramatica_t gramatica, noTerminal_t elemento) {
	gramatica->simInicial = elemento;
}

int esTerminal(gramatica_t gramatica, terminal_t elem) {
	terminal_t * terminales = gramatica->terminales;
	int i = 0;
	while (terminales[i] != '\0') {
		if (terminales[i] == elem)
			return 1;
		i++;
	}
	return 0;
}

int esNoTerminal(gramatica_t gramatica, char elem) {
	noTerminal_t * noTerminales = gramatica->noTerminales;
	int i = 0;
	while (noTerminales[i] != '\0') {
		if (noTerminales[i] == elem)
			return 1;
		i++;
	}
	return 0;
}

int esProduccionValida(gramatica_t gramatica, noTerminal_t pIz,
		char * parteDerecha) {

	if (!esNoTerminal(gramatica, pIz))
		return 0;

	int i = 0;

	while (parteDerecha[i] != '\0') {
		if (isupper(parteDerecha[i])) {
			if (!esNoTerminal(gramatica, parteDerecha[i]))
				return 0;
		} else {
			if (!esTerminal(gramatica, parteDerecha[i])) {
				return 0;
			}
		}
		i++;
	}

	return 1;
}
