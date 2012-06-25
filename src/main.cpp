# include <iostream>
# include <fstream>
# include "inetserverstream.hpp"
# include "exception.hpp"
# include <thread>
# include <unistd.h>

using libsocket::inet_stream_server;
using libsocket::inet_stream;
using libsocket::socket_exception;

void process_connection(inet_stream* clsock)
{
	try {
		*clsock << "Hello World!\n";
	} catch (socket_exception exc)
	{
		std::cerr << exc.mesg;
	}
}

void accept_new_connections(inet_stream_server& srvsock)
{
	inet_stream* clsock;

	try {
		clsock = srvsock.accept();

		std::thread worker(process_connection,clsock);
		worker.join();

	} catch (socket_exception exc)
	{
		std::cerr << exc.mesg;
	}
}

int main(int argc, char** argv)
{
	try {
		inet_stream_server srvsock("0.0.0.0",argv[1],IPv4);
	
		accept_new_connections(srvsock);

		srvsock.destroy();

	} catch (socket_exception exc)
	{
		std::cerr << exc.mesg;
	}
	
	return 0;
}
