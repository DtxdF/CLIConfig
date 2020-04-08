# CLIConfig
> CLIConfig es una utilidad para la línea de comandos, pero la verdadera estrella de esta película es la librería que usa "**conf_parser.h**" qué es la encargada de leer los archivos de configuración.CONF mientras que la herramienta antes mencionada hace mucho más.

## Instalación

```bash
git clone https://github.com/DtxdF/CLIConfig.git
cd ./CLIConfig
sudo make install
CLIConfig -h
```

## Ejemplos de CLIConfig

CLIConfig ayudara a la persona que esté haciendo scripting y necesite interactuar con los archivos de configuración del sistema. Por lo que siguiendo las pautas que se mostrarán a continuación quedará más que claro:

### Leer la configuración

*En mi caso yo usaré el archivo de configuración de DNSMASQ, pero antes haré una copia para esta demostración*

```bash
cd /tmp
cp /etc/dnsmasq.conf .
CLIConfig dnsmasq.conf -s
```

CLIConfig no lee comentarios que tengan almuadillas antepuestas (**#**) por lo que en mi caso se mostraría lo siguiente:

```
[Procesando: dnsmasq.conf]

domain-needed
bogus-priv
no-resolv
log-queries
log-dhcp
log-facility=/var/log/dnsmasq.log
server=1.1.1.1
server=1.0.0.1
dhcp-range=eth0,192.168.0.2,192.168.0.254,12h
interface=eth0

[Procesado: dnsmasq.conf]
```

Sí quisieramos eliminar el procesamiento se tendría que colocar el parámetro "**-n**" ó "**--no-banner**":

```
domain-needed
bogus-priv
no-resolv
log-queries
log-dhcp
log-facility=/var/log/dnsmasq.log
server=1.1.1.1
server=1.0.0.1
dhcp-range=eth0,192.168.0.2,192.168.0.254,12h
interface=eth0

```

Sí quisieramos mostrar las claves "**server**" sería de la siguiente manera:

```bash
CLIConfig -k server -s dnsmasq.conf
```

El resultado sería el siguiente

```
[Procesando: dnsmasq.conf]

server=1.1.1.1
server=1.0.0.1

[Procesado: dnsmasq.conf]
```

Quiero recordar que todo lo que no sea un parámetro o un argumento de un parámetro es un considerado un archivo

```bash
cp /etc/wvdial.conf .
CLIConfig -s dnsmasq.conf wvdial.conf
```

Resultado:

```
[Procesando: dnsmasq.conf]

domain-needed
bogus-priv
no-resolv
log-queries
log-dhcp
log-facility=/var/log/dnsmasq.log
server=1.1.1.1
server=1.0.0.1
dhcp-range=eth0,192.168.0.2,192.168.0.254,12h
interface=eth0

[Procesado: dnsmasq.conf]
[Procesando: wvdial.conf]

[Dialer Defaults]
Init1 = ATZ
Init2 = ATQ0 V1 E1 S0=0 &C1 &D2 +FCLASS=0
Modem Type = Analog Modem
Baud = 9600
New PPPD = yes
Modem = /dev/ttyUSB2
ISDN = 0
Phone = *99
Password = none
Username = none

[Procesado: wvdial.conf]
```

Para modificar los servidores DNS de dnsmasq.conf haríamos los siguiente:

```bash
CLIConfig dnsmasq.conf -k server -v 8.8.8.8 -o
```

```
[Procesando: dnsmasq.conf]

¡server ha sido modificado con éxito!

[Procesado: dnsmasq.conf]
```

Si vemos el archivo modificado:

```bash
cat dnsmasq.conf
```

```
domain-needed
bogus-priv
no-resolv
log-queries
log-dhcp
log-facility=/var/log/dnsmasq.log
server=8.8.8.8
server=8.8.8.8
dhcp-range=eth0,192.168.0.2,192.168.0.254,12h
interface=eth0
```

No se ve tan bien... Y es por qué no le especificamos qué coincidencia deseamos modificar, por lo que lo hará con todos los que encuentre, para realizarlo:

```bash
CLIConfig dnsmasq.conf -k server -v 8.8.4.4 -o -p 2
...
cat dnsmasq.conf
```

```
...
server=8.8.8.8
server=8.8.4.4
...
```

**NOTA**: El parámetro "**-o**" ó "**--overwrite**" sobre-escribe el valor de la clave y el archivo, por lo que es mejor que hagamos un respaldo del anterior en casos reales

Para agregar una nueva clave:

```bash
CLIConfig dnsmasq.conf -k dhcp-range -v wlan0,10.42.0.2,10.42.0.254,5h -w
```

```
[Procesando: dnsmasq.conf]

Escrito: dhcp-range=wlan0,10.42.0.2,10.42.0.254,5h

[Procesado: dnsmasq.conf]
```

Para eliminar todas las claves coincidentes:

```bash
CLIConfig dnsmasq.conf -d -k server
```

Eliminar sólo la segunda coincidencia:

```bash
CLIConfig dnsmasq.conf -d -k server -p 2
```

Ver la ayuda:

```bash
CLIConfig -h
```

## Funcionamiento de cada parámetro



## Sintaxis de los archivos de configuración

La sintaxis de un archivo de configuración se rige de la siguiente manera:

```
clave=valor
```

## Ejemplo de la librería

Primero compilamos la librería:

```bash
make conf_parser.o
```

Teniendo el siguiente archivo de configuración:

```bash
cat test.conf
```

```
name=Josef
lastname=Naranjo
age=17
alias=DtxdF
```

Y teniendo el siguiente *parser* (**test.c**):

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "conf_parser.h"

#define CMP(x,y) strcmp(x, y) == 0

struct configuration {
	char * name;
	char * lastname;
	long age;
	char * alias;

};

int parser(void * config, char * key, char * value) {
	struct configuration * conf = (struct configuration *)config;
	char *endstr;

	if (CMP(key, "name")) {
		conf->name = strdup(value);
	
	} else if (CMP(key, "lastname")) {
		conf->lastname = strdup(value);
	
	} else if (CMP(key, "age")) {
		conf->age = strtol(value, &endstr, 10);

		if (errno != 0) {
			return errno;
		
		}
	
	} else if (CMP(key, "alias")) {
		conf->alias = strdup(value);
	
	} else {
		return 3; // Cuando una clave no es válida (Opcional)
	
	}

	return 0;

}

int main(void) {
	struct configuration config;
	FILE * file;

	if ((file = fopen("test.conf", "rb")) == NULL) {
		perror("Error abriendo el archivo de configuración");
	
	}

	if (ini_parse(file, &config, parser) != 0) {
		perror("Ocurrio un error interpretando el archivo de configuración");

		return errno;
	
	}

	printf("Información del sujeto:\n");
	printf("----------------------\n\n");

	printf("Nombre: %s\n", config.name);
	printf("Apellido: %s\n", config.lastname);
	printf("Edad: %ld\n", config.age);
	printf("Alias: %s\n", config.alias);

	free(config.name);
	free(config.lastname);
	free(config.alias);

	return EXIT_SUCCESS;

}
```

Lo compilamos y ejecutamos:

```bash
gcc -Wall -O2 -o program test.c conf_parser.o strip.o
./program
```

Y obtenemos:

```
Información del sujeto:
----------------------

Nombre: Josef
Apellido: Naranjo
Edad: 17
Alias: DtxdF
```

## TODO

* Auto-completado de comandos
* Manpage
* Verificador de sintaxis

\~ DtxdF
