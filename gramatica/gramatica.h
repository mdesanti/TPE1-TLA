/*
 * gramatica.h
 *
 */

#ifndef GRAMATICA_H_
#define GRAMATICA_H_

#include "../defs.h"
#include "../automata/automata.h"

typedef struct gramatica *gramatica_t;

typedef struct produccion *produccion_t;

gramatica_t nuevaGramatica(void);

void agregarNoTerminal(gramatica_t gramatica, noTerminal_t elemento);

void agregarTerminal(gramatica_t gramatica, terminal_t elemento);

void agregarProduccion(gramatica_t gramatica, noTerminal_t pIz, terminal_t t, noTerminal_t nT);

void setearSimboloIncial(gramatica_t gramatica, noTerminal_t elemento);

void setearTipoG(gramatica_t gramatica, tipoG tipo);

gramatica_t normalizar(gramatica_t gramatica);

gramatica_t pasarAFormaNDerecha(gramatica_t gramatica);

automata_t convertiraAutomata(gramatica_t gramatica);

int isNoTerminal(gramatica_t g, char c);

int isTerminal(gramatica_t g, char c);

void imprimirGramatica(gramatica_t g);

#endif /* GRAMATICA_H_ */
