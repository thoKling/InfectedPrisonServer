#include "Map.h"

#include <iostream>
#include <fstream>

Map::Map()
{
	std::ifstream infile;
	infile.open("Map/map.txt");
	if (!infile)
	{
		std::cout << "There was an error opening the file.\n";
	}

	char ch;
	int currentNumber = 0;
	std::vector<int> temp;

	while (!infile.eof()) {
		infile.get(ch);

		if (ch == ',') {
			temp.push_back(currentNumber);
			currentNumber = 0;
		}
		else if (ch == '\n') {
			_tiles.push_back(temp);
			temp.clear();
		}
		else {
			int ia = ch - '0';
			currentNumber = currentNumber * 10 + ia;
		}
	}
	_tiles.push_back(temp);
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
	return !((_tiles[pos.y][pos.x] == 10) || (_tiles[pos.y][pos.x] == 319));
}
