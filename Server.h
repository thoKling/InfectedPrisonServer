#pragma once

#include "SFML/Network.hpp"

#define MS_PER_UPDATE 1000/60

class Server
{
public:
	Server();
	~Server();
	void start();
private:
	bool _done = false;
	void update();
};

