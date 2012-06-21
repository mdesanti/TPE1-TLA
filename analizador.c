#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char ** argv) {
	int index = 0;
	char * word = argv[1];
	if(funcionA(&index, word)) {
		if(word[index] == '\0'){
			printf("La cadena pertenece");
		} else {
			 printf("La cadena no pertenece");
		}
}