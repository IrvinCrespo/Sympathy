#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include <future>

#if	defined(_WIN32) || defined(_WIN64)
	#include <winsock2.h>
	#include "winsock2.h"
	#include <windows.h>
#elif defined(__unix__)
	#define _BSD_SOURCE
	#include <arpa/inet.h>
	#include <sys/types.h>
	#include <cstring>
    #include <unistd.h> 
	#include <sys/socket.h>
	#include <netinet/in.h> 
#endif

class Server
{	
	private:
		int sockid;
		#if defined(_WIN32) 
			WSADATA wsaData; 
		#endif
		struct sockaddr_in addport,client;
		void from_async();
		
	public:
		Server();
		~Server();
		void _config();
		void _bind();
		void _bindAsync();

	protected:
};



#endif
