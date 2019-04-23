#pragma once

#include <string>

#include <SFML/Graphics/Transformable.hpp>

enum WeaponType {
	NaW, // Not a Weapon
	Gun,
};

const std::string WeaponTypesStr[] = { "NaW", "Gun" };

class Item : public sf::Transformable
{
public:
	Item();
	~Item();
	virtual unsigned int getStack();
	virtual void setStack(unsigned int stack);
	virtual void addStack(unsigned int stack);
	virtual void removeStack(unsigned int stack);

	virtual unsigned int getAmmo();
	virtual WeaponType getWeaponType();
	// Ammos
	virtual WeaponType getAmmoType();

	// Nécessaire pour la sérialization
	virtual std::string getType() = 0;
protected:
	unsigned int _stack = 0;
};

