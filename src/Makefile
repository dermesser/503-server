.SUFFIXES: .cpp .c .o

OBJECTS=main.o

.cpp.o:
	g++ -std=gnu++11 -Wall -c -DVERBOSE $<

.c.o:
	gcc -Wall -c -DVERBOSE $<

all: subdirs $(OBJECTS)
	g++ -std=gnu++11 -o ../503srv $(OBJECTS) socket/*.o -lpthread

subdirs:
	cd socket; make

clean:
	rm */*.o
