#include "Map.h"



Map::Map()
{
	// création intiale de la map
	for (size_t i = 0; i < 16; i++)
	{
		std::vector<int> temp;
		for (size_t j = 0; j < 32; j++)
		{
			if (j == 0 || j == 31 || i == 0 || i == 15)
				temp.push_back(41);
			else if (j == 5 && (i <= 5))
				temp.push_back(41);
			else
				temp.push_back(10);
		}
		_tiles.push_back(temp);
	}
}


Map::~Map()
{
}

std::vector<std::vector<int>> Map::getTiles()
{
	return _tiles;
}

sf::Vector2i Map::transformInTilesPos(const sf::Vector2f & pos)
{
	return sf::Vector2i((int)(pos.x) / TILESIZE, (int)(pos.y) / TILESIZE);
}

bool Map::isObstacle(const sf::Vector2i & pos)
{
	// si on demande une position en dehors de la map
	if (pos.x >= _tiles[0].size() || pos.y >= _tiles.size()) {
		return 1;
	}
	return _tiles[pos.y][pos.x] == 41;
}
