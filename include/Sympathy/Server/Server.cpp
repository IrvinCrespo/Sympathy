#include "Server.h"

//#include <opencv2/opencv.hpp>
//using namespace cv;

#if	defined(_WIN32) || defined(_WIN64)
#define OS WINDOWS
#elif defined(__unix__)
#define OS LINUX
#elif defined(__APPLE__) || defined(__MACH__)
#define OS APPLE
#endif

Server::Server()
{
	
}

Server::~Server()
{
	
}

void Server::_config(){
	std::cout<<"Configuring application...";
	//creating socket
	
	//starting winsock dll is so is windows
	#if	defined(_WIN32) || defined(_WIN64)
	int iResult = WSAStartup( MAKEWORD(2,2), &wsaData);
	if( iResult != NO_ERROR )
    	printf("Error at WSAStartup\n");
	#endif
    
	sockid = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(sockid < 0){
		#if	defined(_WIN32) || defined(_WIN64)
		std::cout<<WSAGetLastError();
		#endif
	}
	
	client.sin_family = AF_INET;
	client.sin_port = htons(0);
	client.sin_addr.s_addr = htonl(INADDR_ANY);
	
	addport.sin_family = AF_INET;
	addport.sin_port = htons(7200);
	addport.sin_addr.s_addr = htonl(INADDR_ANY);
	//_bindAsync();
}

void Server::_bindAsync(){
	std::cout<<"\nHOLOSSS";
	auto f = std::async(std::launch::async, &Server::_bind,this);
}

void Server::_bind(){
	
	int res = bind(sockid,(struct sockaddr *) &addport, sizeof(addport));
	int sizebuffer = 50;
	char *buffer = new char[sizebuffer];
	char buffer_s[] = "OKAY";
	
	//std::cout<<sizeof(buffer);
	struct sockaddr_in clientAddr;
	
	int i = sizeof(clientAddr);
	
	if(res < 0){
		std::cout<<"Bind error, closing";
		#if	defined(_WIN32) || defined(_WIN64)
		closesocket(sockid);
		#elif defined(__unix__)
		close(sockid);
		#endif
	}
	
	while(1){
		memset(buffer,0,sizeof buffer);
		std::cout<<"Binding\n";
		#if	defined(_WIN32) || defined(_WIN64)
		int count;
		#elif defined(__unix__)
		ssize_t count;
		#endif
		

		count = recvfrom(sockid,buffer,sizebuffer,0,(struct sockaddr *) &clientAddr, &i);
		#if	defined(_WIN32) || defined(_WIN64)
			if(count < 0){
				std::cout<<WSAGetLastError();
			}
		#endif
		
		std::cout<<count<<"-------------\n"<<"Size: "<<count<<"\n"<<"Message: "<<buffer<<"\n";
		std::cout<<"IP: "<<inet_ntoa(clientAddr.sin_addr)<<"\n"<<"-------------\n\n";
		sendto(sockid,buffer_s,sizeof(buffer_s),0,(struct sockaddr *)&clientAddr, i);
		
	}
	#if	defined(_WIN32) || defined(_WIN64)
	WSACleanup();
	#endif
}



