.SUFFIXES: .cpp .o .c

CPLPL=g++
CC=gcc
CFLAGS=-D_MIXED

.cpp.o:
	$(CPLPL) $(CFLAGS) -o $@ -std=gnu++11 -Wall -c -DVERBOSE $<

.c.o:
	$(CC) -o $@ -Wall -c -DVERBOSE $<

MODS=exception.o  inetbase.o  inetclientstream.o  inetserverstream.o  socket.o  streamclient.o libinetsocket.o

all : 503socket.a

503socket.a : $(MODS)
	ar rs 503socket.a $(MODS)

clean :
	rm -f *.o
