#include "Item.h"

Item::Item()
{
}

Item::~Item()
{
}

unsigned int Item::getAmmo()
{
	return 0;
}

WeaponType Item::getWeaponType()
{
	return WeaponType::NaW; // Not a Weapon
}

WeaponType Item::getAmmoType()
{
	return WeaponType::NaW; // Not a Weapon
}

unsigned int Item::getStack()
{
	return _stack;
}

void Item::setStack(unsigned int stack)
{
	_stack = stack;
}

void Item::addStack(unsigned int stack)
{
	_stack += stack;
}

void Item::removeStack(unsigned int stack)
{
	_stack -= stack;
	if (_stack < 0)
		stack = 0;
}
