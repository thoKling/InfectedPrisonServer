#pragma once

#include "Entity.h"

class Projectile : public Entity
{
public:
	Projectile();
	~Projectile();

	bool toDelete();

	void update();

private:
	double _velocity = 16.0;
	double _travelledDistance = 0.0;
	sf::Vector2f _direction;
	bool _toDelete = false;
};

