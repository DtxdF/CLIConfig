#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "strip.h"
#include "getline_m.h"
#include "conf_parser.h"

inline size_t count_toequal(char * buffer) {
	size_t i;

	for (i = 0; buffer[i] != '=' && buffer[i] != '#' && buffer[i] != '\0'; i++) {
	
	}

	return i;
}

inline size_t count_tozero(char * buffer, size_t offset) {
	size_t i;

	for (i = 0; buffer[i+offset] != '#' && buffer[i+offset] != '\0'; i++) {
	
	}

	return i;

}

int ini_parse(FILE * file, void * config, conf_parser_func func) {
	char * buffer = NULL;
	char * key = NULL;
	char * value = NULL;
	char * lstrip_ptr;
	char * rstrip_ptr;

	size_t key_size = 0;
	size_t value_size = 0;
	size_t buff_size;
	size_t lstrip_size;

	int key_index;
	int value_index;
	int errcod;

	while(getline_m(&buffer, file) != NULL) {
		buff_size = strlen(buffer);

		if (buff_size == 0) {
			continue;
		
		}

		lstrip(buffer, &lstrip_ptr, buff_size+1);

		if (!lstrip_ptr) {
			break;
		
		}

		lstrip_size = strlen(lstrip_ptr);

		rstrip(lstrip_ptr, &rstrip_ptr, lstrip_size);

		if (!rstrip_ptr) {
			break;
		
		}

		key_size = count_toequal(rstrip_ptr)+1;
		value_size = count_tozero(rstrip_ptr, key_size)+1;

		key = (char *)malloc(key_size);
		value = (char *)malloc(value_size);

		if ((key) || (value)) {
			for (key_index = 0; rstrip_ptr[key_index] != '=' && rstrip_ptr[key_index] != '#' && rstrip_ptr[key_index] != '\0'; key_index++) {
				key[key_index] = rstrip_ptr[key_index];
			
			}

			key[key_index] = '\0';

			key_index++;
			for (value_index = 0; rstrip_ptr[key_index] != '#' && rstrip_ptr[key_index] != '\0'; key_index++, value_index++) {
				value[value_index] = rstrip_ptr[key_index];
			
			}

			value[value_index] = '\0';

			if (key_size-1 != 0) {
				errcod = func(config, key, value);

			}

			free(key);
			free(value);

			if (errcod != 0) {
				break;
			
			}

		}

		free(lstrip_ptr);
		free(rstrip_ptr);
	
	}

	free(buffer);
	
	return errcod;

}
