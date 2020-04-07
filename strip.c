/*	strip.c
 *
 *	Eliminar espacios iniciales y finales
 *
 */

#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int lstrip(char * string, char **ptr, size_t length) {
	char * aux = NULL;

 	int whitespace;
	int result;
	int i;

	for (whitespace = 0; isspace(string[whitespace]); whitespace++) {

	
	}

	result = length-whitespace;
	aux = (char *)calloc(sizeof(char), result+1);

	if (!aux) {
		return errno;
	
	}

	for (i = 0; i < result; i++) {
		aux[i] = string[whitespace+i];

	}

	aux[i] = '\0';

	*ptr = aux;

	return 0;

}

int rstrip(char * string, char **ptr, size_t length) {
	char * aux = NULL;
	int i, j;
	int result;

	length--;

	for (i = 0; isspace(string[length-i]); i++) {
	
	}

	result = (length-i)+1;
	aux = (char *)calloc(sizeof(char), result+1);

	if (!aux) {
		return errno;

	}

	for (j = 0; j < result; j++) {
		aux[j] = string[j];
	
	}

	aux[j] = '\0';

	*ptr = aux;

	return 0;

}
