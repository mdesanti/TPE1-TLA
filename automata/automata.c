/*
 * automata.c
 *
 */

#include "automata.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct nodo
{
	estado_t nroNodo;
	int cantDeTransiciones;
	transicion_t *transiciones;
	int final;
};

struct transicion
{
	terminal_t terminal;
	nodo_t destino;
};

struct automata
{
	nodo_t *estados;
	int cantDeEstados;
//	terminal_t terminales;
	tipoG tipo;
};

nodo_t nuevoEstado(int nroEstado, int estadoFinal);
transicion_t nuevaTransicion(nodo_t destino, terminal_t terminal);
void agregarTransicionInterna(nodo_t origen, nodo_t destino, terminal_t terminal);
nodo_t obtenerNodo(automata_t automata, estado_t estadoOrigen);
void imprimirEstado(nodo_t estado);
void imprimirTransicion(transicion_t transicion);

static void Error(const char* s) {
	fprintf(stderr, s);
	exit(EXIT_FAILURE);
}

automata_t nuevoAutomata() {

	automata_t automata;

	if((automata = malloc(sizeof(struct automata))) == NULL) {
		Error("No hay lugar para otro automata\n");
	}

	automata->estados = NULL;

	return automata;
}

void agregarEstado(automata_t automata, int nroEstado, int estadoFinal) {

	if(automata->estados == NULL) {
		if((automata->estados = malloc(2*sizeof(nodo_t*))) == NULL) {
			Error("No hay lugar para otro estado\n");
		} else {
			automata->estados[0] = nuevoEstado(nroEstado, estadoFinal);
			automata->estados[1] = NULL;
			automata->cantDeEstados++;
		}
	} else {

		if((automata->estados = realloc(automata->estados, (automata->cantDeEstados + 2)*sizeof(nodo_t*))) == NULL) {
			Error("No hay lugar para otro estado\n");
		}

		automata->estados[automata->cantDeEstados] = nuevoEstado(nroEstado, estadoFinal);
		automata->cantDeEstados++;
		automata->estados[automata->cantDeEstados] = NULL;

		return;
	}
}

void agregarTransicion(automata_t automata, int estadoOrigen, int estadoDestino, terminal_t terminal) {

	nodo_t origen = obtenerNodo(automata, estadoOrigen);
	nodo_t destino = obtenerNodo(automata, estadoDestino);

	agregarTransicionInterna(origen, destino, terminal);

	return;

}

void imprimirAutomata(automata_t automata) {
	int i = 0;
	for(i = 0; i < automata->cantDeEstados; i++) {
		imprimirEstado(automata->estados[i]);
	}
}



/**
 * ***************De aca en adelante, funciones privadas para este .c*************************
 * */
void imprimirEstado(nodo_t estado) {
	printf("Nodo nro %d\n", estado->nroNodo);
	printf("\t Tiene %d transiciones\n", estado->cantDeTransiciones);
	int trans = 0;

	for(trans = 0; trans < estado->cantDeTransiciones; trans++) {
		printf("\t\t");
		imprimirTransicion(estado->transiciones[trans]);
	}
}

void imprimirTransicion(transicion_t transicion) {
	printf("voy con %c a %d\n", transicion.terminal, transicion.destino->nroNodo);
}

transicion_t nuevaTransicion(nodo_t destino, terminal_t terminal) {
	transicion_t transicion = *((transicion_t*)(malloc(sizeof(transicion_t))));

	transicion.destino = destino;
	transicion.terminal = terminal;

	return transicion;
}

void agregarTransicionInterna(nodo_t origen, nodo_t destino, terminal_t terminal) {

	if(origen->transiciones == NULL) {
		if((origen->transiciones = ((transicion_t*)malloc(sizeof(transicion_t)))) == NULL) {
			Error("No hay lugar para otra transicion\n");
		}
		origen->transiciones[0] = nuevaTransicion(destino, terminal);
		origen->cantDeTransiciones++;
	} else {

		if((origen->transiciones = realloc(origen->transiciones, sizeof(transicion_t)*(origen->cantDeTransiciones + 1))) == NULL) {
			Error("No hay lugar para otra transicion\n");
		} else {
			origen->transiciones[origen->cantDeTransiciones++] = nuevaTransicion(destino, terminal);
		}
	}

	return;
}

nodo_t obtenerNodo(automata_t automata, estado_t estadoOrigen) {
	nodo_t *estados = automata->estados;

	int i = 0;
	while(estados[i] != NULL && estados[i]->nroNodo != estadoOrigen) {
		i++;
	}

	if(estados[i] == NULL) {
		Error("No existe tal estado");
	}

	return estados[i];
}

nodo_t nuevoEstado(int nroEstado, int estadoFinal) {
	nodo_t ret;

	if((ret = malloc(sizeof(struct nodo))) == NULL) {
		Error("No hay lugar para otro nodo\n");
	}

	ret->cantDeTransiciones = 0;
	ret->final = estadoFinal;
	ret->nroNodo = nroEstado;
	ret->transiciones = NULL;

	return ret;
}