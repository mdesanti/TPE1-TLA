#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

char * reemplazar(char * cadena, char * original, char *reemplazo) {

	int largo = strlen(cadena);
	char * ret = malloc(largo * sizeof(char));
	int i = 0, j = 0;

	while (i < largo) {
		if (cadena[i] != *original) {
			ret[j++] = cadena[i];
		} else {
			if ((*reemplazo) != "") {
				ret[j++] = *reemplazo;
			}
		}
		i++;
	}
	ret[j] = '\0';

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
	ret[j] = '\0';

	return ret;
}

char * copiar(char * str) {
	int largo = strlen(str), i = 0;
	char * ret = malloc(largo * sizeof(char));

	while (str[i] != '\0') {
		ret[i] = str[i++];
	}
	ret[i] = '\0';
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
	ret[i] = '\0';

	return ret;
}

char * segundaParte(char * transicion) {
	char * sinEspacios = quitarEspacios(transicion);
	char * ret = '\0';
	int i = 0, j = 0;

	while(sinEspacios[i] != '>')
		i++;
	i++;

	while (sinEspacios[i] != '\0') {
		if ((j % 10) == 0) {
			ret = realloc(ret, (j + 10) * sizeof(char));
		}
		ret[j++] = sinEspacios[i++];
	}
	ret[j] = '\0';

	return ret;
}

char * crearNombreEstado(int nroEstado) {
	int digits;
	if(nroEstado == 0) {
		digits = 1;
	} else {
		digits = log10((double)nroEstado) + 1;
	}
	char * ret = malloc((5+digits)*sizeof(char));
	char * nro = malloc(2*sizeof(char));
	ret[0] = 'N';
	ret[1] = 'o';
	ret[2] = 'd';
	ret[3] = 'e';
	ret[4] = '\0';
	nro[0] = nroEstado + '0';
	nro[1] = '\0';
	strcat(ret, nro);

	return ret;

}
