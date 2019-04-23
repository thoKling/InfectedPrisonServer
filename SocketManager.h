#pragma once

#include <SFML/Network.hpp>

#include "Player.h"
#include "Map.h"
#include "Item.h"

#define NB_PLAYERS 4

class SocketManager
{
public:
	enum PacketType {
		Unknown,
		Connection,
		PlayerPos,
		Projectile,
		CreateZombie,
		ZombieState,
		ZombieReceiveHit,
		PlayerReceiveHit,
		NextWave,
		CreateItem,
		DeleteItem,
	};

	static void start();
	static std::map<std::string, Player*> getPlayers();
	static Map& getMap();
	static void broadcastPacket(sf::Packet packet, std::string nameException = "");

private:
	static void init();
	static unsigned int _currentNbPlayers;
	static std::map<std::string, Player*> _players;
	static sf::UdpSocket _socket;
	static Map _map;
	static void handlePlayerPos(sf::Packet packet);
	static void handleProjectile(sf::Packet packet);
	static void handlePlayerConnection(sf::Packet packet, sf::IpAddress playerAddr, unsigned int port);
	static void handleDeleteItem(sf::Packet packet);
	static void handleCreateItem(sf::Packet packet);
};

sf::Packet& operator <<(sf::Packet& packet, Item* item);
sf::Packet& operator >>(sf::Packet& packet, Item** item);
sf::Packet& operator <<(sf::Packet& packet, const std::vector<std::vector<int>>& myVec);
template<typename T>
inline sf::Packet & operator>>(sf::Packet & packet, sf::Vector2<T>& vec);
template <typename T>
inline sf::Packet& operator <<(sf::Packet& packet, const sf::Vector2<T>& vec);
sf::Packet& operator >>(sf::Packet& packet, SocketManager::PacketType& pt);