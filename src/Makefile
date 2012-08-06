.SUFFIXES: .cpp .c .o

CPLPL=g++
CC=gcc

OBJECTS=main.o

.cpp.o:
	$(CPLPL) -o ../build/$@ -std=gnu++11 -Wall -c -DVERBOSE $<

.c.o:
	$(CC) -o ../build/$@ -Wall -c -DVERBOSE $<

all: subdirs $(OBJECTS)
	$(CPLPL) -std=gnu++11 -o ../build/503srv ../build/*.o -lpthread

subdirs:
	cd socket; make

clean:
	rm ../build/*
