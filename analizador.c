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


	int flag = 0;
	int backup;
	int pDlenbackup;
	int pDIndexbackup;
	int nodosHojabackup;
	backup = *index;
	pDlenbackup = pDlen;
	pDIndexbackup = pDIndex;
	nodosHojabackup = nodosHoja;
	add("A->aA");
	pDlen += 1;
	nodosHoja += 1;
	printf("A->aA\n");
	noerror = procesar(index, word, "aA");
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
	if(flag){
		pDlen--;
		if(pDlen==pDIndex && word[*index]!='\0')
			return 0;
		else{
			add("A->-");
			return 1;
		}
	}
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
	add("B->aC");
	pDlen += 1;
	nodosHoja += 1;
	printf("B->aC\n");
	noerror = procesar(index, word, "aC");
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
	add("B->bD");
	pDlen += 1;
	nodosHoja += 1;
	printf("B->bD\n");
	noerror = procesar(index, word, "bD");
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
	if(flag){
		pDlen--;
		if(pDlen==pDIndex && word[*index]!='\0')
			return 0;
		else{
			add("B->-");
			return 1;
		}
	}
	return 0;
}

int procedimientoC(int * index, char * word) {
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
	add("C->bD");
	pDlen += 1;
	nodosHoja += 1;
	printf("C->bD\n");
	noerror = procesar(index, word, "bD");
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
	if(flag){
		pDlen--;
		if(pDlen==pDIndex && word[*index]!='\0')
			return 0;
		else{
			add("C->-");
			return 1;
		}
	}
	return 0;
}

int procedimientoD(int * index, char * word) {
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
	add("D->aD");
	pDlen += 1;
	nodosHoja += 1;
	printf("D->aD\n");
	noerror = procesar(index, word, "aD");
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
	add("D->bD");
	pDlen += 1;
	nodosHoja += 1;
	printf("D->bD\n");
	noerror = procesar(index, word, "bD");
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
	add("D->-");
	pDlen += 0;
	nodosHoja += 0;
	printf("D->-\n");
	flag = 1;
	undo();
	*index = backup;
	pDlen = pDlenbackup;
	pDIndex = pDIndexbackup;
	nodosHoja = nodosHojabackup;
	if(flag){
		pDlen--;
		if(pDlen==pDIndex && word[*index]!='\0')
			return 0;
		else{
			add("D->-");
			return 1;
		}
	}
	return 0;
}

int procesar(int * index, char * word, char * seq) {
	printf("%d\t%d\n", pDlen, pDIndex);
	if(nodosHoja>strlen(word))
		return 0;
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
