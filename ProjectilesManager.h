#pragma once

#include <SFML/Graphics.hpp>
#include "Projectile.h"

class ProjectilesManager
{
public:
	static void deleteAllProjectiles();
	// M�thode qui permet de cr�er un nouveau projectile, renvoit son id
	static unsigned int createProjectile(const sf::Vector2f& pos, float rotation);

	// Proc�dure qui met � jour le comportement des projectiles
	static void update();

private:
	// Liste des projectiles
	static std::map<unsigned int, Projectile*> _projectiles;

	// identifiant suivant servant � la cr�ation d'un nouveau projectile
	static unsigned int _nextId;
};

