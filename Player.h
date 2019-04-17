#pragma once

#include <string>
#include <SFML/Network.hpp>

class Player
{
public:
	Player(sf::IpAddress addr, std::string name, unsigned int port);
	std::string name;
	sf::IpAddress addr;
	unsigned int port;
};

