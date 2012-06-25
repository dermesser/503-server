# include <iostream>
# include <fstream>
# include "inetserverstream.hpp"
# include "exception.hpp"
# include <thread>
# include <unistd.h>
# include <string>
# include <stdlib.h>

using libsocket::inet_stream_server;
using libsocket::inet_stream;
using libsocket::socket_exception;

std::string head =
"HTTP/1.1 503 Service Unavailable\n"
"Server: 503srv\n"
"Connection: close\n"
"Content-Length: 187\n\n"; // FIXME: Adapt this if you change the error message

std::string body =
"<html>\n<head><title>503 Service Unavailable</title></head>\n<body>\n<h1>503 Service Temporarily Unavailable</h1>\nDue to a downtime, this service is temporarily unavailable.\n</body>\n</html>\n";

void process_connection(inet_stream* clsock)
{
	try {
		*clsock << head << body;

		std::this_thread::sleep_for(std::chrono::milliseconds(150)); // Necessary to transmit the full message
		clsock->destroy();
	} catch (socket_exception exc)
	{
		std::cerr << exc.mesg;
	}
}

void accept_new_connections(inet_stream_server& srvsock)
{
	inet_stream* clsock;

	while ( 1 )
	{
		try {
			clsock = srvsock.accept();

			std::thread worker(process_connection,clsock);
			worker.detach();

		} catch (socket_exception exc)
		{
			std::cerr << exc.mesg;
		}
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
