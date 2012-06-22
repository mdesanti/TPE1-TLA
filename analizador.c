#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef int(*pt2Func)(int *, char *);
char ** array;
int logIndex = 0;
int procedimientoS(int * index, char * word);

int procedimientoB(int * index, char * word);

int procedimientoC(int * index, char * word);

void add(char * prod);
void undo();


pt2Func funcionPara(char noTerm);
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
		add("S->BC");
	noerror = procesar(index, word, "BC");
	if(noerror){
		return 1;
	}

	undo();
	*index = backup;
	backup = *index;
		add("S->a");
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
		add("B->cS");
	noerror = procesar(index, word, "cS");
	if(noerror){
		return 1;
	}

	undo();
	*index = backup;
	backup = *index;
		add("B->-");
	noerror = procesar(index, word, "-");
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
		add("C->bC");
	noerror = procesar(index, word, "bC");
	if(noerror){
		return 1;
	}

	undo();
	*index = backup;
	backup = *index;
		add("C->-");
	noerror = procesar(index, word, "-");
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

	if(noTerm == 'B') {
		return &procedimientoB;
	}

	if(noTerm == 'C') {
		return &procedimientoC;
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
