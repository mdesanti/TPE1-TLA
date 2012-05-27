/*
 * automata.c
 *
 */

#include "automata.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct nodo {
	estado_t nroNodo;
	int cantDeTransiciones;
	transicion_t *transiciones;
	char * nombre;
	int final;
};

struct transicion {
	terminal_t terminal;
	nodo_t destino;
};

struct automata {
	nodo_t *estados;
	int cantDeEstados;
	int cantDeTerminales;
	tipoG tipo;
	terminal_t *terminales;
};

nodo_t nuevoEstado(char * nombre, int nroEstado, int estadoFinal);
transicion_t nuevaTransicion(nodo_t destino, terminal_t terminal);
void agregarTransicionInterna(nodo_t origen, nodo_t destino,
		terminal_t terminal);
nodo_t obtenerNodo(automata_t automata, char * nombre);
void imprimirEstado(nodo_t estado);
void imprimirTransicion(transicion_t transicion);
void analizarEstado(gramatica_t gramatica, nodo_t estado);
void agregarTerminalAutomata(automata_t automata, terminal_t terminal);
void imprimirTransiciones(FILE * dotFile, nodo_t estado);

static void Error(const char* s) {
	fprintf(stderr, s);
	exit(EXIT_FAILURE);
}

automata_t nuevoAutomata() {

	automata_t automata;

	if ((automata = malloc(sizeof(struct automata))) == NULL) {
		Error("No hay lugar para otro automata\n");
	}

	automata->estados = NULL;
	automata->cantDeEstados = 0;
	automata->cantDeTerminales = 0;

	return automata;
}

void agregarEstado(automata_t automata, char * nombre, int nroEstado,
		int estadoFinal) {

	if (automata->estados == NULL) {
		if ((automata->estados = malloc(2 * sizeof(nodo_t*))) == NULL) {
			Error("No hay lugar para otro estado\n");
		} else {
			automata->estados[0] = nuevoEstado(nombre, nroEstado, estadoFinal);
			automata->estados[1] = NULL;
			automata->cantDeEstados++;
		}
	} else {

		if ((automata->estados = realloc(automata->estados,
				(automata->cantDeEstados + 2) * sizeof(nodo_t*))) == NULL) {
			Error("No hay lugar para otro estado\n");
		}

		automata->estados[automata->cantDeEstados] = nuevoEstado(nombre,
				nroEstado, estadoFinal);
		automata->cantDeEstados++;
		automata->estados[automata->cantDeEstados] = NULL;

	}
}

void agregarTransicion(automata_t automata, char * estadoOrigen,
		char * estadoDestino, terminal_t terminal) {

	if (terminal != '\\')
		agregarTerminalAutomata(automata, terminal);

	nodo_t origen = obtenerNodo(automata, estadoOrigen);
	nodo_t destino = obtenerNodo(automata, estadoDestino);

	agregarTransicionInterna(origen, destino, terminal);

	return;

}

gramatica_t crearGramatica(automata_t automata) {
	gramatica_t gramatica = nuevaGramatica();
	setearTipoG(gramatica, GLD);
	int i = 0;

	for (i = 0; i < automata->cantDeTerminales; i++) {
		agregarTerminal(gramatica, automata->terminales[i]);
	}

	for (i = 0; i < automata->cantDeEstados; i++) {
		nodo_t estado = (automata->estados)[i];
		agregarNoTerminal(gramatica, 'A' + estado->nroNodo);
	}

	for (i = 0; i < automata->cantDeEstados; i++) {
		analizarEstado(gramatica, (automata->estados)[i]);
	}

	return gramatica;
}

void crearArchivoDOT(automata_t automata) {
	FILE * dotFile = fopen("automata.dot", "a");
	char * write = "digraph{\nrankdir = \"LR\";";
	fprintf(dotFile, "digraph{\nrankdir = \"LR\"\n");
	int i = 0;
	for (i = 0; i < automata->cantDeEstados; i++) {
		nodo_t estado = (automata->estados[i]);
		char * shape;
		if (estado->final) {
			shape = "doublecircle";
		} else {
			shape = "circle";
		}

		fprintf(dotFile, "node[shape = %s] Node%d [label=\"%d\"];\n", shape,
				estado->nroNodo, estado->nroNodo);
	}

	for (i = 0; i < automata->cantDeEstados; i++) {
		imprimirTransiciones(dotFile, (automata->estados)[i]);
	}

	fprintf(dotFile, "}");
	fclose(dotFile);
}

void imprimirAutomata(automata_t automata) {
	int i = 0;
	printf("Simbolos terminales:\n");
	for (i = 0; i < automata->cantDeTerminales; i++) {
		printf("\t%c\n", automata->terminales[i]);
	}
	printf("Estados:\n");
	for (i = 0; i < automata->cantDeEstados; i++) {
		printf("\t%s", automata->estados[i]->nombre);
		if (automata->estados[i]->nroNodo == 0)
			printf(" - Es el estado inicial\n");
		else
			printf("\n");
	}
	printf("Estados finales:\n");
	for (i = 0; i < automata->cantDeEstados; i++) {
		if (automata->estados[i]->final)
			printf("\t%s\n", automata->estados[i]->nombre);
	}
	printf("Transiciones:\n");
	for (i = 0; i < automata->cantDeEstados; i++) {
		imprimirEstado(automata->estados[i]);
	}
}

/**
 * ***************De aca en adelante, funciones privadas para este .c*************************
 * */

void imprimirTransiciones(FILE * dotFile, nodo_t estado) {
	int trans = 0;

	char * muestra = "Node0->Node1 [label=\"a\"];";
	for (trans = 0; trans < estado->cantDeTransiciones; trans++) {
		transicion_t transicion = (estado->transiciones)[trans];
		fprintf(dotFile, "Node%d->Node%d [label=\"%c\"];\n", estado->nroNodo,
				(transicion.destino)->nroNodo, transicion.terminal);
	}
}

void analizarEstado(gramatica_t gramatica, nodo_t estado) {
	int trans = 0;
	char terminal;

	for (trans = 0; trans < estado->cantDeTransiciones; trans++) {
		transicion_t transicion = (estado->transiciones)[trans];
		if (transicion.terminal == '\\') {
			terminal = 0;
		} else {
			terminal = transicion.terminal;
		}
		agregarProduccion(gramatica, 'A' + estado->nroNodo, terminal,
				'A' + (transicion.destino)->nroNodo);
	}
}

void agregarTerminalAutomata(automata_t automata, terminal_t terminal) {

	if (automata->terminales == NULL) {
		if ((automata->terminales = malloc(10 * sizeof(terminal_t))) == NULL) {
			Error("No se puede agregar el no terminal\n");
		} else if ((automata->cantDeTerminales % 10) == 0) {
			if ((automata->terminales = realloc(automata->terminales,
					(automata->cantDeEstados + 10) * sizeof(char))) == NULL
			)
				Error("No se puede agregar el no terminal\n");
		}
	}

	int cantTerm = automata->cantDeTerminales, i = 0;
	;

	for (i = 0; i < cantTerm; i++) {
		if (automata->terminales[i] == terminal)
			return;
	}

	(automata->terminales)[i] = terminal;
	automata->cantDeTerminales++;
}

void imprimirEstado(nodo_t estado) {
	int trans = 0;

	for (trans = 0; trans < estado->cantDeTransiciones; trans++) {
		printf("delta(%d, %c) = %d\n", estado->nroNodo,
				estado->transiciones[trans].terminal,
				estado->transiciones[trans].destino->nroNodo);
	}
}

void imprimirTransicion(transicion_t transicion) {
	printf("voy con %c a %d\n", transicion.terminal,
			transicion.destino->nroNodo);
}

transicion_t nuevaTransicion(nodo_t destino, terminal_t terminal) {
	transicion_t transicion = *((transicion_t*) (malloc(sizeof(transicion_t))));

	transicion.destino = destino;
	transicion.terminal = terminal;

	return transicion;
}

void agregarTransicionInterna(nodo_t origen, nodo_t destino,
		terminal_t terminal) {

	if (origen->transiciones == NULL) {
		if ((origen->transiciones = ((transicion_t*) malloc(
				sizeof(transicion_t)))) == NULL) {
			Error("No hay lugar para otra transicion\n");
		}
		origen->transiciones[0] = nuevaTransicion(destino, terminal);
		origen->cantDeTransiciones++;
	} else {

		if ((origen->transiciones = realloc(origen->transiciones,
				sizeof(transicion_t) * (origen->cantDeTransiciones + 1)))
				== NULL) {
			Error("No hay lugar para otra transicion\n");
		} else {
			origen->transiciones[origen->cantDeTransiciones++] =
					nuevaTransicion(destino, terminal);
		}
	}

	return;
}

nodo_t obtenerNodo(automata_t automata, char * nombre) {
	nodo_t *estados = automata->estados;

	int i = 0;
	while (estados[i] != NULL && strcmp(estados[i]->nombre, nombre)) {
		i++;
	}

	if (estados[i] == NULL) {
		Error("No existe tal estado");
	}

	return estados[i];
}

nodo_t nuevoEstado(char * nombre, int nroEstado, int estadoFinal) {
	nodo_t ret;

	if ((ret = malloc(sizeof(struct nodo))) == NULL) {
		Error("No hay lugar para otro nodo\n");
	}

	ret->cantDeTransiciones = 0;
	ret->final = estadoFinal;
	ret->nroNodo = nroEstado;
	ret->nombre = nombre;
	ret->transiciones = NULL;

	return ret;
}

char * recuperarNombre(automata_t automata, int nroDeEstado) {
	int i = 0;
	for (i = 0; i < automata->cantDeEstados; i++) {
		if (automata->estados[i]->nroNodo == nroDeEstado)
			return automata->estados[i]->nombre;
	}
	return NULL;
}
