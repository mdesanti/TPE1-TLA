#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef int(*pt2Func)(int *, char *);
char ** array;
int logIndex = 0;
int procedimientoA(int * index, char * word);

int procedimientoB(int * index, char * word);

int procedimientoC(int * index, char * word);

int procedimientoD(int * index, char * word);

pt2Func funcionPara(char noTerm);
void add(char * prod);
void undo();
int main(int argc, char ** argv) {
	array = malloc(10*sizeof(char *));
	int index = 0;
	char * word = argv[1];
	if(procedimientoA(&index, word)) {
		if(word[index] == '\0'){
			printf("La cadena pertenece\n");
		} else {
			printf("La cadena no pertenece\n");
		}
	} else {
		printf("La cadena no pertenece\n");
	}
	int i = 0;
	while(array[i] != '\0') {
		printf("%s\n",array[i++]);
	}
}

int procedimientoA(int * index, char * word) {
	int noerror = 1;


	int backup;
	backup = *index;
	add("A->aB");
	noerror = procesar(index, word, "aB");
	if(noerror){
		return 1;
	}

	undo();
	*index = backup;
	backup = *index;
	add("A->c");
	noerror = procesar(index, word, "c");
	if(noerror){
		return 1;
	}

	undo();
	*index = backup;
	backup = *index;
	add("A->a");
	noerror = procesar(index, word, "a");
	if(noerror){
		return 1;
	}

	undo();
	*index = backup;
	return 0;
}

int procedimientoB(int * index, char * word) {
	int noerror = 1;


	int backup;
	backup = *index;
	add("B->bC");
	noerror = procesar(index, word, "bC");
	if(noerror){
		return 1;
	}

	undo();
	*index = backup;
	backup = *index;
	add("B->b");
	noerror = procesar(index, word, "b");
	if(noerror){
		return 1;
	}

	undo();
	*index = backup;
	return 0;
}

int procedimientoC(int * index, char * word) {
	int noerror = 1;


	int backup;
	backup = *index;
	add("C->bD");
	noerror = procesar(index, word, "bD");
	if(noerror){
		return 1;
	}

	undo();
	*index = backup;
	return 0;
}

int procedimientoD(int * index, char * word) {
	int noerror = 1;


	int backup;
	backup = *index;
	add("D->aBD");
	noerror = procesar(index, word, "aBD");
	if(noerror){
		return 1;
	}

	undo();
	*index = backup;
	backup = *index;
	add("D->aC");
	noerror = procesar(index, word, "aC");
	if(noerror){
		return 1;
	}

	undo();
	*index = backup;
	backup = *index;
	add("D->aD");
	noerror = procesar(index, word, "aD");
	if(noerror){
		return 1;
	}

	undo();
	*index = backup;
	backup = *index;
	add("D->a");
	noerror = procesar(index, word, "a");
	if(noerror){
		return 1;
	}

	undo();
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
void add(char * prod) {
	if(logIndex % 10 == 0) {
		array = realloc(array, (logIndex + 10)*sizeof(char *));
	}
	int size = strlen(prod);
	array[logIndex] = malloc(size * sizeof(char));
	strcpy(array[logIndex++], prod);
}

void undo() {
	array[--logIndex] = '\0';
}
