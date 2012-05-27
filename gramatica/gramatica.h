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

int agregarProduccion(gramatica_t gramatica, noTerminal_t pIz, terminal_t t, noTerminal_t nT);

void setearSimboloIncial(gramatica_t gramatica, noTerminal_t elemento);

void setearTipoG(gramatica_t gramatica, tipoG tipo);

void normalizar(gramatica_t g);

gramatica_t pasarAFormaNDerecha(gramatica_t gramatica);

void crearArchivoGR(gramatica_t g);

//automata_t convertiraAutomata(gramatica_t gramatica);

int isNoTerminal(gramatica_t g, char c);

int isTerminal(gramatica_t g, char c);

void imprimirGramatica(gramatica_t g);

int isNormal(gramatica_t g);

int obtenerProducciones(gramatica_t g, noTerminal_t pIz, noTerminal_t noTer);

void agregarProduccionDesdeCadena(gramatica_t gramatica, char * cadena);

#endif /* GRAMATICA_H_ */
