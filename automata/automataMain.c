/*
 * automataMain.c
 *
 *  Created on: May 19, 2012
 *      Author: mdesanti90
 */

#include <stdio.h>
#include <stdlib.h>
#include "automata.h"
#include "../defs.h"

int main(int argc, char **argv) {

	automata_t automata = nuevoAutomata();

	agregarEstado(automata, 0, 0);
	agregarEstado(automata, 1, 1);
	agregarEstado(automata, 2, 0);

	agregarTransicion(automata, 0, 0, 'a');
	agregarTransicion(automata, 0, 1, 'b');
	agregarTransicion(automata, 1, 2, 'b');
	agregarTransicion(automata, 2, 1, 'a');

	imprimirAutomata(automata);

	gramatica_t gramatica = crearGramatica(automata);
	imprimirGramatica(gramatica);
}

