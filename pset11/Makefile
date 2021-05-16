CC=gcc
CFLAGS=-Wall -pedantic -g3

PROGRAMS=testInserter


all: $(PROGRAMS)
testInserter: testInserter.o inserter.o
testInserter.o: testInserter.c inserter.h inserter.o
inserter.o: inserter.c inserter.h

clean:
	$(RM) *.o $(PROGRAMS)

