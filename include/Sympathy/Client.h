#ifndef CLIENT_H
#define CLIENT_H
#include <iostream>
#include <vector>
#include <future>

#if	defined(_WIN32) || defined(_WIN64)
	#include <winsock2.h>
	#include "winsock2.h"
	#include <windows.h>
    #include <strsafe.h>
	#include <cstring>
#elif defined(__unix__)
	#define _BSD_SOURCE
	#include <arpa/inet.h>
	#include <sys/types.h>
	#include <cstring>
    #include <unistd.h> 
	#include <sys/socket.h>
	#include <netinet/in.h> 
#endif

class Client
{
private:
    struct sockaddr_in clientaddr;
    int id;
    /* data */
public:
    bool connected = false;
    int sockid;
    int data_thread;
    int index;
    //std::future<void> fut;
    DWORD WINAPI receive(LPVOID);
    Client(/* args */);
    Client(struct sockaddr_in , int id);
};



#endif
