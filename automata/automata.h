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

void agregarEstado(automata_t automata, int nroEstado, int estadoFinal);

void agregarTransicion(automata_t automata, int estadoOrigen, int estadoDestino, terminal_t terminal);

void imprimirAutomata(automata_t automata);

gramatica_t crearGramatica(automata_t automata);

#endif /* AUTOMATA_H_ */
