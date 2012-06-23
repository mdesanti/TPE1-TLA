#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef int(*pt2Func)(int *, char *);
char ** array;
int logIndex = 0;
int pDlen = 1;
int pDIndex = 0;
int nodosHoja = 0;
int procedimientoS(int * index, char * word);

int procedimientoA(int * index, char * word);

int procedimientoB(int * index, char * word);

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
	int nodosHojabackup;
	backup = *index;
	pDlenbackup = pDlen;
	pDIndexbackup = pDIndex;
	nodosHojabackup = nodosHoja;
	add("S->AB");
	pDlen += 1;
	nodosHoja += 0;
	printf("S->AB\n");
	noerror = procesar(index, word, "AB");
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
	nodosHoja = nodosHojabackup;
	backup = *index;
	pDlenbackup = pDlen;
	pDIndexbackup = pDIndex;
	nodosHojabackup = nodosHoja;
	add("S->aBbc");
	pDlen += 3;
	nodosHoja += 3;
	printf("S->aBbc\n");
	noerror = procesar(index, word, "aBbc");
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
	nodosHoja = nodosHojabackup;
	if(flag)
		return 1;
	return 0;
}

int procedimientoA(int * index, char * word) {
	int noerror = 1;


	int flag = 0;
	int backup;
	int pDlenbackup;
	int pDIndexbackup;
	int nodosHojabackup;
	backup = *index;
	pDlenbackup = pDlen;
	pDIndexbackup = pDIndex;
	nodosHojabackup = nodosHoja;
	add("A->bB");
	pDlen += 1;
	nodosHoja += 1;
	printf("A->bB\n");
	noerror = procesar(index, word, "bB");
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
	nodosHoja = nodosHojabackup;
	backup = *index;
	pDlenbackup = pDlen;
	pDIndexbackup = pDIndex;
	nodosHojabackup = nodosHoja;
	add("A->-");
	pDlen += 0;
	nodosHoja += 0;
	printf("A->-\n");
	noerror = procesar(index, word, "-");
	if(noerror){
		return 1;
	}
	undo();
	*index = backup;
	pDlen = pDlenbackup;
	pDIndex = pDIndexbackup;
	nodosHoja = nodosHojabackup;
	if(flag)
		return 1;
	return 0;
}

int procedimientoB(int * index, char * word) {
	int noerror = 1;


	int flag = 0;
	int backup;
	int pDlenbackup;
	int pDIndexbackup;
	int nodosHojabackup;
	backup = *index;
	pDlenbackup = pDlen;
	pDIndexbackup = pDIndex;
	nodosHojabackup = nodosHoja;
	add("B->aAA");
	pDlen += 2;
	nodosHoja += 1;
	printf("B->aAA\n");
	noerror = procesar(index, word, "aAA");
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
	nodosHoja = nodosHojabackup;
	if(flag)
		return 1;
	return 0;
}

int procesar(int * index, char * word, char * seq) {
	printf("%d\t%d\n", pDlen, pDIndex);
	if(nodosHoja>strlen(word))
		return 0;
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
