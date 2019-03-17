#include "Server.h"
#include <ctime>
#include <algorithm>

#define LPORT 7200

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
std::vector<std::future<void>> pending_futures;

struct ARGS {
	int i;
	Server *s;
	Client c;
};

void Server::config(){

	int MAX = 10000000;
	std::vector<Client> clis;
	unsigned int n = std::thread::hardware_concurrency();
    std::cout << n << " concurrent threads are supported.\n";

	clock_t begin = clock();
	for(int i = 0; i < MAX; i++){
		Client c = Client();
		c.connected = true;
		clis.push_back(c);
	}
	int x = 0;
	for(int i = 0; i < clis.size(); i++){
		clis[i].connected = false;
		x += 1;
	}

	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout<<x<<" iterated in "<<elapsed_secs<<" secs\n";

	/*
	* creating socket
	* starting winsock dll if OS is windows
	*/
	#if	defined(_WIN32) || defined(_WIN64)
	int iResult = WSAStartup( MAKEWORD(2,2), &wsaData);
	if( iResult != NO_ERROR )
    	printf("Error at WSAStartup\n");
	#endif
	
	if(this->TYPE == TCP)
		sockid = socket(PF_INET,SOCK_STREAM,0);
	else
		sockid = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);

	if(sockid < 0){
		#if	defined(_WIN32) || defined(_WIN64)
		std::cout<<WSAGetLastError();
		#endif
	}
	addport.sin_family = AF_INET;
	addport.sin_port = htons(LPORT);
	addport.sin_addr.s_addr = htonl(INADDR_ANY);
}


void Server::bindC(){
	int sizebuffer = 1500;
	char *buffer = new char[sizebuffer];
	char buffer_s[] = "OKAY";
	struct sockaddr_in clientAddr;

	//socket start to bind at this port
	int res = bind(sockid,(struct sockaddr *) &addport, sizeof(addport));
	
	#if	defined(_WIN32) || defined(_WIN64)
	int i = sizeof(clientAddr);
	#elif defined(__unix__)
	socklen_t i;
	#endif

	if(res < 0){
		_close_socket();
	}	
	listen(sockid,100);

	if(TYPE == TCP)
		_bindTCP(sockid);
	else
		_bindUDP(sockid);
	
	delete buffer;
	#if	defined(_WIN32) || defined(_WIN64)
	WSACleanup();
	#endif
}

void Server::accepter(){
	_bindTCP(sockid);
}


DWORD WINAPI ThreadProcINT(LPVOID lpParameter) {
	
	//int cid = *static_cast<int*>(lpParameter);
	ARGS *args = (ARGS *)lpParameter;
	Server *_serv = args->s;
	int _cid = args->i;
	//delete args;
	char *buffer = new char[1500];

    while(1){
		int r = recv(_cid,buffer,sizeof(buffer),0);
		if(r == 0){
			std::cout<<"client disconnected"<<_cid<<"\n";
			auto itr = std::remove_if(_serv->clients.begin(),
			_serv->clients.end(),
			[&](Client c){return args->c.sockid == _cid;});

			_serv->clients.erase(itr, _serv->clients.end());
			return 0;
		}
		if(r < 0){
			std::cout<<"client disconnected, there was an error"<<_cid<<"\n";
			auto itr = std::remove_if(_serv->clients.begin(),
			_serv->clients.end(),
			[&](Client c){return args->c.sockid == _cid;});

			_serv->clients.erase(itr, _serv->clients.end());
			return 0;
		}
		std::cout<<"FROM: "<<_cid<<"\nReceive: "<<buffer<<"\n";
		char resp[4] ="OK";
		_serv->_broadcast(buffer);
		memset(buffer,0,sizeof buffer);
		//send(cid,resp,sizeof(resp),0);
		//delete buffer;
        
    }
    return 0;
    //Client::receive(lpParameter);
}

void Server::_broadcast(char *buffer){
	int i = 0;
	int size = this->clients.size();
	for(i;i<size;i++){
		std::cout<<"broadcasting to: "<<clients[i].sockid<<"\n";
		send(this->clients[i].sockid,buffer,sizeof(buffer),0);
	}
	delete buffer;
}

DWORD WINAPI ThreadProcCLI(LPVOID lpParameter) {
	Client *cli = (Client *)lpParameter;
    while(1){
        char *buffer = new char[1500];
        int r = recv(cli->sockid,buffer,sizeof(buffer),0);
		
        std::cout<<"FROM: "<<cli->sockid<<"Receive: "<<buffer<<"\n";
        //char resp[4] ="OK";
        //send(sockid,resp,sizeof(resp),0);
        //delete buffer;
    }
    return 0;
    //Client::receive(lpParameter);
}


void Server::_bindTCP(int sockid){

	while(1){
		struct sockaddr_in clientAddr;
		int i = sizeof(clientAddr);
		int new_S = accept(sockid,(struct sockaddr *)&clientAddr, &i);

		// add client to list
		Client cli = Client(clientAddr,new_S);
		cli.connected = true;
		cli.index = clients.size();
		clients.push_back(cli);	

		std::cout<<"Client connected "<<new_S<<" , number of clients: "<<clients.size()<<"\n";
		
		/*
		* First create a thread for each accepted client
		* send server as arg to thread to handle the server
		* also, send the socket id to know data rec from.
		*/
		ARGS args = {
			new_S , 
			this ,
			cli
		};
		CreateThread(NULL,0,ThreadProcINT,&args,0,NULL);
	}
}


void Server::_bindUDP(int sockid){
	char *buffer = new char[1500];
	while(1){
		memset(buffer,0,sizeof buffer);
		std::cout<<"Waiting for messages...\n";
		struct sockaddr_in clientAddr;
		int i = sizeof(clientAddr);
		auto count = recvfrom(sockid,buffer,1500,0,(struct sockaddr *) &clientAddr, &i);

		#if	defined(_WIN32) || defined(_WIN64)
			if(count < 0){
				std::cout<<WSAGetLastError();
			}
		#endif
	}
}

void Server::_recv(int sock_cli){

	while(1){
		char *buffer = new char[1500];
		int r = recv(sock_cli,buffer,sizeof(buffer),0);
		std::cout<<"FROM: "<<"Receive: "<<buffer<<"\n";
		char resp[4] ="OK";
		send(sock_cli,resp,sizeof(resp),0);
		delete buffer;
	}
	
}


void Server::_close_socket(){

	#if	defined(_WIN32) || defined(_WIN64)
	closesocket(sockid);
	#elif defined(__unix__)
	close(sockid);
	#endif

}



