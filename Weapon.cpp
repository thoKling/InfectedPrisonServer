#include "Weapon.h"

Weapon::Weapon() :
	_capacity(5),
	_fireRate(10),
	_reloadSpeed(2),
	_type(WeaponType::Gun),
	_isReloading(false),
	_tickSinceReloadingUpdate(0),
	_tickSinceFiringUpdate(0),
	_isFireable(true)
{
	_ammo = _capacity;
}

Weapon::~Weapon()
{
}

unsigned int Weapon::getAmmo()
{
	return _ammo;
}

WeaponType Weapon::getWeaponType() {
	return _type;
}

std::string Weapon::getType()
{
	return "Weapon";
}
