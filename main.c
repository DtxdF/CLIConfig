#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <errno.h>

#include "conf_parser.h"
#include "main.h"
#include "exists.h"

// CALLBACKS
int show_config(void * config, char * key, char * value); // Mostramos la configuración
int write_config(FILE * file, char * key, char * value); // Escribimos en la configuración
int overwrite_config(void * config, char * name, char * val); // Sobre-escribimos la configuración
/* Detectamos si nuestro amigo "strtol" devuelve un error, sí es así
   muestra un mensaje */
int invalid_number(char * strton, const char * message);
/* Reiniciamos valores auxiliares. Útil para cuando hay muchos
   archivos */
inline void reset_values();
void usage();

extern char * key;
extern char * value;
extern char * expression;
extern int do_delete;
extern int execute_exit;
extern long only;
extern long only_aux;
extern long identifier;
extern long id_aux;
extern long patterns;
extern long pattern_aux;

int main(int argc, char *argv[]) {
	// La configuración del archivo temporal
	struct tmp_config tmp_conf;
	char tmp_name[26];

	FILE * file;

	// Los valores por defecto
	int do_show = 0;
	int do_help = 0;
	int do_write = 0;
	int do_overwrite = 0;
	int do_no_banner = 1;

	// Para getopt
	int opt;
	int index = 0;
	int errcod;
	char * shortopts = ":hsk:wv:noE:O:i:p:d";

	char * filename;

	struct option options[] = {
		{"help",       no_argument,       &do_help,      1 },
		{"show",       no_argument,       &do_show,      1 },
		{"key",        required_argument, NULL,         'k'},
		{"write",      no_argument,       &do_write,     1 },
		{"value",      required_argument, NULL,         'v'},
		{"overwrite",  no_argument,       &do_overwrite, 1 },
		{"no-banner",  no_argument,       &do_no_banner, 0 },
		{"expression", required_argument, NULL,         'E'},
		{"only",       required_argument, NULL,         'O'},
		{"id",         required_argument, NULL,         'i'},
		{"pattern",    required_argument, NULL,         'p'},
		{"delete",     required_argument, NULL,         'd'},
		{0,            0,                 0,             0 }
	
	};

	opterr = 0;

	while ((opt = getopt_long(argc, argv, shortopts, options, NULL)) != -1) {
		index++;

		switch (opt) {
			case 0:
				break;

			case 'w':
				do_write = 1;
				break;

			case 'd':
				do_overwrite = 1;
				do_delete = 1;
				break;
			case 'o':
				do_overwrite = 1;
				break;

			case 's':
				do_show = 1;
				break;

			case 'h':
				do_help = 1;
				break;

			case 'v':
				value = optarg;
				break;

			case 'k':
				key = optarg;
				break;

			case 'n':
				do_no_banner = 0;
				break;

			case 'O':
				only = invalid_number(optarg, "Número de coincidencias incorrecto");
				break;

			case 'i':
				identifier = invalid_number(optarg, "Identificador incorrecto");
				break;

			case 'p':
				patterns = invalid_number(optarg, "El número de patrones no es correcto");
				break;

			case 'E':
				expression = optarg;
				break;

			case ':':
				printf("\"-%c\" o \"--%s\" requiere un argumento\n",
					   optopt, options[index].name);
				break;

			case '?':
			default:
				usage();

				fprintf(stderr, "\n");
				fprintf(stderr, "Error: Use los parámetros -h, --help para mostrar la ayuda\n");

				return EXIT_FAILURE; 

		
		}
	
	}

	if ((index == 0) && (opt == -1)) {
		usage();

		fprintf(stderr, "\n");
		fprintf(stderr, "Error: Debes indicar una opción para poder continuar.\n");

		return EXIT_FAILURE;
	
	}

	if (do_help) {
		usage();

		return EXIT_SUCCESS;
	
	}

	if ((argc-optind) == 0) {
		fprintf(stderr, "Debe escribir la ruta de por lo menos un archivo para poder continuar\n");

		return EXIT_FAILURE;
	
	}

	for (; optind < argc; optind++) {
		filename = argv[optind];

		if (do_no_banner) {
			printf("[Procesando: %s]\n\n", filename);

		}

		if (!exists(filename)) {
			perror("¡No se pudo detectar el archivo de configuración!");

			return errno;
		
		}

		if ((file = fopen(filename, "a+b")) == NULL) {
			perror("¡No se su pudo abrir el archivo de configuración!");

			return errno;
		
		}

		if (do_show) {
			if ((errcod = ini_parse(file, NULL, show_config)) != 0) {
				perror("No se pudo leer la configuración");
			
			} else {
				if (key) {
					if (!execute_exit) {
						fprintf(stderr, "¡No se encontro ninguna coincidencia!\n");
					
					}
				
				}

			}
			
		} else if (do_write) {
			if ((!key) || (!value)) {
				fprintf(stderr, "¡Debe definir la clave y el valor para poder continuar!\n");

				return 3;
			
			}

			if ((errcod = write_config(file, key, value)) != 0) {
				perror("Ocurrió un error leyendo el archivo de configuración");

				return errcod;
			
			} else {
				if (strncmp("-", value, sizeof(char)) == 0) {
					printf("Escrito: %s\n", key);
				
				} else {
					printf("Escrito: %s=%s\n", key, value);
				
				}
			
			}

		} else if (do_overwrite) {
			if (!value && !do_delete) {
				fprintf(stderr, "¡Debe definir el valor para poder sobre-escribir!\n");

				return 3;
			
			}

			if (!key && do_delete) {
				fprintf(stderr, "Debe definir la clave a eliminar\n");

				return 3;
			
			} else {
				if (!key && !expression) {
					fprintf(stderr, "Debe definir la clave o una expresión para usarla como coincidencia\n");
				
					return 3;

				}

			}

			strncpy(tmp_name, "CLIConfig-XXXXXX", sizeof(char)*17);

			tmp_conf.fd = mkstemp(tmp_name);

			if ((errcod = ini_parse(file, &tmp_conf, overwrite_config)) != 0) {
				perror("¡No se pudo sobre-escribir la clave en ese archivo de configuración!");
			
				return errno;

			} else {
				if (execute_exit == 1) {
					if (do_delete) {
						printf("¡%s ha sido eliminado con éxito!\n", key);
					
					} else {
						printf("¡%s ha sido modificado con éxito!\n", (key ? key : expression));

					}

					if (rename(tmp_name, filename) < 0) {
						fprintf(stderr, "No se pudo re-escribir los datos modificados\n");

						return errno;
					
					}
				
				} else {
					fprintf(stderr, "No hubo coincidencias para modificar\n");
				
				}
			
			}

			close(tmp_conf.fd);
			unlink(tmp_name);
		
		} else {
			fprintf(stderr, "Error: ¡Opción no encontrada!\n");

			return 3;
		
		}

		if (do_no_banner) {
			printf("\n[Procesado: %s]\n", filename);

		}

		fclose(file);

		reset_values();

	}

	return errno;

}

int invalid_number(char * strton, const char * message) {
	long n;
	char * endstr;

	n = strtol(strton, &endstr, 10);

	if (errno != 0) {
		perror(message);
		exit(errno);
	
	}

	return n;

}

inline void reset_values() {
	execute_exit = 0;
	only_aux = 0;
	id_aux = 0;
	pattern_aux = 0;

}

inline void print_if(char * name, char * value) {
	if (strlen(value) == 0) {
		printf("%s\n", name);
		
	} else {
		printf("%s=%s\n", name, value);

	}

}

inline unsigned int only_count() {
	if (only == 0) {
		return 0;
	
	}

	if (only == only_aux) {
		return 1;
	
	} else {
		only_aux++;
		return 0;
	
	}

}

inline unsigned int identified() {
	if (identifier == 0) {
		return 0;
	
	}
	
	id_aux++;

	if (identifier == id_aux) {
		return 0;
	
	}

	return 1;

}

inline unsigned int pattern_count() {
	pattern_aux++;

	if ((patterns != pattern_aux) && (patterns != 0)) {
		return 1;
	
	}

	return 0;

}

inline unsigned int isdeleted(char * name) {
	if ((!do_delete) || (!key)) {
		return 0;
	
	}

	if (strcmp(key, name) == 0) {
		if (!pattern_count() || patterns == 0) {
			execute_exit = 1;

			return 1;
		
		}
	
	}

	return 0;

}

int show_config(void * config, char * name, char * value) {
	int found = 0;

	if ((only_count()) || identified()) {
		return 0;
	}

	if (key) {
		if (strcmp(key, name) == 0) {
			found = 1;

		}
	
	} else if (expression) {
		if (strcmp(expression, value) == 0) {
			found = 1;
		
		}
	
	} else {
		print_if(name, value);
	
	}

	if (found) {
		if (pattern_count()) {
			return 0;
		
		}

		execute_exit = 1; // Coincidencia encontrado :-D
		print_if(name, value);
		
	
	}

	return 0;

}

int write_config(FILE * file, char * key, char * value) {
	if ((only_count()) || identified()) {
		return 0;
	}

	fwrite(key, sizeof(char), strlen(key), file);
	if (strncmp("-", value, sizeof(char)) != 0) {
		fwrite("=", sizeof(char), 1, file);
		fwrite(value, sizeof(char), strlen(value), file);

	}
	fwrite("\n", sizeof(char), 1, file);
	
	return errno;

}

int overwrite_config(void * config, char * name, char * val) {
	struct tmp_config * conf;
	size_t val_size;
	int found = 0;
	int overwrite_normal = 0;

	if (only_count() || isdeleted(name)) {
		return 0;
	}

	conf = (struct tmp_config *)config;
	val_size = strlen(val);

	write(conf->fd, name, strlen(name));

	if (!do_delete) {
		if (key) {
			if (strcmp(key, name) == 0) {
				found = 1;

			}

		} else if(expression) {
			if (strcmp(expression, val) == 0) {
				found = 1;
			
			}
		
		} 

	}
	
	if (found) {
		if (!pattern_count()) {
			execute_exit = 1;

			write(conf->fd, "=", sizeof(char));
			write(conf->fd, value, strlen(value));
		
		} else {
			overwrite_normal = 1;
		
		}
	
	} else {
		overwrite_normal = 1;
	
	}
	
	if (overwrite_normal) {
		if (val_size > 0) {
			write(conf->fd, "=", sizeof(char));
			write(conf->fd, val, strlen(val));
		
		}

	}

	write(conf->fd, "\n", sizeof(char));

	return errno;

}

void usage() {
	fprintf(stderr, "Usage: CLIConfig [OPTIONS] file[,file1,file2,...]\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "    -h, --help        Mostrar este mensaje\n");
	fprintf(stderr, "    -s, --show        Mostrar las claves junto con el valor\n");
	fprintf(stderr, "    -k, --key         Actuar sólo cuando haya una coincidencia con la clave\n");
	fprintf(stderr, "    -w, --write       Escribir la clave y el valor en el archivo. No se sobre-escribirá el archivo al contrario de \"--overwrite\"\n");
	fprintf(stderr, "    -v, --value       El valor de la clave a escribir\n");
	fprintf(stderr, "    -n, --no-banner   No mostrar el procesamiento de lo que se está haciendo (útil cuando se lee)\n");
	fprintf(stderr, "    -o, --overwrite   Sobre-escribe el valor de una clave. ADVERTENCIA: ¡También sobre-escribe el archivo!\n");
	fprintf(stderr, "    -E, --expression  Sólo actuar cuando haya una coincidencia con el valor\n");
	fprintf(stderr, "    -O, --only        Sólo actuar N veces\n");
	fprintf(stderr, "    -i, --id          Actuar sólo cuando la N vez sea igual al identificador\n");
	fprintf(stderr, "    -p, --pattern     Similar a \"--id\", pero actúa sólo con la N coincidencia\n");
	fprintf(stderr, "    -d, --delete      Borrar una coincidencia. Al igual que \"-o\" ¡TAMBIÉN SOBRE-ESCRIBE EL ARCHIVO!\n");

}
