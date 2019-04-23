#include "SocketManager.h"

#include "ZombiesManager.h"
#include "ProjectilesManager.h"
#include "ItemsManager.h"
#include "Weapon.h"
#include "Ammo.h"

#include <iostream>

unsigned int SocketManager::_currentNbPlayers;
std::map<std::string, Player*> SocketManager::_players;
sf::UdpSocket SocketManager::_socket;
Map SocketManager::_map;

void SocketManager::start()
{
	init();
	sf::Packet packet;
	sf::IpAddress sender;
	unsigned short port;
	while (_socket.receive(packet, sender, port) == sf::Socket::Done) {
		PacketType packetType = PacketType::Unknown;
		packet >> packetType;
		switch (packetType) {
		case PacketType::Connection:
			handlePlayerConnection(packet, sender, port);
			break;
		case PacketType::PlayerPos:
			handlePlayerPos(packet);
			break;
		case PacketType::Projectile:
			handleProjectile(packet);
			break;
		case PacketType::CreateItem:
			handleCreateItem(packet);
			break;
		case PacketType::DeleteItem:
			handleDeleteItem(packet);
			break;
		default:
			std::cout << "Unknown packetType from " << sender << std::endl;
			break;
		}
	}
}

std::map<std::string, Player*> SocketManager::getPlayers()
{
	return _players;
}

Map & SocketManager::getMap()
{
	return _map;
}

void SocketManager::init()
{
	// liage de la socket au port 
	if (_socket.bind(9999) != sf::Socket::Done)
	{
		throw std::string("Impossible de lié la socket au port 9999");
	}

	std::cout << "Lancement du serveur en attente de nouvelles connexions..." << std::endl;
}

void SocketManager::handlePlayerPos(sf::Packet packet)
{
	sf::Vector2f pos;
	packet >> pos;
	float rotation;
	packet >> rotation;
	int showing;
	packet >> showing;
	std::string name;
	packet >> name;
	_players[name]->setPosition(pos);
	_players[name]->setRotation(rotation);
	sf::Packet broadcast;
	broadcast << PacketType::PlayerPos << name << pos << rotation << showing;
	broadcastPacket(broadcast, name);
}

void SocketManager::handleProjectile(sf::Packet packet)
{
	sf::Vector2f pos;
	packet >> pos;
	float rotation;
	packet >> rotation;
	int weaponType;
	packet >> weaponType;
	std::string name;
	packet >> name;
	ProjectilesManager::createProjectile(pos, rotation);
	sf::Packet broadcast;
	broadcast << PacketType::Projectile << name << pos << rotation << weaponType;
	broadcastPacket(broadcast, name);
}

void SocketManager::broadcastPacket(sf::Packet packet, std::string nameException)
{
	for (auto player = _players.begin(); player != _players.end(); player++)
	{
		if (player->second->name != nameException) {
			_socket.send(packet, player->second->addr, player->second->port);
		}
	}
}

void SocketManager::handlePlayerConnection(sf::Packet packet, sf::IpAddress playerAddr, unsigned int port)
{
	std::string name;
	packet >> name;
	// Si le joueur est déjà connecté
	if (_players.find(name) != _players.end()) {
		return;
	}
	_players[name] = new Player(playerAddr, name, port);
	++_currentNbPlayers;
	///
	/// Connexion
	///
	std::cout << "Connection de : " << name << " : " << playerAddr << std::endl;
	sf::Packet response;
	response << _map.getTiles();
	if (_socket.send(response, playerAddr, port) != sf::Socket::Done)
	{
		throw std::string("Erreur lors de l'envoie du paquet de la map");
	}
	///
	/// On fait se connecter les joueurs deja connecte au nouveau joueur
	///
	for (auto player = _players.begin(); player != _players.end(); player++)
	{
		if (player->second->name != name) {
			sf::Packet connection;
			connection << PacketType::Connection << player->second->name;
			_socket.send(connection, playerAddr, port);
		}
	}
	///
	/// On lui créer les zombies 
	///
	for (auto zombie = ZombiesManager::getZombies().begin(); zombie != ZombiesManager::getZombies().end(); zombie++) {
		sf::Packet zombieCreation;
		zombieCreation << PacketType::CreateZombie << zombie->second->getPosition();
		_socket.send(zombieCreation, playerAddr, port);
	}
	///
	/// On lui envoie les items
	///
	for (auto item = ItemsManager::getItems().begin(); item != ItemsManager::getItems().end(); item++) {
		sf::Packet itemCreation;
		itemCreation << PacketType::CreateItem << *item;
		_socket.send(itemCreation, playerAddr, port);
	}
	///
	/// On annonce à tous le monde que le joueur vient de se connecter
	///
	sf::Packet broadcast;
	broadcast << PacketType::Connection << name;
	broadcastPacket(broadcast, name);
}

void SocketManager::handleDeleteItem(sf::Packet packet)
{
	std::string name;
	Item* item;
	packet >> &item >> name;
	ItemsManager::deleteItem(item);
	sf::Packet broadcast;
	broadcast << PacketType::DeleteItem << item;
	broadcastPacket(broadcast, name);
}

void SocketManager::handleCreateItem(sf::Packet packet)
{
	Item* item;
	packet >> &item;
	ItemsManager::addItem(item);
}

sf::Packet & operator<<(sf::Packet & packet, Item * item)
{
	packet << item->getType() << item->getStack() << item->getPosition();
	return packet;
}

sf::Packet & operator>>(sf::Packet & packet, Item ** item)
{
	std::string type;
	unsigned int stack;
	sf::Vector2f position;
	packet >> type >> stack >> position;

	if (type == "Weapon") {
		*item = new Weapon();
	}
	else if (type == "Ammo") {
		*item = new Ammo(WeaponType::Gun);
	}
	(*item)->setStack(stack);
	(*item)->setPosition(position);
	return packet;
}

sf::Packet& operator <<(sf::Packet& packet, const std::vector<std::vector<int>>& myVec)
{
	for (size_t i = 0; i < myVec.size(); i++)
	{
		for (size_t j = 0; j < myVec[0].size(); j++)
		{
			packet << myVec[i][j];
		}
	}
	return packet;
}

template<typename T>
inline sf::Packet & operator>>(sf::Packet & packet, sf::Vector2<T>& vec)
{
	packet >> vec.x;
	packet >> vec.y;
	return packet;
}

template <typename T>
inline sf::Packet& operator <<(sf::Packet& packet, const sf::Vector2<T>& vec)
{
	packet << vec.x;
	packet << vec.y;
	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, SocketManager::PacketType& pt)
{
	int temp;
	packet >> temp;
	pt = SocketManager::PacketType(temp);
	return packet;
}