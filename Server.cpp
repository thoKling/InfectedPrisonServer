#include "Server.h"

#include <iostream>

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


enum PacketType {
	Unknown,
	Connection,
	PlayerPos,
	Projectile,
};

template <typename T>
inline sf::Packet& operator <<(sf::Packet& packet, const sf::Vector2<T>& vec)
{
	packet << vec.x;
	packet << vec.y;
	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, PacketType& pt)
{
	int temp;
	packet >> temp;
	pt = PacketType(temp);
	return packet;
}

Server::Server()
{
	// liage de la socket au port 
	if (_socket.bind(9999) != sf::Socket::Done)
	{
		throw std::string("Impossible de lié la socket au port 9999");
	}

	std::cout << "Lancement du serveur en attente de nouvelles connexions..." << std::endl;
	// création intiale de la map
	for (size_t i = 0; i < 16; i++)
	{
		std::vector<int> temp;
		for (size_t j = 0; j < 32; j++)
		{
			if (j == 0 || j == 31 || i == 0 || i == 15)
				temp.push_back(41);
			else if (j == 5 && (i <= 5))
				temp.push_back(41);
			else
				temp.push_back(10);
		}
		_level.push_back(temp);
	}
	_players.resize(NB_PLAYERS);
	for (size_t i = 0; i < NB_PLAYERS; i++)
	{
		_players[i] = nullptr;
	}
}


Server::~Server()
{
	_socket.unbind();
}

// Lancement du serveur
void Server::start() {

	///
	/// Boucle principale
	///
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
			default:
				std::cout << "Unknown packetType from "<< sender << std::endl;
				break;
		}
	}
}

// Envoie la map à un joueur
void Server::sendMap(sf::IpAddress destination) {
	std::cout << "Envoie de la map a : " << destination << std::endl;
	unsigned short port = 10000;
	sf::Packet packet;
	packet << _level;
	if (_socket.send(packet, destination, port) != sf::Socket::Done)
	{
		throw std::string("Erreur lors de l'envoie du paquet de la map");
	}
}

void Server::handlePlayerPos(sf::Packet packet)
{
	sf::Vector2f pos;
	packet >> pos;
	float rotation;
	packet >> rotation;
	std::string name;
	packet >> name;
	sf::Packet broadcast;
	broadcast << PacketType::PlayerPos << name << pos << rotation; 
	broadcastPacket(broadcast, name);
}

void Server::handleProjectile(sf::Packet packet)
{
	float rotation;
	packet >> rotation;
	sf::Vector2f pos;
	packet >> pos;
	int weaponType;
	packet >> weaponType;
	std::string name;
	packet >> name;
	sf::Packet broadcast;
	broadcast << PacketType::Projectile << name << rotation << pos << weaponType;
	broadcastPacket(broadcast, name);
}

void Server::broadcastPacket(sf::Packet packet, std::string nameException)
{
	for (auto player = _players.begin(); player != _players.end(); player++)
	{
		if (*player != nullptr)
			if ((*player)->name != nameException) {
				_socket.send(packet, (*player)->addr, (*player)->port);
			}
	}
}

void Server::handlePlayerConnection(sf::Packet packet, sf::IpAddress player, unsigned int port)
{
	std::string name;
	packet >> name;
	// Si le joueur est déjà connecté
	if (getPlayerId(name) != -1) {
		return;
	}
	///
	/// On cherche une place libre
	///
	int playerId = -1;
	for (size_t i = 0; i < NB_PLAYERS; i++)
	{
		if (_players[i] == nullptr) {
			_players[i] = new Player(player, name, port);
			playerId = i;
			++_currentNbPlayers;
			break;
		}
	}
	if (playerId == -1)
		return;
	///
	/// Connexion
	///
	std::cout << "Connection de : " << name << " : "<<player << std::endl;
	sf::Packet response;
	response <<_level;
	if (_socket.send(response, player, port) != sf::Socket::Done)
	{
		throw std::string("Erreur lors de l'envoie du paquet de la map");
	}
	for (size_t i = 0; i < NB_PLAYERS; i++)
	{
		if (_players[i] != nullptr) {
			sf::Packet connection;
			connection << PacketType::Connection << _players[i]->name;
			if (_players[i]->name != name) {
				_socket.send(connection, player, port);
			}
		}
	}
	sf::Packet broadcast;
	broadcast << PacketType::Connection << name;
	broadcastPacket(broadcast, name);
}
int Server::getPlayerId(std::string name) {
	for (size_t i = 0; i < NB_PLAYERS; i++)
	{
		if(_players[i] != nullptr)
			if (_players[i]->name == name) {
				return i;
			}
	}
	return -1;
}