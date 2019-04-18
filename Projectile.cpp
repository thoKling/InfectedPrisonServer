#include "Projectile.h"

#include "Utils.h"
#include "ZombiesManager.h"
#include "SocketManager.h"

Projectile::Projectile()
{
	_width = 32;
	_height = 32;
	this->setOrigin(16.f, 16.f);
}

Projectile::~Projectile()
{
}

bool Projectile::toDelete()
{
	return _toDelete;
}

void Projectile::update()
{
	move(_velocity*cos(getRotation()*toRadians), _velocity*sin(getRotation()*toRadians));
	// Portée max tu projectile
	_travelledDistance += _velocity;
	if (_travelledDistance > 800)
		_toDelete = true;
	// Collisions avec les zombies
	auto zombies = ZombiesManager::getZombies();
	for (auto it = zombies.begin(); it != zombies.end(); ++it)
	{
		if (it->second->getGlobalBounds().intersects(getGlobalBounds())) {
			it->second->receiveHit(getPosition());
			_toDelete = true;
			break;
		}
	}
	// Collisions avec les murs
	if (isInObstacle())
		_toDelete = true;
}