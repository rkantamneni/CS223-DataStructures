CC=gcc
CFLAGS=-Wall -pedantic -g3

PROGRAMS=testOrderBook


all: $(PROGRAMS)
testOrderBook: testOrderBook.o orderBook.o
testOrderBook.o: testOrderBook.c orderBook.h orderBook.o
orderBook.o: orderBook.c orderBook.h

clean:
	$(RM) *.o $(PROGRAMS)

