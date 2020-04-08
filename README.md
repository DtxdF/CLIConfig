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

CLIConfig ayudará a la persona que esté haciendo scripting y necesite interactuar con los archivos de configuración del sistema. Por lo que siguiendo las pautas que se mostrarán a continuación quedará más que claro:

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

**CLIConfig no le quita los espacios a las claves, por lo que en el caso de wvdial se tendría que colocar espacios.**

```bash
CLIConfig -s /etc/wvdial.conf -k "Username "
```

Resultado:

```
[Procesando: /etc/wvdial.conf]

Username = none

[Procesado: /etc/wvdial.conf]
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

| Parámetro       | Descripción                                                                 |
| --------------- | --------------------------------------------------------------------------- |
| -h/--help       | Muestra la ayuda                                                            |
| -s/--show       | Muestra el archivo de configuración parseado                                |
| -k/--key        | Coincidir con la clave                                                      |
| -w/--write      | Agregar una clave y un valor a la configuración                             |
| -v/--value      | El valor de la clave                                                        |
| -n/--no-banner  | No mostrar el procesamiento de los que se está haciendo                     |
| -o/--overwrite  | Sobre-escribe el valor de una(s) clave(s)                                   |
| -E/--expression | Usar un valor como coincidencia en vez de la clave                          |
| -I/--init       | Iniciar la interacción después de un número N                               |
| -O/--only       | Sólo actuar con N filas                                                     |
|    --offset     | Lo mismo que -O/--only                                                      |
| -i/--id         | Actuar solamente cuando la llamada del callback número X sea igual a Y      |
| -p/--pattern    | Cuándo haya N coincidencias encontradas actuar con la que ajustó el usuario |
| -d/--delete     | Borrar una clave                                                            |

**NOTA:** Es necesario comprender el funcionamiento de cada parámetro que usa, ya que no todos funcionan de la misma manera y no todos se pueden combinar, además de los valores por defecto que fueron otorgados a éstos por el diseño del mismo programa.

| Parámetro          | Combinaciones                                                                                        |
| ------------------ | ---------------------------------------------------------------------------------------------------- |
| -s/--show          | -k/--key, -E/--expression, -O/--only, -i/--id, -p/--pattern                                          |
| -k/--key           | -v/--value, -w/--write, -s/--show, -o/--overwrite, -O/--only, -i/--id, -p/--pattern, -d/--delete     |
| -w/--write         | -k/--key, -v/--value                                                                                 |
| -v/--value         | -k/--key, -w/--write, -o/--overwrite, -O/--only, -p/--pattern                                        |
| -o/--overwrite     | -k/--key, -v/--value, -E/--expression, -O/--only, -p/--pattern                                       |
| -E/--expression    | -s/--show, -v/--value, -o/--overwrite, -O/--only, -p/--pattern                                       |
| -I/--init          | -s/--show, -k/--key, -v/--value, -o/--overwrite, -E/--expression, -p/--pattern, -d/--delete          |
| -O/--only/--offset | -s/--show, -k/--key, -v/--value, -o/--overwrite, -E/--expression, -i/--id, -p/--pattern, -d/--delete |
| -i/--id            | -s/--show, -O/--only                                                                                 |
| -p/--pattern       | -s/--show, -k/--key, -v/--value, -o/--overwrite, -E/--expression, -O/--only, -d/--delete             |

**NOTA-#1:** El parámetro **-n**/**--no-banner** no fue colocado en la tabla porque se puede combinar con todos

**NOTA-#2:** Sí se define el parámetro **-k/--key** al mismo tiempo que **-E/--expression**, el segundo no se tomara en cuenta, pero si el primero.

**NOTA-#3:** El valor por defecto de los siguientes parámetros es "**0**", lo que quiere decir que no se hará nada al menos que se le coloque un número mayor: **-O/--only**, **-i/--id**, **-p/--pattern**.

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
