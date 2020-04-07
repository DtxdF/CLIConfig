/*	strlen_m.c
 *
 *	Cuenta hasta el carácter nulo para saber la longitud de un string y le
 *	suma uno (util para algunos casos en que se desee sacar la longitud
 *	más el carácter nulo).
 *
*/

#include <string.h>

size_t strlen_m(const char * string) {
	return strlen(string)+1;

}
