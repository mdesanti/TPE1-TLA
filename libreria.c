#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char * reemplazar(char * cadena, char * original, char *reemplazo) {

	int largo = strlen(cadena);
	char * ret = malloc(largo * sizeof(char));
	int i = 0, j = 0;

	while (i < largo) {
		if (cadena[i] != *original) {
			ret[j++] = cadena[i];
		} else {
			if (*reemplazo != "") {
				ret[j++] = *reemplazo;
			}
		}
		i++;
	}
	ret[j] = NULL;


	return ret;
}

char * quitarEspacios(char * cadena) {
	int largo = strlen(cadena);
	char * ret = malloc(largo * sizeof(char));
	int i = 0, j = 0;

	while (i < largo) {
		if (cadena[i] != ' ') {
			ret[j++] = cadena[i];
		}
		i++;
	}
	ret[j] = NULL;


	return ret;
}
