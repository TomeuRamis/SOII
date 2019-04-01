CC=gcc
CFLAGS=-c -g -Wall -std=gnu99
#LDFLAGS=-pthread

SOURCES=ficheros.c ficheros_basico.c bloques.c mi_mkfs.c leer_sf.c escribir.c leer.c#todos los .c
LIBRARIES=ficheros_basico.o bloques.o  ficheros.o #todos los .o de la biblioteca del SF
INCLUDES=ficheros_basico.h bloques.h ficheros.h #todos los .h
PROGRAMS=mi_mkfs leer_sf escribir leer
OBJS=$(SOURCES:.c=.o)

all: $(OBJS) $(PROGRAMS)

$(PROGRAMS): $(LIBRARIES) $(INCLUDES)
	$(CC) $(LDFLAGS) $(LIBRARIES) $@.o -o $@

%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	rm -rf *.o *~ $(PROGRAMS)