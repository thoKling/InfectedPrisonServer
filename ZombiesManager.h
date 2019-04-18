#pragma once

#include <map>

#include "Zombie.h"

class ZombiesManager
{
public:
	static unsigned int createZombie(const sf::Vector2f& pos);
	static std::map<unsigned int, Zombie*>& getZombies();
	static void update();
	static void destroyZombies();

private:
	static std::map<unsigned int, Zombie*> _zombies;
	static unsigned int _nextId;
};

