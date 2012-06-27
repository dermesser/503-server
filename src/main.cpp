# include <iostream>
# include <fstream>
# include "inetserverstream.hpp"
# include "exception.hpp"
# include <thread>
# include <unistd.h>
# include <string>
# include <stdlib.h>
# include <time.h>

using libsocket::inet_stream_server;
using libsocket::inet_stream;
using libsocket::socket_exception;

std::string head =
"HTTP/1.1 503 Service Unavailable\x0d\x0a"
"Server: 503srv\x0d\x0a"
"Connection: close\x0d\x0a"
"Content-Length: 187\x0d\x0a\x0d\x0a"; // FIXME: Adapt this if you change the error message

std::string body =
"<html>\n<head><title>503 Service Unavailable</title></head>\n<body>\n<h1>503 Service Temporarily Unavailable</h1>\nDue to a downtime, this service is temporarily unavailable.\n</body>\n</html>\n";

void process_connection(inet_stream* clsock,std::ofstream* logfile)
{
	time_t epoch_time;
	char* timebuf = new char[26];

	try {
		{
			epoch_time = time(0);

			ctime_r(&epoch_time,timebuf);

			timebuf[24] = 0;

			*logfile << timebuf << " Client: " << clsock->gethost().c_str() << ":" << clsock->getport().c_str() << "\n";

			logfile->flush();
		}

		*clsock << head << body;

		std::this_thread::sleep_for(std::chrono::milliseconds(150)); // Necessary to transmit the full message

		delete clsock;
	} catch (socket_exception exc)
	{
		std::cerr << exc.mesg;
		delete[] timebuf;
	}

	delete[] timebuf;
}

void accept_new_connections(inet_stream_server& srvsock)
{
	inet_stream* clsock;

	std::ofstream logfile("503srv.log");

	while ( 1 )
	{
		try {
			clsock = srvsock.accept();

			std::thread worker(process_connection,clsock,&logfile);
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

		daemon(1,0);

		accept_new_connections(srvsock);

		srvsock.destroy();

	} catch (socket_exception exc)
	{
		std::cerr << exc.mesg;
	}

	return 0;
}
