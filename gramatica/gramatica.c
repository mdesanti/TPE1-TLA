/*
 * gramatica.c
 *
 */
#include "gramatica.h"
#include "../libreria.h"
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
int obtEstado(gramatica_t g, noTerminal_t noTer);

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
		if (isNoTerminal(gramatica, elemento)) {
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
		Error("los elementos terminales tienen que estar en minusculas");
	}
	terminal_t *ter = gramatica->terminales;
	char aux[] = { elemento, '\0' };
	if (ter == NULL) {
		ter = malloc(sizeof(terminal_t) * 2);
		ter[0] = elemento;
		ter[1] = '\0';
	} else {
		if (isTerminal(gramatica, elemento)) {
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

int agregarProduccion(gramatica_t gramatica, noTerminal_t pIz, terminal_t t,
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
			if (gramatica->producciones[i]->parteIzquierda == pIz
					&& gramatica->producciones[i]->terminal == t
					&& gramatica->producciones[i]->noTerminal == nT) {
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

void setearSimboloIncial(gramatica_t gramatica, noTerminal_t elemento) {
	gramatica->simInicial = elemento;
}

void setearTipoG(gramatica_t gramatica, tipoG tipo2) {
	gramatica->tipo = tipo2;
}

void normalizar(gramatica_t g) {
	if (isNormal(g)) {
		return;
	}
	if (g->tipo == GLI) {
		g = pasarAFormaNDerecha(g);
	}
	noTerminal_t auxNT;
	int cant = 0;
	int i = 0;
	int j = 0;
	int aux2 = 0;
	int flag = 1;

	for (i = 0; g->producciones[i] != NULL; i++) {
		cant++;
	}

	//printf("primero: elimino producciones unitarias\n");
	while (flag == 1) {
		flag = 0;
		for (i = 0; i < cant; i++) {
			if (g->producciones[i]->terminal == NULL
					&& g->producciones[i]->noTerminal != NULL) {
				flag = 1;
				aux2 = obtenerProducciones(g,
						g->producciones[i]->parteIzquierda,
						g->producciones[i]->noTerminal);
				cant = cant + aux2;
				eliminarProduccion(g, g->producciones[i]);
				cant--;
			}
		}
	}

	//printf("segundo: elimino producciones lambda\n");
	for (i = 0; i < cant; i++) {
		if (g->producciones[i]->terminal == NULL
				&& g->producciones[i]->noTerminal == NULL) {
			auxNT = g->producciones[i]->parteIzquierda;
			for (j = 0; j < cant; j++) {
				if (g->producciones[j]->terminal != NULL
						&& g->producciones[j]->noTerminal == auxNT) {
					agregarProduccion(g, g->producciones[j]->parteIzquierda,
							g->producciones[j]->terminal, NULL);
					cant++;
					//g->producciones[j]->noTerminal = NULL;
				}
			}
			eliminarProduccion(g, g->producciones[i]);
			i--;
			cant--;
		}
	}

	//printf("tercero: elimino improductivos\n");
	noTerminal_t * productivos;
	char aux[2];
	productivos = malloc(sizeof(noTerminal_t) * 1);
	productivos[0] = '\0';
	//agrego a la lista de productivos a los que tienen un terminal en alguna produccion
	for (i = 0; i < cant; i++) {
		if (!strchr(productivos, g->producciones[i]->parteIzquierda)) {
			if (g->producciones[i]->terminal != NULL
					&& g->producciones[i]->noTerminal == NULL) {
				aux[0] = g->producciones[i]->parteIzquierda;
				aux[1] = '\0';
				if ((productivos = realloc(productivos,
						(strlen(productivos) + 1) * sizeof(noTerminal_t)))
						== NULL) {
					Error("No hay lugar para otro elemento\n");
				}
				productivos = strcat(productivos, aux);
			}
		}
	}
	//agrego a la lista de productivos a los que tienen a algun no terminal productivo en alguna produccion
	for (i = 0; i < cant; i++) {
		if (!strchr(productivos, g->producciones[i]->parteIzquierda)) {
			for (j = 0; j < strlen(productivos); j++) {
				if (g->producciones[i]->noTerminal == productivos[j]) {
					aux[0] = g->producciones[i]->parteIzquierda;
					aux[1] = '\0';
					if ((productivos = realloc(productivos,
							(strlen(productivos) + 1) * sizeof(noTerminal_t)))
							== NULL) {
						Error("No hay lugar para otro elemento\n");
					}
					productivos = strcat(productivos, aux);
				}
			}
		}
	}
	//elimino los improductivos (si los hay)
	for (i = 0; i < strlen(g->noTerminales); i++) {
		if (!strchr(productivos, g->noTerminales[i])) {
			for (j = 0; j < cant; j++) {
				if (g->producciones[j]->parteIzquierda == g->noTerminales[i]
						|| g->producciones[j]->noTerminal == g->noTerminales[i]) {
					eliminarProduccion(g, g->producciones[j]);
					cant--;
				}
			}
			eliminarNoTerminal(g, g->noTerminales[i]);
		}
	}

	//printf("cuarto: elimino inalcanzables\n");
	noTerminal_t * alcanzables;
	alcanzables = malloc(sizeof(noTerminal_t) * 2);
	alcanzables[0] = g->simInicial;
	alcanzables[1] = '\0';
	flag = 1;
	//printf("\tidentifico los alcanzables\n");
	while (flag == 1) {
		flag = 0;
		for (i = 0; i < cant; i++) {
			for (j = 0; j < strlen(alcanzables); j++) {
				if (g->producciones[i]->parteIzquierda == alcanzables[j]
						&& !strchr(alcanzables, g->producciones[i]->noTerminal)) {
					aux[0] = g->producciones[i]->noTerminal;
					aux[1] = '\0';
					if ((alcanzables = realloc(alcanzables,
							(strlen(alcanzables) + 1) * sizeof(noTerminal_t)))
							== NULL) {
						Error("No hay lugar para otro elemento\n");
					}
					alcanzables = strcat(alcanzables, aux);
					flag = 1;
				}
			}
		}
	}
	//printf("\telimino los incalcanzables (si los hay)\n");
	for (i = 0; i < strlen(g->noTerminales); i++) {
		if (!strchr(alcanzables, g->noTerminales[i])) {
			for (j = 0; j < cant; j++) {
				if (g->producciones[j]->parteIzquierda == g->noTerminales[i]
						|| g->producciones[j]->noTerminal == g->noTerminales[i]) {
					eliminarProduccion(g, g->producciones[j]);
					cant--;
				}
			}
			eliminarNoTerminal(g, g->noTerminales[i]);
		}
	}

	//printf("quinto: paso a forma normal\n");
	noTerminal_t nuevoNT = 'A';
	flag = 1;
	while (flag == 1) {
		flag = 0;
		if (strchr(g->noTerminales, nuevoNT)) {
			nuevoNT = nuevoNT + 1;
			flag = 1;
		}
	}
	agregarNoTerminal(g, nuevoNT);
	agregarProduccion(g, nuevoNT, NULL, NULL);
	for (i = 0; i < cant; i++) {
		if (g->producciones[i]->noTerminal == NULL) {
			g->producciones[i]->noTerminal = nuevoNT;
		}
	}

}

gramatica_t pasarAFormaNDerecha(gramatica_t g) {
	if (g->tipo == GLD) {
		return g;
	} else if (g->tipo != GLI) {
		Error("La tipo de la gramatica no es un tipo valido");
	}
	int i = 0;
	int cant = 0;
	int cantLambda = 0;
	int flagTer = 0;

	for (i = 0; g->producciones[i] != NULL; i++) {
		cant++;
	}

	for (i = 0; i < cant; i++) {
		if (g->producciones[i]->noTerminal == NULL
				&& g->producciones[i]->terminal == NULL) {
			cantLambda++;
		} else if (g->producciones[i]->noTerminal == NULL
				&& g->producciones[i]->terminal != NULL) {
			flagTer = 1;
		}
	}

	noTerminal_t nuevoNT = 'A';
	int flag = 1;
	while (flag == 1) {
		flag = 0;
		if (strchr(g->noTerminales, nuevoNT)) {
			nuevoNT = nuevoNT + 1;
			flag = 1;
		}
	}

	if (flagTer == 1) {
		agregarNoTerminal(g, nuevoNT);
		agregarProduccion(g, nuevoNT, NULL, NULL);
		cantLambda++;
		for (i = 0; i < cant; i++) {
			if (g->producciones[i]->noTerminal == NULL
					&& g->producciones[i]->terminal != NULL) {
				g->producciones[i]->noTerminal = nuevoNT;
			}
		}
	}

	if (cantLambda == 0) {
		agregarNoTerminal(g, nuevoNT);
		agregarProduccion(g, nuevoNT, NULL, NULL);
		for (i = 0; i < cant; i++) {
			if (g->producciones[i]->noTerminal == NULL
					&& g->producciones[i]->terminal != NULL) {
				g->producciones[i]->noTerminal = nuevoNT;
			}
		}
	} else if (cantLambda > 1) {
		agregarNoTerminal(g, nuevoNT);
		agregarProduccion(g, nuevoNT, NULL, NULL);
		for (i = 0; i < cant; i++) {
			if (g->producciones[i]->noTerminal == NULL
					&& g->producciones[i]->terminal == NULL) {
				g->producciones[i]->noTerminal = nuevoNT;
			}
		}
		cant++;
	}
	gramatica_t nuevaG = nuevaGramatica();
	for (i = 0; i < strlen(g->noTerminales); i++) {
		agregarNoTerminal(nuevaG, g->noTerminales[i]);
	}

	for (i = 0; i < strlen(g->terminales); i++) {
		agregarTerminal(nuevaG, g->terminales[i]);
	}

	for (i = 0; i < cant; i++) {
		if (g->producciones[i]->noTerminal == NULL
				&& g->producciones[i]->terminal == NULL) {
			setearSimboloIncial(nuevaG, g->producciones[i]->parteIzquierda);
		} else {
			agregarProduccion(nuevaG, g->producciones[i]->noTerminal,
					g->producciones[i]->terminal,
					g->producciones[i]->parteIzquierda);
		}
	}
	agregarProduccion(nuevaG, g->simInicial, NULL, NULL);
	setearTipoG(nuevaG, GLD);

	return nuevaG;
}

automata_t convertiraAutomata(gramatica_t g) {
	if (!isNormal(g) || g->tipo != GLD) {
		normalizar(g);
	}
	int i = 0;
	int j = 0;
	int cont = 1;
	int cant = 0;
	int final = 0;
	automata_t automata = nuevoAutomata();
	int estados[strlen(g->noTerminales)];for
(	i=0; g->producciones[i] != NULL; i++) {
		cant++;
	}
	for (i = 0; i < strlen(g->noTerminales); i++) {
		for (j = 0; j < cant; j++) {
			if (g->producciones[j]->parteIzquierda == g->noTerminales[i]
					&& g->producciones[j]->terminal == NULL
					&& g->producciones[j]->noTerminal == NULL) {
				final = 1;
			}
		}
		if (g->noTerminales[i] != g->simInicial) {
			estados[i] = cont;
			agregarEstado(automata, crearNombreEstado(cont), cont, final);
			cont++;
		} else {
			estados[i] = 0;
			agregarEstado(automata, crearNombreEstado(0), 0, final);
		}
		final = 0;
	}
	for (i = 0; i < cant; i++) {
		if (g->producciones[i]->noTerminal != NULL
				&& g->producciones[i]->terminal != NULL) {
			agregarTransicion(automata,
					estados[obtEstado(g, g->producciones[i]->parteIzquierda)],
					estados[obtEstado(g, g->producciones[i]->noTerminal)],
					g->producciones[i]->terminal);
		}
	}

	return automata;
}

/*
 * Retorna 1 si hubo algun cambio en el conjunto de producciones
 * */
int eliminarProduccionesUnitarias(gramatica_t gramatica) {

}

void crearArchivoGR(gramatica_t g) {
	FILE * grFile = fopen("prueba.gr", "w");
	int i, j, cant;
	cant = 0;
	noTerminal_t auxNT;
	fprintf(grFile, "G=({");
	for (i = 0; i < strlen(g->noTerminales) - 1; i++) {
		fprintf(grFile, "%c,", g->noTerminales[i]);
	}
	fprintf(grFile, "%c},{", g->noTerminales[i]);
	for (i = 0; i < strlen(g->terminales) - 1; i++) {
		fprintf(grFile, "%c,", g->terminales[i]);
	}
	fprintf(grFile, "%c},", g->terminales[i]);
	fprintf(grFile, "%c,{", g->simInicial);
	for (i = 0; g->producciones[i] != NULL; i++) {
		cant++;
	}
	for (i = 0; i < strlen(g->noTerminales) - 1; i++) {

		fimprimirProducciones(g, g->noTerminales[i], grFile);
		fprintf(grFile, ", ");
	}

	fimprimirProducciones(g, g->noTerminales[i], grFile);
	fprintf(grFile, "})\n");
	fclose(grFile);
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
	if (!isNoTerminal(gramatica, elem1) && !isTerminal(gramatica, elem1)
			&& elem1 != '\\')
		return 0;
	if (!isNoTerminal(gramatica, elem2) && !isTerminal(gramatica, elem2)
			&& elem2 != '\\')
		return 0;
	if (elem1 == 0 && elem2 == 0)
		return 0;
	if (elem1 == 0 || elem2 == 0)
		return 1;
	if (elem1 == '\\' || elem2 == '\\')
		return 1;
	//ambos elementos de la parte derecha no pueden ser terminales a la vez
	if (isTerminal(gramatica, elem1) && isTerminal(gramatica, elem2))
		return 0;
	//ambos elementos de la parte derecha no pueden ser no terminales a la vez
	if (isNoTerminal(gramatica, elem1) && isNoTerminal(gramatica, elem2))
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
	for (i = 0; i < strlen(g->noTerminales) - 1; i++) {

		imprimirProducciones(g, g->noTerminales[i]);
		printf(", ");
	}

	imprimirProducciones(g, g->noTerminales[i]);
	printf("})\n");

}

int isNormal(gramatica_t g) {
	int cant = 0;
	int i = 0;
	for (i = 0; g->producciones[i] != NULL; i++) {
		cant++;
	}

	for (i = 0; i < cant; i++) {
		if ((g->producciones[i]->terminal == NULL
				&& g->producciones[i]->noTerminal != NULL)
				|| (g->producciones[i]->terminal != NULL
						&& g->producciones[i]->noTerminal == NULL)) {
			return 0;
		}
	}
	return 1;
}

void eliminarProduccion(gramatica_t g, produccion_t p) {
	int cant = 0;
	int i = 0;
	int flag = 0;
	for (i = 0; g->producciones[i] != NULL; i++) {
		cant++;
	}

	for (i = 0; i < cant; i++) {
		if (g->producciones[i]->parteIzquierda == p->parteIzquierda
				&& g->producciones[i]->noTerminal == p->noTerminal
				&& g->producciones[i]->terminal == p->terminal) {
			flag = 1;
		}
		if (flag == 1) {
			//free?
			g->producciones[i] = g->producciones[i + 1];
		}
	}
	g->producciones[cant] = NULL;
	return;
}

int obtenerProducciones(gramatica_t g, noTerminal_t pIz, noTerminal_t noTer) {
	int cant = 0;
	int i = 0;
	int sum = 0;
	int aux = 0;
	for (i = 0; g->producciones[i] != NULL; i++) {
		cant++;
	}
	for (i = 0; i < cant; i++) {
		if (g->producciones[i]->parteIzquierda == noTer) {
			if (!(g->producciones[i]->terminal == NULL
					&& g->producciones[i]->noTerminal != NULL)) {
				sum = sum
						+ agregarProduccion(g, pIz,
								g->producciones[i]->terminal,
								g->producciones[i]->noTerminal);
			} else if (g->producciones[i]->noTerminal != pIz) {
				aux = aux
						+ obtenerProducciones(g, pIz,
								g->producciones[i]->noTerminal);
			}
		}
	}
	return sum + aux;

}

void eliminarNoTerminal(gramatica_t g, noTerminal_t noT) {
	int i;
	int flag = 0;
	for (i = 0; i < strlen(g->noTerminales); i++) {
		if (g->noTerminales[i] == noT) {
			flag = 1;
		}
		if (flag == 1) {
			g->noTerminales[i] = g->noTerminales[i + 1];
		}
	}

}

void imprimirProducciones(gramatica_t g, noTerminal_t pIz) {
	int i = 0;
	int j = 0;
	int cantProd = 0;
	int cant = 0;
	for (i = 0; g->producciones[i] != NULL; i++) {
		if (g->producciones[i]->parteIzquierda == pIz) {
			cantProd++;
		}
	}
	for (i = 0; g->producciones[i] != NULL; i++) {
		cant++;
	}

	printf("%c -> ", pIz);
	i = 0;
	for (j = 0; j < cant && i < cantProd - 1; j++) {
		if (g->producciones[j]->parteIzquierda == pIz) {
			i++;
			if (g->producciones[j]->terminal == NULL
					&& g->producciones[j]->noTerminal == NULL) {
				printf("\\ | ");
			} else {
				if (g->tipo == GLD) {
					printf("%c%c | ", g->producciones[j]->terminal,
							g->producciones[j]->noTerminal);
				} else if (g->tipo == GLI) {
					printf("%c%c | ", g->producciones[j]->noTerminal,
							g->producciones[j]->terminal);
				} else {
					Error("El tipo de la gramatica no es un tipo valido");
				}
			}
		}
	}

	for (; j < cant && i < cantProd; j++) {
		if (g->producciones[j]->parteIzquierda == pIz) {
			i++;
			if (g->producciones[j]->terminal == NULL
					&& g->producciones[j]->noTerminal == NULL) {
				printf("\\");
			} else {
				if (g->tipo == GLD) {
					printf("%c%c", g->producciones[j]->terminal,
							g->producciones[j]->noTerminal);
				} else if (g->tipo == GLI) {
					printf("%c%c", g->producciones[j]->noTerminal,
							g->producciones[j]->terminal);
				} else {
					Error("El tipo de la gramatica no es un tipo valido");
				}
			}
		}
	}

}

void fimprimirProducciones(gramatica_t g, noTerminal_t pIz, FILE *grFile) {
	int i = 0;
	int j = 0;
	int cantProd = 0;
	int cant = 0;
	for (i = 0; g->producciones[i] != NULL; i++) {
		if (g->producciones[i]->parteIzquierda == pIz) {
			cantProd++;
		}
	}
	for (i = 0; g->producciones[i] != NULL; i++) {
		cant++;
	}

	fprintf(grFile, "%c -> ", pIz);
	i = 0;
	for (j = 0; j < cant && i < cantProd - 1; j++) {
		if (g->producciones[j]->parteIzquierda == pIz) {
			i++;
			if (g->producciones[j]->terminal == NULL
					&& g->producciones[j]->noTerminal == NULL) {
				fprintf(grFile, "\\ | ");
			} else {
				if (g->tipo == GLD) {
					fprintf(grFile, "%c%c | ", g->producciones[j]->terminal,
							g->producciones[j]->noTerminal);
				} else if (g->tipo == GLI) {
					fprintf(grFile, "%c%c | ", g->producciones[j]->noTerminal,
							g->producciones[j]->terminal);
				} else {
					Error("El tipo de la gramatica no es un tipo valido");
				}
			}
		}
	}

	for (; j < cant && i < cantProd; j++) {
		if (g->producciones[j]->parteIzquierda == pIz) {
			i++;
			if (g->producciones[j]->terminal == NULL
					&& g->producciones[j]->noTerminal == NULL) {
				fprintf(grFile, "\\");
			} else {
				if (g->tipo == GLD) {
					fprintf(grFile, "%c%c", g->producciones[j]->terminal,
							g->producciones[j]->noTerminal);
				} else if (g->tipo == GLI) {
					fprintf(grFile, "%c%c", g->producciones[j]->noTerminal,
							g->producciones[j]->terminal);
				} else {
					Error("El tipo de la gramatica no es un tipo valido");
				}
			}
		}
	}

}

int obtEstado(gramatica_t g, noTerminal_t noTer) {
	int i = 0;
	for (i = 0; i < strlen(g->noTerminales) && g->noTerminales[i] != noTer;
			i++) {
	}
	return i;
}
