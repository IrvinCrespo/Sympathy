#include "Server.h"

#if	defined(_WIN32) || defined(_WIN64)
#define OS WINDOWS
#elif defined(__unix__)
#define OS LINUX
#elif defined(__APPLE__) || defined(__MACH__)
#define OS APPLE
#endif

Server::Server(SOCKTYPE TYPE)
{
	this->TYPE = TYPE;
}

Server::~Server()
{
	this->TYPE = TCP;
}

void Server::_configureTCP(){

}

void Server::_configureUDP(){
	
}

void Server::config(){
	if(this->TYPE == TCP)
		std::cout<<"TCP sockets...\n";
	else
		std::cout<<"UDP\n";

	std::cout<<"Configuring application...\n";
	
	//creating socket
	//starting winsock dll if OS is windows
	#if	defined(_WIN32) || defined(_WIN64)
	int iResult = WSAStartup( MAKEWORD(2,2), &wsaData);
	if( iResult != NO_ERROR )
    	printf("Error at WSAStartup\n");
	#endif
    if(this->TYPE == TCP)
		sockid = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
	else
		sockid = socket(PF_INET,SOCK_STREAM,IPPROTO_IPV4);

	if(sockid < 0){
		#if	defined(_WIN32) || defined(_WIN64)
		std::cout<<WSAGetLastError();
		#endif
	}
	
	addport.sin_family = AF_INET;
	addport.sin_port = htons(7200);
	addport.sin_addr.s_addr = htonl(INADDR_ANY);
	
}



void Server::bindAsync(){
	std::cout<<"\nHOLOSSS";
	auto f = std::async(std::launch::async, &Server::bindC,this);
}

void Server::bindC(){
	
	int res = bind(sockid,(struct sockaddr *) &addport, sizeof(addport));
	int sizebuffer = 1500;
	char *buffer = new char[sizebuffer];
	char buffer_s[] = "OKAY";
	
	struct sockaddr_in clientAddr;
	
	#if	defined(_WIN32) || defined(_WIN64)
	int i = sizeof(clientAddr);
	#elif defined(__unix__)
	socklen_t i;
	#endif


	
	if(res < 0){
		_close_socket();
	}
	while(1){
		memset(buffer,0,sizeof buffer);
		std::cout<<"Binding\n";
		auto count = recvfrom(sockid,buffer,sizebuffer,0,(struct sockaddr *) &clientAddr, &i);
		
		#if	defined(_WIN32) || defined(_WIN64)
			if(count < 0){
				std::cout<<WSAGetLastError();
			}
		#endif
		
		std::cout<<count<<"-------------\n"<<"Size: "<<count<<"\n"<<"Message: "<<buffer<<"\n";
		std::cout<<"IP: "<<inet_ntoa(clientAddr.sin_addr)<<"\n"<<"-------------\n\n";
		sendto(sockid,buffer_s,sizeof(buffer_s),0,(struct sockaddr *)&clientAddr, i);
		
	}
	delete buffer;
	#if	defined(_WIN32) || defined(_WIN64)
	WSACleanup();
	#endif
}

template<int, typename S,char[], typename SS>
void Server::_bindTCP(int sockid,S i, char buffer[], SS clientAddr){
	int res = listen(sockid,2);
	
	int new_S = accept(sockid,(struct sockaddr *)&SS, S);
	clients.insert(new_S);
	
}

template<int, typename S, char[], typename SS>
void Server::_bindUDP(int sockid, S, char buffer[], SS clientAddr){
	
}



void Server::_close_socket(){

	#if	defined(_WIN32) || defined(_WIN64)
	closesocket(sockid);
	#elif defined(__unix__)
	close(sockid);
	#endif

}



