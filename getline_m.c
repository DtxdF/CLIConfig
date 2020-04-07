#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

char * getline_m(char ** buff, FILE * file) {
	char c;
	long int i;

	if (*buff != NULL) {
		free(*buff);
		*buff = NULL;
	
	}

	if ((feof(file) != 0) || (ferror(file) != 0)) {
		return NULL;
	
	}
	
	for (i = 0; (c = fgetc(file)) != EOF; i++) {
		*buff = (char *)realloc(*buff, i+1);
		*(*buff+i) = c;

		if (c == '\n') {
			break;
		
		}
	
	}

	if (c != EOF) {
		*(*buff+i) = '\0';

	}

	return *buff;

}
