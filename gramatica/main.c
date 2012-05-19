/*
 * main.c
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include "gramatica.h"
#include "defs.h"


int main(int argc, char **argv) {
	gramatica_t g;
	g = nuevaGramatica();
	agregarNoTerminal(g, 'A');
	agregarNoTerminal(g, 'B');
	agregarNoTerminal(g, 'C');
	agregarTerminal(g, 'a');
	agregarTerminal(g, 'b');
	agregarTerminal(g, 'c');
	setearSimboloIncial(g, 'A');
	setearTipoG(g, 1);
	agregarProduccion(g, 'A', 'a', 'B');
	agregarProduccion(g, 'A', 'c', 'C');
	agregarProduccion(g, 'B', 'b', NULL);
	agregarProduccion(g, 'C', NULL, NULL);
	imprimirGramatica(g);
	return 0;
}
