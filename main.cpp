#include "Server.h"

#include <iostream>

int main()
{
	Server server;
	server.start();

	// pour �viter que la console ne se ferme
	int end;
	std::cout << "finito";
	std::cin >> end;
	return 0;
}

