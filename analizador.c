#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef int(*pt2Func)(int *, char *);
char ** array;
int logIndex = 0;
int pDlen = 1;
int pDIndex = 0;
int procedimientoS(int * index, char * word);

int procedimientoB(int * index, char * word);

int procedimientoC(int * index, char * word);

pt2Func funcionPara(char noTerm);
void add(char * prod);
void undo();
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


	int flag = 0;
	int backup;
	int pDlenbackup;
	int pDIndexbackup;
	backup = *index;
	pDlenbackup = pDlen;
	pDIndexbackup = pDIndex;
	add("S->BC");
	pDlen += 1;
	noerror = procesar(index, word, "BC");
	if(noerror){
		if(backup == *index)
			flag = 1;
		else
			return 1;
	}

	undo();
	*index = backup;
	pDlen = pDlenbackup;
	pDIndex = pDIndexbackup;
	backup = *index;
	pDlenbackup = pDlen;
	pDIndexbackup = pDIndex;
	add("S->a");
	pDlen += 0;
	noerror = procesar(index, word, "a");
	if(noerror){
		if(backup == *index)
			flag = 1;
		else
			return 1;
	}

	undo();
	*index = backup;
	pDlen = pDlenbackup;
	pDIndex = pDIndexbackup;
	if(flag)
		return 1;	return 0;
}

int procedimientoB(int * index, char * word) {
	int noerror = 1;


	int flag = 0;
	int backup;
	int pDlenbackup;
	int pDIndexbackup;
	backup = *index;
	pDlenbackup = pDlen;
	pDIndexbackup = pDIndex;
	add("B->cS");
	pDlen += 1;
	noerror = procesar(index, word, "cS");
	if(noerror){
		if(backup == *index)
			flag = 1;
		else
			return 1;
	}

	undo();
	*index = backup;
	pDlen = pDlenbackup;
	pDIndex = pDIndexbackup;
	backup = *index;
	pDlenbackup = pDlen;
	pDIndexbackup = pDIndex;
	add("B->-");
	pDlen += 0;
	noerror = procesar(index, word, "-");
	if(noerror){
		if(backup == *index)
			flag = 1;
		else
			return 1;
	}

	undo();
	*index = backup;
	pDlen = pDlenbackup;
	pDIndex = pDIndexbackup;
	if(flag)
		return 1;	return 0;
}

int procedimientoC(int * index, char * word) {
	int noerror = 1;


	int flag = 0;
	int backup;
	int pDlenbackup;
	int pDIndexbackup;
	backup = *index;
	pDlenbackup = pDlen;
	pDIndexbackup = pDIndex;
	add("C->bC");
	pDlen += 1;
	noerror = procesar(index, word, "bC");
	if(noerror){
		if(backup == *index)
			flag = 1;
		else
			return 1;
	}

	undo();
	*index = backup;
	pDlen = pDlenbackup;
	pDIndex = pDIndexbackup;
	backup = *index;
	pDlenbackup = pDlen;
	pDIndexbackup = pDIndex;
	add("C->-");
	pDlen += 0;
	noerror = procesar(index, word, "-");
	if(noerror){
		if(backup == *index)
			flag = 1;
		else
			return 1;
	}

	undo();
	*index = backup;
	pDlen = pDlenbackup;
	pDIndex = pDIndexbackup;
	if(flag)
		return 1;	return 0;
}

int procesar(int * index, char * word, char * seq) {
	if(seq[0] == '-') {
		pDlen--;
		if(pDlen==pDIndex && word[*index]!='\0')
			return 0;
		else
			return 1;
	}
	int seqIndex = 0;
	int (*fp) (int *, char*);
	while(seq[seqIndex] != '\0') {
		if(islower(seq[seqIndex])) {
			if(seq[seqIndex] == word[*index]) {
				(*index)++;seqIndex++;
				pDIndex++;
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
