CC=gcc
CFLAGS=-Wall -pedantic -g3

PROGRAMS=decrypt


all: $(PROGRAMS)
decrypt: decrypt.o
decrypt.o: decrypt.c decrypt.h


clean:
	$(RM) *.o $(PROGRAMS)
