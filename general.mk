include config.mk

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

conf_parser.o: strip.h strip.o conf_parser.h \
			   getline_m.o getline_m.h

main.o: conf_parser.h conf_parser.o main.h \
		exists.h exists.o getline_m.h \
		getline_m.o

clean: cleanobj

cleanall: cleanobj
	-$(RM) $(RMFLAGS) $(TARGET)

cleanobj:
	-$(RM) $(RMFLAGS) $(OBJECTS)

.PHONY: all clean cleanobj
.SILENT: cleanobj cleanall
