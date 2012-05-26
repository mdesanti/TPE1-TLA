/*
 * defs.h
 *
 */

#ifndef DEFS_H_
#define DEFS_H_

typedef char noTerminal_t;

typedef char terminal_t;

typedef int estado_t;

typedef int tipoG;

typedef struct gramatica *gramatica_t;

typedef struct produccion *produccion_t;

typedef struct nodo *nodo_t;

typedef struct automata *automata_t;

typedef struct transicion transicion_t;


#define GLI	1
#define GLD	2

#define GRAMATICA 1
#define AUTOMATA 2

#endif /* DEFS_H_ */
