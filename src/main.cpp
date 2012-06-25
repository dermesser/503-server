# include <iostream>
# include <fstream>
# include "inetserverstream.hpp"
# include "exception.hpp"
# include <thread>
# include <unistd.h>

int main(int argc, char** argv)
{
	using libsocket::inet_stream_server;

	libsocket::inet_stream* clsock;

	try {
		inet_stream_server srvsock("0.0.0.0","8081",IPv4);
	
		clsock = srvsock.accept();

		*clsock << "Hello World!\n";
		srvsock.destroy();

	} catch (libsocket::socket_exception exc)
	{
		std::cerr << exc.mesg;
	}
	
	return 0;
}
