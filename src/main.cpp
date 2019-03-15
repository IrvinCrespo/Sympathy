#include <iostream>
#include <future>
#include "Server/Server.h"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
using namespace std;

int main() {
	
	Server serv(TCP);
	serv._config();

	future<void> f = async(std::launch::async,&Server::bindC,&serv);
	
	getchar();
	system("pause");
	return 0;
}
