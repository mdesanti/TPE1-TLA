/*
 * gramatica2.h
 *
 *  Created on: Jun 20, 2012
 *      Author: mdesanti90
 */

#ifndef GRAMATICA2_H_
#define GRAMATICA2_H_

#include "../defs.h"

gramatica_t nuevaGramatica(void);

void agregarProduccionDesdeCadena(gramatica_t gramatica, char * cadena);

int agregarProduccion(gramatica_t gramatica, char * parteDerecha,
		noTerminal_t parteIzquierda);

void agregarNoTerminal(gramatica_t gramatica, noTerminal_t elemento);

void agregarTerminal(gramatica_t gramatica, terminal_t elemento);

void setearSimboloIncial(gramatica_t gramatica, noTerminal_t elemento);

int esNoTerminal(gramatica_t gramatica, char elem);

int esProduccionValida(gramatica_t gramatica, noTerminal_t pIz, char * parteDerecha);

void crearAnalizador(gramatica_t gramatica);

int esTerminal(gramatica_t gramatica, terminal_t elem);

#endif /* GRAMATICA2_H_ */
