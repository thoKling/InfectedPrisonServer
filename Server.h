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
	unsigned int _tick = 0;
	unsigned int _nbrZombies = 0;
	unsigned int _zombiesSpawned = 0;
	unsigned int _currentWave = 1;
};

