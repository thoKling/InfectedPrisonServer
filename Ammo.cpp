#include "Ammo.h"

Ammo::Ammo(WeaponType ammoType)
{
	_ammoType = ammoType;
}


Ammo::~Ammo()
{
}

WeaponType Ammo::getAmmoType()
{
	return _ammoType;
}

std::string Ammo::getType()
{
	return "Ammo";
}