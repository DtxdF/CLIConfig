/* conf_parser - Parsear archivos de configuración .CONF típicamente de *NIX
   
   Autor: DtxdF
   Repositorio: https://github.com/DtxdF/CLIConfig
   Documentación: ./README.md
   Licencia: GNU General Public License

   */

#ifndef _CONF_PARSER_H
#define _CONF_PARSER_H

// El nombre del callback

typedef int(*conf_parser_func)(void *, char *, char *);

// La función que parsea el archivo de configuración

int ini_parse(FILE * file, void * config,
			  conf_parser_func func);

#endif
