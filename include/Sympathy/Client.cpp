#include "Client.h"

Client::Client(struct sockaddr_in *clientAddr){
    this->clientaddr = clientAddr;
}