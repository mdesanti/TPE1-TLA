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
	agregarEstado(automata, 1, 0);
	agregarEstado(automata, 2, 0);
	agregarEstado(automata, 3, 1);
	char term = 'a';

	agregarTransicion(automata, 0, 1, term);
	term = 'b';
	agregarTransicion(automata, 0, 3, term);
	term = 'c';
	agregarTransicion(automata, 2, 0, term);

	imprimirAutomata(automata);
}

