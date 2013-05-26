.SUFFIXES: .cpp .c .o

CPLPL=g++
CC=gcc
CFLAGS=-g

OBJECTS=main.o

.cpp.o:
	$(CPLPL) $(CFLAGS) -o $@ -std=gnu++11 -Wall -c -DVERBOSE $<

.c.o:
	$(CC) $(CFLAGS) -o $@ -Wall -c -DVERBOSE $<

../build/503srv : subdirs $(OBJECTS)
	$(CPLPL) -std=gnu++11 -o ../build/503srv $(OBJECTS) socket/503socket.a -lpthread

subdirs:
	cd socket; \
	$(MAKE)

clean:
	rm -f $(OBJECTS); \
	cd socket; \
	$(MAKE) clean
