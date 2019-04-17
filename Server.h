#pragma once

#include "SFML/Network.hpp"

#include "Player.h"

#define NB_PLAYERS 4

class Server
{
public:
	Server();
	~Server();
	void start();
private:
	unsigned int _currentNbPlayers = 0;
	std::vector<Player*> _players;
	sf::UdpSocket _socket;
	void sendMap(sf::IpAddress destination);
	void handlePlayerPos(sf::Packet packet);
	void handleProjectile(sf::Packet packet);
	void broadcastPacket(sf::Packet packet, std::string nameException);
	void handlePlayerConnection(sf::Packet packet, sf::IpAddress player, unsigned int port);
	int getPlayerId(std::string playerIp);
	std::vector<std::vector<int>> _level;
};

