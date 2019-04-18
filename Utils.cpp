#include "Utils.h"

bool Utils::debugMode = false;

int Utils::getQuadrant(const sf::Vector2f & pos1, const sf::Vector2f & pos2)
{
	if (pos2.x >= pos1.x)
	{
		if (pos2.y < pos1.y) // Quadrant 1: "en-haut à droite"
			return 1;
		else				 // Quadrant 2:  "en-bas à droite"
			return 2;
	}
	else
	{
		if (pos2.y >= pos1.y) // Quadrant 3: "en-bas à gauche"
			return 3;
		else				  // Quadrant 4: "en-haut à gauche"
			return 4;
	}
}

sf::Vector2f Utils::getVecUnit(const sf::Vector2f& startPos, const sf::Vector2f& endPos)
{
	if (startPos == endPos)
		return { 0,0 };
	sf::Vector2f res;
	res.x = (endPos.x - startPos.x) / Utils::distance(startPos, endPos);
	res.y = (endPos.y - startPos.y) / Utils::distance(startPos, endPos);
	return res;
}
double Utils::distance(const sf::Vector2f & pos1, const sf::Vector2f & pos2)
{
	float test1 = pow(pos1.x - pos2.x, 2);
	float test2 = pow(pos1.y - pos2.y, 2);
	return sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2));
}

std::vector<std::string> Utils::split(std::string stringToSplit, std::string delimiter)
{
	size_t pos = 0;
	std::vector<std::string> tokens;
	while ((pos = stringToSplit.find(delimiter)) != std::string::npos) {
		tokens.push_back(stringToSplit.substr(0, pos));
		//std::cout << token << std::endl;
		stringToSplit.erase(0, pos + delimiter.length());
	}
	tokens.push_back(stringToSplit);
	return tokens;
}

