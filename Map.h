#pragma once

#include <vector>

#include <SFML/Graphics/Transformable.hpp>

#define TILESIZE 64

class Map
{
public:
	Map();
	~Map();

	std::vector<std::vector<int>> getTiles();
	sf::Vector2i transformInTilesPos(const sf::Vector2f& pos);
	bool isObstacle(const sf::Vector2i& pos);
private:
	std::vector<std::vector<int>> _tiles;
};

