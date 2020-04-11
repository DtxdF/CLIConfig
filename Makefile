CC=gcc
SHELL=/bin/sh
OBJS=conf_parser.o strip.o exists.o
TARGET=CLIConfig
CFLAGS=-Wall -Wextra --verbose -O2 -ggdb
RM_PROGRAM=rm
RM_FLAGS=-rf
DSTDIR=/usr/bin
DOCDST=/usr/share/man/man1
DOCNAME=CLIConfig.1

${TARGET}: ${OBJS}
	${CC} ${CFLAGS} -o ${TARGET} main.c ${OBJS}

conf_parser.o: strip.o
	${CC} ${CFLAGS} -c conf_parser.c

strip.o:
	${CC} ${CFLAGS} -c strip.c

exists.o:
	${CC} ${CFLAGS} -c exists.c

clean: ${TARGET}
	${RM_PROGRAM} ${RM_FLAGS} *.o

install: ${TARGET}
	./INSTALL

uninstall:
	${RM_PROGRAM} ${RM_FLAGS} ${DOCDST}/${DOCNAME}.gz
	${RM_PROGRAM} ${RM_FLAGS} ${DSTDIR}/${TARGET}

.PHONY: clean install uninstall
