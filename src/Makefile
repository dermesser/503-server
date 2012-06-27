.SUFFIXES: .cpp .c .o

CPLPL=g++
CC=gcc

OBJECTS=main.o

.cpp.o:
	$(CPLPL) -std=gnu++11 -Wall -c -DVERBOSE $<

.c.o:
	$(CC) -Wall -c -DVERBOSE $<

all: subdirs $(OBJECTS)
	$(CPLPL) -std=gnu++11 -o ../503srv $(OBJECTS) socket/*.o -lpthread

subdirs:
	cd socket; make

clean:
	rm *.o */*.o
