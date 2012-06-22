#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef int(*pt2Func)(int *, char *);
char ** array;
int logIndex = 0;
int procedimientoS(int * index, char * word);

int procedimientoA(int * index, char * word);

int procedimientoB(int * index, char * word);

pt2Func funcionPara(char noTerm);
void add(char * prod);
void undo();
void clear();


int main(int argc, char ** argv) {
	array = malloc(10*sizeof(char *));
	int index = 0;
	char * word = argv[1];
	if(procedimientoS(&index, word)) {
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

int procedimientoS(int * index, char * word) {
	int noerror = 1;


	int backup;
	backup = *index;
	add("S->AB");
	noerror = procesar(index, word, "AB");
	if(noerror && word[*index] == '\0'){
		return 1;
	}

	clear();
	*index = backup;
	backup = *index;
	add("S->aBbc");
	noerror = procesar(index, word, "aBbc");
	if(noerror && word[*index] == '\0'){
		return 1;
	}

	clear();
	*index = backup;
	return 0;
}

int procedimientoA(int * index, char * word) {
	int noerror = 1;


	int backup;
	backup = *index;
	add("A->bB");
	noerror = procesar(index, word, "bB");
	if(noerror){
		return 1;
	}

	undo();
	*index = backup;
	backup = *index;
	add("A->-");
	noerror = procesar(index, word, "-");
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
	add("B->aAA");
	noerror = procesar(index, word, "aAA");
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
	if(noTerm == 'S') {
		return &procedimientoS;
	}

	if(noTerm == 'A') {
		return &procedimientoA;
	}

	if(noTerm == 'B') {
		return &procedimientoB;
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
void clear() {
	int i = 0;
	logIndex = 0;	while(array[i] != '\0') {
		array[i++] = '\0';
	};}
