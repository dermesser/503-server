# include <iostream>
# include <fstream>
# include "socket/inetserverstream.hpp"
# include "socket/exception.hpp"
# include <thread>
# include <mutex>
# include <unistd.h>
# include <string>
# include <stdlib.h>
# include <time.h>
# include <sstream>

using libsocket::inet_stream_server;
using libsocket::inet_stream;
using libsocket::socket_exception;
using std::string;

static std::mutex logfile_mutex;
static string http_output;

void setup_httpdata(void)
{
    string header =
"HTTP/1.1 503 Service Unavailable\x0d\x0a"
"Server: 503srv\x0d\x0a"
"Connection: close\x0d\x0a"
"Content-Length: ";

    string body =
"<html>\n"
"    <head>\n"
"	<title>503 Service Unavailable</title>\n"
"    </head>\n"
"   <body>\n"
"	<h1>503 Service Temporarily Unavailable</h1>\n"
"	    This HTTP service is unfortunately temporarily unavailable :(\n"
"	    Maybe you want to come back later...\n"
"    </body>\n"
"</html>\n";

    std::ostringstream full_body_stream;

    full_body_stream << header << body.length() << "\x0d\x0a\x0d\x0a" << body;

    http_output = full_body_stream.str();

}

void process_connection(inet_stream* clsock,std::ofstream* logfile)
{
	time_t epoch_time;
	char* timebuf = new char[26];

	try {
		{
			epoch_time = time(0);

			ctime_r(&epoch_time,timebuf);

			timebuf[24] = 0;

			std::unique_lock<std::mutex> log_out(logfile_mutex);

			*logfile << timebuf << " Client: " << clsock->gethost().c_str() << ":" << clsock->getport().c_str() << "\n";
			logfile->flush();
		}

		*clsock << http_output;

		std::this_thread::sleep_for(std::chrono::milliseconds(300)); // apparently necessary for correct delivery

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
    setup_httpdata();

	try {
		inet_stream_server srvsock("::",argv[1],LIBSOCKET_IPv6); // If we bind to v6 ::, we get IPv4 connections too on most Linux systems (net.ipv6.bindv6only)

		daemon(1,0);

		accept_new_connections(srvsock);

		srvsock.destroy();

	} catch (socket_exception exc)
	{
		std::cerr << exc.mesg;
	}

	return 0;
}
