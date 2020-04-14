TARGET = CLIConfig
OBJECTS = main.o exists.o conf_parser.o \
		  strip.o
CC = gcc
CFLAGS =-Wall -Wextra -O2 -ggdb -pipe
RM = rm
RMFLAGS = -v -f
prefix = /usr
exec_prefix = $(prefix)
mandir = $(prefix)/share/man
man1dir = $(mandir)/man1
srcdir = .
bindir = $(exec_prefix)/bin
INSTALL = install
INSTALL_PROGRAM = $(INSTALL) -m 0751 -v -C
INSTALL_DATA = $(INSTALL) -m 0644 -v -C

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $^

conf_parser.o: strip.h conf_parser.h strip.o

main.o: conf_parser.h main.h exists.h \
		conf_parser.o exists.o

clean: cleanobj

cleanall: cleanobj
	-$(RM) $(RMFLAGS) $(TARGET)

cleanobj:
	-$(RM) $(RMFLAGS) $(OBJECTS)

install: all
	$(INSTALL) -v -d "$(prefix)"
	$(INSTALL) -v -d "$(exec_prefix)"
	$(INSTALL) -v -d "$(bindir)"
	$(INSTALL) -v -d "$(mandir)"
	$(INSTALL) -v -d "$(man1dir)"
	
	-$(INSTALL_PROGRAM) "$(srcdir)/$(TARGET)" "$(bindir)"

	if ! [ -f "$(srcdir)/doc/CLIConfig.1.gz" ] ; \
	then \
		gzip "$(srcdir)/doc/CLIConfig.1" ; \
	fi;

	-$(INSTALL_DATA) "$(srcdir)/doc/CLIConfig.1.gz" "$(man1dir)"

uninstall:
	-$(RM) $(RMFLAGS) "$(bindir)/$(TARGET)"
	-$(RM) $(RMFLAGS) "$(man1dir)/CLIConfig.1.gz"

.SILENT: clean cleanobj cleanall
.PHONY: clean cleanall cleanobj all install uninstall
