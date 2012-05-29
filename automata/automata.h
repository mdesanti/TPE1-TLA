/*
 * automata.h
 *
 */

#ifndef AUTOMATA_H_
#define AUTOMATA_H_

#include "../defs.h"
#include "../gramatica/gramatica.h"

typedef struct nodo *nodo_t;

typedef struct automata *automata_t;

typedef struct transicion transicion_t;

automata_t nuevoAutomata();

void agregarEstado(automata_t automata, char * nombre, int nroEstado,
		int estadoFinal);

void agregarTransicion(automata_t automata, char * estadoOrigen,
		char * estadoDestino, terminal_t terminal);

void imprimirAutomata(automata_t automata);

gramatica_t crearGramatica(automata_t automata);

void crearArchivoDOT(automata_t automata);

char * recuperarNombre(automata_t automata, int nroDeEstado);
int validarEstadoInicial(automata_t automata);

#endif /* AUTOMATA_H_ */
