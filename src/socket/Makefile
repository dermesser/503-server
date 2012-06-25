.SUFFIXES: .cpp .o .c

.cpp.o:
	g++ -Wall -c -DVERBOSE $<

.c.o:
	gcc -Wall -c -DVERBOSE $<

MODS=exception.o  inetbase.o  inetclientstream.o  inetserverstream.o  socket.o  streamclient.o libinetsocket.o

all: $(MODS)
