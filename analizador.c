#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef int(*pt2Func)(int *, char *);
int procedimientoA(int * index, char * word);
int procedimientoB(int * index, char * word);
int procedimientoC(int * index, char * word);
int procedimientoD(int * index, char * word);
pt2Func funcionPara(char noTerm);
int main(int argc, char ** argv) {
	int index = 0;
	char * word = argv[1];
	if(procedimientoA(&index, word)) {
		if(word[index] == '\0'){
			printf("La cadena pertenece\n");
		}	} else {
		printf("La cadena no pertenece\n");
	}
}

int procedimientoA(int * index, char * word) {
	int noerror = 1;


	int backup;
	backup = *index;
	noerror = procesar(index, word, "aB");
	if(noerror){
		return 1;
	}

	*index = backup;
	backup = *index;
	noerror = procesar(index, word, "c");
	if(noerror){
		return 1;
	}

	*index = backup;
	return 0;
}

int procedimientoB(int * index, char * word) {
	int noerror = 1;


	int backup;
	backup = *index;
	noerror = procesar(index, word, "bC");
	if(noerror){
		return 1;
	}

	*index = backup;
	backup = *index;
	noerror = procesar(index, word, "-");
	if(noerror){
		return 1;
	}

	*index = backup;
	return 0;
}

int procedimientoC(int * index, char * word) {
	int noerror = 1;


	int backup;
	backup = *index;
	noerror = procesar(index, word, "bD");
	if(noerror){
		return 1;
	}

	*index = backup;
	backup = *index;
	noerror = procesar(index, word, "-");
	if(noerror){
		return 1;
	}

	*index = backup;
	return 0;
}

int procedimientoD(int * index, char * word) {
	int noerror = 1;


	int backup;
	backup = *index;
	noerror = procesar(index, word, "aBD");
	if(noerror){
		return 1;
	}

	*index = backup;
	backup = *index;
	noerror = procesar(index, word, "aC");
	if(noerror){
		return 1;
	}

	*index = backup;
	return 0;
}

int procesar(int * index, char * word, char * seq) {
	if(seq[0] == '-') {
		return 1;
	}
	int seqIndex = 0;
	int (*fp) (int *, char*);
	while(seq[seqIndex] != '\0') {
		if(islower(seq[seqIndex])) {
			if(seq[seqIndex] == word[*index]) {
				(*index)++;seqIndex++;
			} else {
				 return 0;
			}
		} else if(isupper(seq[seqIndex])) {
			fp = funcionPara(seq[seqIndex]);
			if(!(*fp)(index, word)) {
				return 0;
			} else {
 				 seqIndex++;
			}
		}
	}
	 return 1;
}

pt2Func funcionPara(char noTerm) {
	if(noTerm == 'A') {
		return &procedimientoA;
	}

	if(noTerm == 'B') {
		return &procedimientoB;
	}

	if(noTerm == 'C') {
		return &procedimientoC;
	}

	if(noTerm == 'D') {
		return &procedimientoD;
	}

}
