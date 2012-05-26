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

char * copiar(char * str) {
	int largo = strlen(str), i = 0;
	char * ret = malloc(largo * sizeof(char));

	while (str[i] != NULL) {
		ret[i] = str[i++];
	}
	ret[i] = NULL;
	return ret;
}

char * primeraParte(char * transicion) {
	char * sinEspacios = quitarEspacios(transicion);
	char * ret = malloc(10 * sizeof(char));
	int i = 0;

	while (sinEspacios[i] != '-') {
		if ((i % 10) == 0) {
			ret = realloc(ret, (i + 10) * sizeof(char));
		}
		ret[i] = sinEspacios[i++];
	}
	ret[i] = NULL;

	return ret;
}

char * segundaParte(char * transicion) {
	char * sinEspacios = quitarEspacios(transicion);
	char * ret = NULL;
	int i = 0, j = 0;

	while(sinEspacios[i] != '>')
		i++;
	i++;

	while (sinEspacios[i] != NULL) {
		if ((j % 10) == 0) {
			ret = realloc(ret, (j + 10) * sizeof(char));
		}
		ret[j++] = sinEspacios[i++];
	}
	ret[j] = NULL;

	return ret;
}
