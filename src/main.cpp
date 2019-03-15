#include <iostream>
#include <future>
#include "Server/Server.h"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
using namespace std;

int main() {
	
	Server serv;
	serv._config();
	future<void> f = async(std::launch::async,&Server::_bind,&serv);
	//std::vector<int> vec;
	
	getchar();
	system("pause");
	return 0;
}
