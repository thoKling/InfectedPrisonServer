#pragma once

#include <string>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

class Player : public sf::Transformable
{
public:
	Player(sf::IpAddress addr, std::string name, unsigned int port);
	std::string name;
	sf::IpAddress addr;
	unsigned int port;
};

