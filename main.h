#ifndef _MAIN_H_
#define _MAIN_H_

char * key = NULL;
char * value = NULL;
char * expression = NULL;

int do_delete = 0;
int execute_exit = 0; // Para informar cuando haya o no haya una coincidencia

// Opciones

/* El identificador (identifier) es parecido a la coincidencia (patterns), pero la diferencia está en que éste es usado para mostrarse cuándo un número N sea igual al número de llamado Y. Un ejemplo que explique mucho mejor ésto es el siguiente:
  int N = 3;
  for (int Y = 0; Y < 10; Y++) {
      if (N == Y) {
          // Hacer algo
		  
      }
  
  }

  Cómo se puede observar, hasta que N no sea igual a Y no se hará nada. Pero si hablamos de la coincidencia (patterns):

  int X, Y;

  X = 0;
  Y = 4;

  while (1) {
      if (strcmp("str", "str") == 0) {
	      X++;
	  
	  }

	  if (X == Y) {
	      // Hacer algo
		  break; // Por ejemplo
	  
	  }
  
  }

  X vendría siendo las veces que fue encontrada esa coincidencia, mientras que Y el limite de las veces.


  Ahora si hablamos de (only) es más fácil y no se necesita código para explicarlo. Simplemente es el limite que debe actuar el callback. Por ejemplo: Mostrar sólo 10 claves con sus valores (si es que los tiene).

*/

long identifier = 0; 
long only = 0;
long patterns = 0;
long init = 0;

// Auxiliares

long id_aux = 0;
long only_aux = 0;
long pattern_aux = 0;
long init_aux = 0;

struct tmp_config {
	int fd;

};

#endif
