#include "Zombie.h"

#include "Utils.h"
#include "SocketManager.h"
#include "ZombiesManager.h"

Zombie::Zombie(unsigned int id) : _id(id)
{
	_width = 48;
	_height = 48;
	setOrigin(24.f, 24.f);
}


Zombie::~Zombie()
{
}

void Zombie::update()
{
	if (_beingHit) {
		_tick++;
		if (_tick % 17 == 16) {
			_beingHit = false;
			_tick = 0;
		}
	}
	auto players = SocketManager::getPlayers();
	int minDist = INT_MAX;
	for (auto it = players.begin(); it != players.end(); it++)
	{
		if (Utils::distance(it->second->getPosition(), getPosition()) < minDist) {
			_target = it->second->getPosition();
			minDist = Utils::distance(it->second->getPosition(), getPosition());
			if (minDist < 80) {
				sf::Packet packet;
				packet << SocketManager::PacketType::PlayerReceiveHit << it->second->name << getPosition();
				SocketManager::broadcastPacket(packet);
			}
		}
	}

	// Si on est à distance on attaque
	if (Utils::distance(getPosition(), _target) > 80)
		myMove();
}

bool Zombie::isDead()
{
	return _lifes == 0;
}

void Zombie::receiveHit(sf::Vector2f hitterPosition)
{
	if (!_beingHit) {
		_beingHit = true;
		sf::Packet packet;
		packet << SocketManager::PacketType::ZombieReceiveHit << _id;
		SocketManager::broadcastPacket(packet);
		sf::Vector2f newPos;
		sf::Vector2f vecUnit = Utils::getVecUnit(hitterPosition, getPosition());
		for (size_t i = 0; i < 20; i++)
		{
			move(vecUnit.x * 5, 0);
			if (isInObstacle() || isInZombie()) {
				move(-vecUnit.x * 5, 0);
				break;
			}
		}
		for (size_t i = 0; i < 20; i++)
		{
			move(0, vecUnit.y * 5);
			if (isInObstacle() || isInZombie()) {
				move(0, -vecUnit.y * 5);
				break;
			}
		}
		_lifes--;
	}
}


void Zombie::myMove()
{
	orientate(_target);
	// On se déplace vers la destination
	sf::Vector2f vecUnit = Utils::getVecUnit(getPosition(), _target);

	this->move(vecUnit.x * _velocity, 0);
	if (isInObstacle() || isInZombie()) {
		this->move(-vecUnit.x * _velocity, 0);
	}

	this->move(0, vecUnit.y*_velocity);
	if (isInObstacle() || isInZombie()) {
		this->move(0, -vecUnit.y * _velocity);
	}
}

bool Zombie::isInZombie()
{
	for (auto zombie = ZombiesManager::getZombies().begin(); zombie != ZombiesManager::getZombies().end() ; zombie++)
	{
		if (zombie->second != this && zombie->second->getGlobalBounds().intersects(getGlobalBounds())) {
			return true;
		}
	}
	return false;
}
