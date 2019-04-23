#pragma once

#include "Item.h"

class Ammo : public Item
{
public:
	Ammo(WeaponType ammoType);
	~Ammo();
	virtual WeaponType getAmmoType();
	virtual std::string getType();

private:
	WeaponType _ammoType;
};

