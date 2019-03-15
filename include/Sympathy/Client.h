#ifndef CLIENT_H
#define CLIENT_H

#if	defined(_WIN32) || defined(_WIN64)

    #include <winsock2.h>
    #include "winsock2.h"
    #include <windows.h>

#elif defined(__unix__)
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
    Client(/* args */);
    ~Client();
};



#endif
