#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include <future>
#include "../Client.h"

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
enum SOCKTYPE {TCP, UDP};
class Server
{	
	private:
		int sockid;
		#if defined(_WIN32) 
			WSADATA wsaData; 
		#endif
		struct sockaddr_in addport,client;
		void _configureTCP();
		void _configureUDP();
		void _close_socket();

		template<int, typename S, char[], typename SS>
		void _bindTCP(int,S,char[],SS);

		template<int, typename S, char[], typename SS>
		void _bindUDP(int,S,char[],SS);

		
		
	public:
		
		Server();
		Server(SOCKTYPE TYPE);
		~Server();
		void config();
		void bindC();
		void bindAsync();
		
		SOCKTYPE TYPE;
		std::vector<Client> clients;

	protected:
};



#endif
