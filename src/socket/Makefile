.SUFFIXES: .cpp .o .c

CPLPL=g++
CC=gcc

.cpp.o:
	$(CPLPL) -std=gnu++11 -Wall -c -DVERBOSE $<

.c.o:
	$(CC) -Wall -c -DVERBOSE $<

MODS=exception.o  inetbase.o  inetclientstream.o  inetserverstream.o  socket.o  streamclient.o libinetsocket.o

all: $(MODS)
