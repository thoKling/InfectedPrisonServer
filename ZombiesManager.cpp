#include "ZombiesManager.h"

#include "SocketManager.h"

unsigned int ZombiesManager::_nextId = 0;
std::map<unsigned int, Zombie*> ZombiesManager::_zombies;

// on désalloue la mémoire utilisée pour stocker les zombies
void ZombiesManager::destroyZombies()
{
	for (auto it = _zombies.begin(); it != _zombies.end(); ++it)
	{
		delete it->second;
	}
}

// créé un nouveau zombie et renvoit l'id de ce dernier
unsigned int ZombiesManager::createZombie(const sf::Vector2f& pos) {
	Zombie* temp = new Zombie(_nextId);
	temp->setPosition(pos);
	if (temp->isInZombie())
		temp->move(50, 0);
	_zombies[_nextId] = temp;
	_nextId++;
	sf::Packet packet;
	packet << SocketManager::PacketType::CreateZombie << pos;
	SocketManager::broadcastPacket(packet);
	return _nextId - 1;
}

std::map<unsigned int, Zombie*>& ZombiesManager::getZombies()
{
	return _zombies;
}

// on update chaque zombie
void ZombiesManager::update() {
	std::vector<unsigned int> toErase;
	for (auto it = _zombies.begin(); it != _zombies.end(); ++it)
	{
		it->second->update();
		if (it->second->isDead())
			toErase.push_back(it->first);
		else {
			sf::Packet packet;
			packet << SocketManager::PacketType::ZombieState << it->first << it->second->getPosition() << it->second->getRotation();
			SocketManager::broadcastPacket(packet);
		}
	}
	// On supprime les zombies morts
	for (auto it = toErase.begin(); it != toErase.end(); ++it)
	{
		_zombies.erase(*it);
	}
}