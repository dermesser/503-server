# include <string.h>
# include <string>

/*
The committers of the libsocket project, all rights reserved
(c) 2012, dermesser <lbo@spheniscida.de>

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
following conditions are met:

	1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
	disclaimer.
	2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
	disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

*/

/*
 * DESCRIPTION FOR INETSERVERSTREAM.CPP
 * 	inet_stream_server provides the TCP server part of libsocket.
 *	It's main function is accept() which returns a pointer to
 *	a dynamically allocated inet_stream (client socket) class which
 *	provides the connection to the client. You may setup the socket
 *	either with the second constructor or with setup()
 */

# include "libinetsocket.h"
# include "inetclientstream.hpp"
# include "exception.hpp"
# include "inetserverstream.hpp"

namespace libsocket
{
	using std::string;

	inet_stream_server::inet_stream_server(void)
		: nonblock(false)
	{
	}

	inet_stream_server::inet_stream_server(const char* bindhost, const char* bindport, int proto_osi3, int flags)
		: nonblock(false)
	{
		setup(bindhost,bindport,proto_osi3,flags);
	}

	inet_stream_server::inet_stream_server(const string& bindhost, const string& bindport, int proto_osi3, int flags)
		: nonblock(false)
	{
		setup(bindhost,bindport,proto_osi3,flags);
	}

	void inet_stream_server::setup(const char* bindhost, const char* bindport, int proto_osi3, int flags)
	{
		if ( sfd != -1 )
			throw socket_exception(__FILE__,__LINE__,"inet_stream_server::inet_stream_server() - already bound and listening!\n");
		if ( bindhost == 0 || bindport == 0 )
			throw socket_exception(__FILE__,__LINE__,"inet_stream_server::inet_stream_server() - at least one bind argument invalid!\n");
		if ( -1 == (sfd = create_inet_server_socket(bindhost,bindport,LIBSOCKET_TCP,proto_osi3,flags)) )
			throw socket_exception(__FILE__,__LINE__,"inet_stream_server::inet_stream_server() - could not create server socket!\n");

		host = string(bindhost);
		port = string(bindport);

		nonblock = false;
# ifdef __linux__
		if (flags & SOCK_NONBLOCK)
			nonblock = true;
# endif
	}

	void inet_stream_server::setup(const string& bindhost, const string& bindport, int proto_osi3, int flags)
	{
		if ( sfd != -1 )
			throw socket_exception(__FILE__,__LINE__,"inet_stream_server::inet_stream_server() - already bound and listening!\n");
		if ( bindhost.empty() || bindport.empty() )
			throw socket_exception(__FILE__,__LINE__,"inet_stream_server::inet_stream_server() - at least one bind argument invalid!\n");
		if ( -1 == (sfd = create_inet_server_socket(bindhost.c_str(),bindport.c_str(),LIBSOCKET_TCP,proto_osi3,flags)) )
			throw socket_exception(__FILE__,__LINE__,"inet_stream_server::inet_stream_server() - could not create server socket!\n");

		host = string(bindhost);
		port = string(bindport);

		nonblock = false;
# ifdef __linux__
		if (flags & SOCK_NONBLOCK)
			nonblock = true;
# endif
	}

	inet_stream* inet_stream_server::accept(int numeric,int accept_flags)
	{
		if ( sfd < 0 )
			throw socket_exception(__FILE__,__LINE__,"inet_stream_server::accept() - stream server socket is not in listening state -- please call first setup()!\n");

		char* src_host = new char[1024];
		char* src_port = new char[32];

		memset(src_host,0,1024);
		memset(src_port,0,32);

		int client_sfd;
		inet_stream* client = new inet_stream;

		if ( -1 == (client_sfd = accept_inet_stream_socket(sfd,src_host,1023,src_port,31,numeric,accept_flags)) )
		{
			if ( nonblock == false )
			{
				throw socket_exception(__FILE__,__LINE__,"inet_stream_server::accept() - could not accept new connection on stream server socket!\n");
			} else
			{
				return NULL; // Only return NULL but don't throw an exception if the socket is nonblocking
			}
		}

		client->sfd = client_sfd;
		client->host = string(src_host);
		client->port = string(src_port);
		client->proto = proto;

		return client;
	}

}
