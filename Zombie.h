#pragma once

#include "Entity.h"

class Zombie : public Entity
{
public:
	Zombie(unsigned int id);
	~Zombie();
	void update();
	bool isDead();
	void receiveHit(sf::Vector2f hitterPosition);
private:
	bool _beingHit = false;
	unsigned int _lifes = 3;
	void myMove();
	float _velocity = 4.0f;
	unsigned int _tick = 0;
	unsigned int _id;
	sf::Vector2f _target;
};

