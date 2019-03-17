#include "Client.h"

Client::Client(struct sockaddr_in client, int sid){
    clientaddr = client;
    id = sid;
    sockid = sid;
}

Client::Client(){

}

DWORD WINAPI Client::receive(LPVOID lpParam){
    std::cout<<"receiving in "<<sockid<<"\n";
    while(1){
        char *buffer = new char[1500];
        int r = recv(sockid,buffer,sizeof(buffer),0);
        std::cout<<"FROM: "<<sockid<<"Receive: "<<buffer<<"\n";
        char resp[4] ="OK";
        send(sockid,resp,sizeof(resp),0);
        delete buffer;
    }
    return 0;
}

