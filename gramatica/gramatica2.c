/*
 * gramatica2.c
 *
 *  Created on: Jun 20, 2012
 *      Author: mdesanti90
 */

#include "gramatica2.h"
#include "../libreria.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct produccion {
	noTerminal_t parteIzquierda;
	noTerminal_t* parteDerecha;
};

struct gramatica {
	noTerminal_t *noTerminales;
	terminal_t *terminales;
	noTerminal_t simInicial;
	produccion_t *producciones;
	tipoG tipo;
};

char ** split(char elem, char * str);
void crearMain(gramatica_t gramatica, FILE * analizador);
void crearProcedimiento(gramatica_t gramatica, FILE * analizador,
		char noTerminal);
produccion_t * produccionesPara(gramatica_t gramatica, char noTerminal);
void crearBaseDeFunciones(gramatica_t gramatica, FILE * analizador);
char * removeElem(char * seq, char elem, int number);
int containsElem(char * seq, char elem);

static void Error(const char* s) {
	printf("%s\n", s);
	exit(EXIT_FAILURE);
}

gramatica_t nuevaGramatica(void) {
	gramatica_t g;
	if ((g = malloc(sizeof(struct gramatica))) == NULL) {
		Error("No hay lugar para otro nodo\n");
	}
	g->noTerminales = NULL;
	g->terminales = NULL;
	g->producciones = NULL;
	g->tipo = -1;
	return g;
}

void crearAnalizador(gramatica_t gramatica) {
	FILE * analizador = fopen("analizador.c", "w");
	printf("Producciones leidas:\n");
	//imprimirProd(gramatica);
	//eliminarProduccionesLambda(gramatica);
	//printf("Se eliminaron producciones lambda para facilitar el algoritmo. Las nuevas producciones son:\n");
	imprimirProd(gramatica);
	crearMain(gramatica, analizador);
	crearProcedimientos(gramatica, analizador);
	crearFuncionProcesar(gramatica, analizador);
	crearBaseDeFunciones(gramatica, analizador);
	crearFuncionDeLog(analizador);
	fclose(analizador);
}

void eliminarProduccionesLambda(gramatica_t g) {
	int cant = 0, i = 0, j = 0;
	char auxNT;
	for (i = 0; g->producciones[i] != NULL; i++) {
		cant++;
	}
	for (i = 0; i < cant; i++) {
		if (g->producciones[i]->parteDerecha[0] == '-') {
			auxNT = g->producciones[i]->parteIzquierda;
			for (j = 0; j < cant; j++) {
				int qty = 0;
				if (g->producciones[j]->parteDerecha[0] != '-'
						&& (qty = containsElem(g->producciones[j]->parteDerecha, g->producciones[i]->parteIzquierda))) {
					while (qty > 0) {
						char * pD = removeElem(g->producciones[j]->parteDerecha, auxNT, qty--);
						/*if(strlen(pD)==0){
							//pD = malloc(5*sizeof(char));
							pD[0]='\\';
							pD[1]='\0';
						}*/
						agregarProduccion(g, pD, g->producciones[j]->parteIzquierda);
						cant++;
					}
					//g->producciones[j]->noTerminal = NULL;
				}
			}
			if(g->producciones[i]->parteIzquierda != g->simInicial){
				eliminarProduccion(g, g->producciones[i]);
				i--;
				cant--;
			}
		}
	}
}

char * removeElem(char * seq, char elem, int number) {
	int i = 0, flag = 0, j = 0;
	char * ret = malloc(5*sizeof(char));
	while (seq[i] != '\0') {
		if(j % 5 == 0) {
			ret = realloc(ret, (j +5) * sizeof(char));
		}
		if (seq[i] == elem) {
			number--;
			if (number == 0) {
				flag = 1;
			}
		}
		if (flag) {
			ret[j++] = seq[i+1];
		} else {
			ret[j++] = seq[i];
		}
		i++;
	}
	return ret;
}

int containsElem(char * seq, char elem) {
	int i = 0, qty = 0;
	while (seq[i] != '\0') {
		if (seq[i++] == elem) {
			qty++;
		}
	}
	return qty;
}

void eliminarProduccion(gramatica_t g, produccion_t p) {
	int cant = 0;
	int i = 0;
	int flag = 0;
	for (i = 0; g->producciones[i] != NULL; i++) {
		cant++;
	}

	for (i = 0; i < cant; i++) {
		if (g->producciones[i]->parteIzquierda == p->parteIzquierda
				&& !strcmp(g->producciones[i]->parteDerecha, p->parteDerecha)) {
			flag = 1;
		}
		if (flag == 1) {
			//free?
			g->producciones[i] = g->producciones[i + 1];
		}
	}
	g->producciones[cant] = NULL;
	return;
}

void crearMain(gramatica_t gramatica, FILE * analizador) {
	fprintf(
			analizador,
			"#include <stdio.h>\n#include <stdlib.h>\n#include <ctype.h>\n#include <string.h>\n\n");
	fprintf(analizador, "typedef int(*pt2Func)(int *, char *);\n");
	fprintf(analizador, "char ** array;\n");
	fprintf(analizador, "int logIndex = 0;\n");
	fprintf(analizador, "int pDlen = 1;\n");
	fprintf(analizador, "int pDIndex = 0;\n");
	fprintf(analizador, "int nodosHoja = 0;\n");
	char * noTerminales = gramatica->noTerminales;
	int index = 0;

	while (noTerminales[index] != '\0') {
		fprintf(analizador,
				"int procedimiento%c(int * index, char * word);\n\n",
				noTerminales[index++]);
	}
	fprintf(analizador, "pt2Func funcionPara(char noTerm);\n");
	fprintf(analizador, "void add(char * prod);\n");
	fprintf(analizador, "void undo();\n");

	fprintf(analizador, "int main(int argc, char ** argv) {\n");
	fprintf(analizador, "\tarray = malloc(10*sizeof(char *));\n");
	fprintf(analizador, "\tint index = 0;\n\tchar * word = argv[1];\n");
	fprintf(analizador, "\tif(procedimiento%c(&index, word)) {\n",
			gramatica->simInicial);
	fprintf(analizador, "\t\tif(word[index] == \'\\0\'){\n");
	fprintf(analizador, "\t\t\tprintf(\"La cadena pertenece\\n\");\n");
	fprintf(analizador, "\t\t} else {\n");
	fprintf(analizador, "\t\t\tprintf(\"La cadena no pertenece\\n\");\n");
	fprintf(analizador, "\t\t}\n");
	fprintf(analizador, "\t} else {\n");
	fprintf(analizador, "\t\tprintf(\"La cadena no pertenece\\n\");\n");
	fprintf(analizador, "\t}\n");
	fprintf(analizador, "\tint i = 0;\n");
	fprintf(analizador, "\twhile(array[i] != \'\\0\') {\n");
	fprintf(analizador, "\t\tprintf(\"%%s\\n\",array[i++]);\n");
	fprintf(analizador, "\t}\n");
	fprintf(analizador, "}\n\n");
}

void crearProcedimientos(gramatica_t gramatica, FILE * analizador) {
	char * noTerminales = gramatica->noTerminales;
	int index = 0;

	while (noTerminales[index] != '\0') {
		crearProcedimiento(gramatica, analizador, noTerminales[index]);
		index++;
	}
}

void crearProcedimiento(gramatica_t gramatica, FILE * analizador,
		char noTerminal) {
	produccion_t * producciones = gramatica->producciones;
	produccion_t * produccionesDesdeNoTerm = produccionesPara(gramatica,
			noTerminal);
	int index = 0;
	fprintf(analizador, "int procedimiento%c(int * index, char * word) {\n",
			noTerminal);
	fprintf(analizador, "\tint noerror = 1;\n\n\n");
	fprintf(analizador, "\tint flag = 0;\n");
	fprintf(analizador, "\tint backup;\n");
	fprintf(analizador, "\tint pDlenbackup;\n");
	fprintf(analizador, "\tint pDIndexbackup;\n");
	fprintf(analizador, "\tint nodosHojabackup;\n");
	while (produccionesDesdeNoTerm[index] != NULL) {
		fprintf(analizador, "\tbackup = *index;\n");
		fprintf(analizador, "\tpDlenbackup = pDlen;\n");
		fprintf(analizador, "\tpDIndexbackup = pDIndex;\n");
		fprintf(analizador, "\tnodosHojabackup = nodosHoja;\n");
		fprintf(analizador, "\tadd(\"%c->%s\");\n", noTerminal,
				produccionesDesdeNoTerm[index]->parteDerecha);
		fprintf(analizador, "\tpDlen += %d;\n", strlen(produccionesDesdeNoTerm[index]->parteDerecha)-1);
		fprintf(analizador, "\tnodosHoja += %d;\n", contarTerminales(produccionesDesdeNoTerm[index]->parteDerecha));
		fprintf(analizador, "\tprintf(\"%c->%s\\n\");\n", noTerminal, produccionesDesdeNoTerm[index]->parteDerecha);
		//fprintf(analizador, "\tif(nodosHoja<=strlen(word)){\n");
		fprintf(analizador, "\tnoerror = procesar(index, word, \"%s\");\n",
				produccionesDesdeNoTerm[index]->parteDerecha);
		fprintf(analizador, "\tif(noerror){\n");
		if(produccionesDesdeNoTerm[index]->parteDerecha[0]!='-'){
			fprintf(analizador, "\t\tif(backup == *index)\n");
			fprintf(analizador, "\t\t\tflag = 1;\n");
			fprintf(analizador, "\t\telse\n");
			fprintf(analizador, "\t\t\treturn 1;\n");
		}else{
			fprintf(analizador, "\t\treturn 1;\n");
		}
		fprintf(analizador, "\t}\n");
		//fprintf(analizador, "\t}\n\n");
		fprintf(analizador, "\tundo();\n");
		fprintf(analizador, "\t*index = backup;\n");
		fprintf(analizador, "\tpDlen = pDlenbackup;\n");
		fprintf(analizador, "\tpDIndex = pDIndexbackup;\n");
		fprintf(analizador, "\tnodosHoja = nodosHojabackup;\n");
		index++;
	}
	fprintf(analizador, "\tif(flag)\n");
	fprintf(analizador, "\t\treturn 1;\n");
	fprintf(analizador, "\treturn 0;\n");
	fprintf(analizador, "}\n\n");

}

void crearFuncionProcesar(gramatica_t gramatica, FILE * analizador) {
	fprintf(analizador,
			"int procesar(int * index, char * word, char * seq) {\n");
	fprintf(analizador, "\tprintf(\"%%d\\t%%d\\n\", pDlen, pDIndex);\n");
	fprintf(analizador, "\tif(nodosHoja>strlen(word))\n");
	fprintf(analizador, "\t\treturn 0;\n");
	fprintf(analizador, "\tif(seq[0] == \'-\') {\n");
//	fprintf(analizador, "\t\t(*index)++;\n");
	fprintf(analizador, "\t\tpDlen--;\n");
	fprintf(analizador, "\t\tif(pDlen==pDIndex && word[*index]!=\'\\0\')\n");
	fprintf(analizador, "\t\t\treturn 0;\n");
	fprintf(analizador, "\t\telse\n");
	fprintf(analizador, "\t\t\treturn 1;\n");
	fprintf(analizador, "\t}\n");
	fprintf(analizador, "\tint seqIndex = 0;\n");
	fprintf(analizador, "\tint (*fp) (int *, char*);\n");
	fprintf(analizador, "\twhile(seq[seqIndex] != \'\\0\') {\n");
	fprintf(analizador, "\t\tif(islower(seq[seqIndex])) {\n");
	fprintf(analizador, "\t\t\tif(seq[seqIndex] == word[*index]) {\n");
	fprintf(analizador, "\t\t\t\t(*index)++;seqIndex++;\n");
	fprintf(analizador, "\t\t\t\tpDIndex++;\n");
	fprintf(analizador, "\t\t\t} else {\n\t\t\t\t return 0;\n\t\t\t}\n");
	fprintf(analizador, "\t\t} else if(isupper(seq[seqIndex])) {\n");
	fprintf(analizador, "\t\t\tfp = funcionPara(seq[seqIndex]);\n");
	fprintf(analizador, "\t\t\tif(!(*fp)(index, word)) {\n");
	fprintf(analizador, "\t\t\t\treturn 0;\n");
	fprintf(analizador, "\t\t\t} else {\n \t\t\t\t seqIndex++;\n\t\t\t}\n");
	fprintf(analizador, "\t\t}\n");
	fprintf(analizador, "\t}\n");
	fprintf(analizador, "\t return 1;\n");
	fprintf(analizador, "}\n\n");

}

void crearBaseDeFunciones(gramatica_t gramatica, FILE * analizador) {
	char * noTerminales = gramatica->noTerminales;
	int index = 0;

	fprintf(analizador, "pt2Func funcionPara(char noTerm) {\n");

	while (noTerminales[index] != '\0') {
		fprintf(analizador, "\tif(noTerm == \'%c\') {\n", noTerminales[index]);
		fprintf(analizador, "\t\treturn &procedimiento%c;\n",
				noTerminales[index]);
		fprintf(analizador, "\t}\n\n");
		index++;
	}
	fprintf(analizador, "}\n");
}

void crearFuncionDeLog(FILE * analizador) {
	fprintf(analizador, "void add(char * prod) {\n");
	fprintf(analizador, "\tif(logIndex % 10 == 0) {\n");
	fprintf(analizador,
			"\t\tarray = realloc(array, (logIndex + 10)*sizeof(char *));\n");
	fprintf(analizador, "\t}\n");
	fprintf(analizador, "\tint size = strlen(prod);\n");
	fprintf(analizador, "\tarray[logIndex] = malloc(size * sizeof(char));\n");
	fprintf(analizador, "\tstrcpy(array[logIndex++], prod);\n");
	fprintf(analizador, "}\n\n");

	fprintf(analizador, "void undo() {\n");
	fprintf(analizador, "\tarray[--logIndex] = \'\\0\';\n");
	fprintf(analizador, "}\n");
}

produccion_t * produccionesPara(gramatica_t gramatica, char noTerminal) {
	produccion_t * producciones = gramatica->producciones;
	produccion_t * ret = malloc(5 * sizeof(produccion_t));
	int retIndex = 0, prodIndex = 0;

	while (producciones[prodIndex] != NULL) {
		if (retIndex % 5 == 0) {
			realloc(ret, (retIndex + 5) * sizeof(produccion_t));
		}
		if (producciones[prodIndex]->parteIzquierda == noTerminal)
			ret[retIndex++] = producciones[prodIndex++];
		else
			prodIndex++;
	}

	ret[retIndex] = NULL;
	return ret;
}

void agregarProduccionDesdeCadena(gramatica_t gramatica, char * cadena) {
	char pIz = cadena[0];
	char * parteDerecha;
	char term = 0, noTerm = 0, seteada = 0;
	int termino = 0, indice = 3;

	if (pIz == '\\') {
		Error("Lambda no puede ser la parte izquierda de una produccion\n");
	}
	if (cadena[1] != '-' || cadena[2] != '>') {
		Error(
				"Debe haber un solo simbolo en la parte izquierda de una produccion\n");
	}

	parteDerecha = malloc((strlen(&(cadena[indice])) + 1) * sizeof(char));
	strcpy(parteDerecha, &(cadena[indice]));

	char ** producciones = split('|', parteDerecha);

	int i = 0;

	while (producciones[i] != '\0') {
		agregarProduccion(gramatica, producciones[i], pIz);
		i++;
	}

}

char ** split(char elem, char * str) {
	int i = 0;
	int prod = 0;
	char ** ret = malloc(5 * sizeof(char));

	while (str[i] != '\0') {
		if (prod % 5 == 0) {
			//lugar para 5 partes derechas adicionales
			realloc(ret, (prod + 5) * sizeof(char));
		}
		int index = 0;
		char * word = malloc(5 * sizeof(char));
		while (str[i] != '\0' && str[i] != elem) {
			if (index % 5 == 0) {
				realloc(word, (index + 5) * sizeof(char));
			}
			word[index++] = str[i++];
		}
		i++;
		if (index % 5 == 0) {
			realloc(word, (index + 1) * sizeof(char));
		}
		word[index] = '\0';
		ret[prod++] = word;
	}

	ret[prod] = '\0';

	return ret;
}

int agregarProduccion(gramatica_t gramatica, char * parteDerecha,
		noTerminal_t parteIzquierda) {

	if (parteDerecha[0] != '\\'
			&& !esProduccionValida(gramatica, parteIzquierda, parteDerecha)) {
		Error("La producci—n no es v‡lida");
	}
	if (parteDerecha[0] == '\\')
		parteDerecha[0] = '-';
	produccion_t *prod = gramatica->producciones;
	produccion_t aux;
	if ((aux = malloc(sizeof(struct produccion))) == NULL) {
		Error("No hay lugar para otro nodo\n");
	}
	aux->parteIzquierda = parteIzquierda;
	aux->parteDerecha = parteDerecha;
	int cant = 0;
	int i;
	if (prod == NULL) {
		prod = malloc(sizeof(struct produccion) * 2);
		prod[0] = aux;
		prod[1] = NULL;
	} else {
		for (i = 0; prod[i] != NULL; i++) {
			cant++;
		}
		for (i = 0; i < cant; i++) {
			if (gramatica->producciones[i]->parteIzquierda == parteIzquierda
					&& !strcmp((gramatica->producciones[i])->parteDerecha,
							parteDerecha)) {
				return 0;
			}
		}
		if ((prod = realloc(prod, (cant + 2) * sizeof(produccion_t))) == NULL) {
			Error("No hay lugar para otro nodo\n");
		}
		prod[cant] = aux;
		prod[cant + 1] = NULL;
	}

	gramatica->producciones = prod;

	return 1;
}

void agregarNoTerminal(gramatica_t gramatica, noTerminal_t elemento) {
	if (!isupper(elemento)) {
		Error("los elementos no terminales tienen que estar en mayusculas\n");
	}

	noTerminal_t *noTer = gramatica->noTerminales;
	char aux[] = { elemento, '\0' };
	if (noTer == NULL) {
		noTer = malloc(sizeof(noTerminal_t) * 2);
		noTer[0] = elemento;
		noTer[1] = '\0';
	} else {
		if (esNoTerminal(gramatica, elemento)) {
			return;
		}
		if ((noTer = realloc(noTer, (strlen(noTer) + 1) * sizeof(noTerminal_t)))
				== NULL) {
			Error("No hay lugar para otro nodo\n");
		}
		noTer = strcat(noTer, aux);
	}

	gramatica->noTerminales = noTer;
}

void agregarTerminal(gramatica_t gramatica, terminal_t elemento) {
	if (!islower(elemento)) {
		Error("los elementos terminales tienen que estar en minusculas\n");
	}
	terminal_t *ter = gramatica->terminales;
	char aux[] = { elemento, '\0' };
	if (ter == NULL) {
		ter = malloc(sizeof(terminal_t) * 2);
		ter[0] = elemento;
		ter[1] = '\0';
	} else {
		if (esTerminal(gramatica, elemento)) {
			return;
		}
		if ((ter = realloc(ter, (strlen(ter) + 1) * sizeof(terminal_t)))
				== NULL) {
			Error("No hay lugar para otro nodo\n");
		}
		ter = strcat(ter, aux);
	}

	gramatica->terminales = ter;
}

void setearSimboloIncial(gramatica_t gramatica, noTerminal_t elemento) {
	gramatica->simInicial = elemento;
}

int esTerminal(gramatica_t gramatica, terminal_t elem) {
	terminal_t * terminales = gramatica->terminales;
	int i = 0;
	while (terminales[i] != '\0') {
		if (terminales[i] == elem)
			return 1;
		i++;
	}
	return 0;
}

int esNoTerminal(gramatica_t gramatica, char elem) {
	noTerminal_t * noTerminales = gramatica->noTerminales;
	int i = 0;
	while (noTerminales[i] != '\0') {
		if (noTerminales[i] == elem)
			return 1;
		i++;
	}
	return 0;
}

int esProduccionValida(gramatica_t gramatica, noTerminal_t pIz,
		char * parteDerecha) {

	if (!esNoTerminal(gramatica, pIz))
		return 0;

	int i = 0;

	while (parteDerecha[i] != '\0') {
		if (isupper(parteDerecha[i])) {
			if (!esNoTerminal(gramatica, parteDerecha[i]))
				return 0;
		} else {
			if (!esTerminal(gramatica, parteDerecha[i])) {
				return 0;
			}
		}
		i++;
	}

	return 1;
}

void imprimirProd(gramatica_t gramatica) {
	int i = 0;
	produccion_t * producciones = gramatica->producciones;
	while(producciones[i] != '\0') {
		printf("%c->%s\n", producciones[i]->parteIzquierda, producciones[i]->parteDerecha);
		i++;
	}
}

int contarTerminales(char * string){
	int count = 0;
	int i=0;
	for( i=0 ; string[i]!='\0' ; i++){
		if(islower(string[i]))
			count++;
	}
	return count;
}
