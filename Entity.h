#pragma once

#include <SFML/Graphics/Transformable.hpp>

#include <vector>

class Entity : public sf::Transformable
{
public:
	Entity();
	~Entity();
	sf::Vector2f getPosition() const;
	sf::FloatRect getGlobalBounds();
protected:
	std::vector<sf::Vector2f> getCorners();
	bool isInObstacle();
	void orientate(const sf::Vector2f & position);
	unsigned int _width;
	unsigned int _height;
};

