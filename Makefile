CC=gcc
SHELL=/bin/sh
OBJS=conf_parser.o strip.o getline_m.o exists.o
TARGET=CLIConfig
CFLAGS=-Wall -Wextra --verbose -O2 -ggdb
INSTALL_PROGRAM=install
INSTALL_FLAGS=-v -b -C
RM_PROGRAM=rm
RM_FLAGS=-rf
DSTDIR=/usr/bin

${TARGET}: ${OBJS}
	${CC} ${CFLAGS} -o ${TARGET} main.c ${OBJS}

conf_parser.o:
	${CC} ${CFLAGS} -c conf_parser.c

strip.o:
	${CC} ${CFLAGS} -c strip.c

getline_m.o:
	${CC} ${CFLAGS} -c getline_m.c

exists.o:
	${CC} ${CFLAGS} -c exists.c

clean: ${TARGET}
	${RM_PROGRAM} ${RM_FLAGS} *.o

install: ${TARGET}
	${INSTALL_PROGRAM} ${INSTALL_FLAGS} ${TARGET} ${DSTDIR}

uninstall:
	${RM_PROGRAM} ${RM_FLAGS} ${DSTDIR}/${TARGET}

.PHONY: clean install uninstall
