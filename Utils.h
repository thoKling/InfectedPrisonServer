#pragma once

#include <SFML/Graphics.hpp>

constexpr auto PI = 3.14159265;

#define toRadians PI / 180.0
#define toDegrees 180.0 / PI
// Nombre de regions max
#define REGION_NUMBER 400

#define WINDOWS_HEIGHT 512
#define WINDOWS_WIDTH 1024

class Utils
{
public:
	// Récupération du numéro de quadrant dans lequel se situe la pos2 par rapport a la pos1
	static int getQuadrant(const sf::Vector2f& pos1, const sf::Vector2f& pos2);
	// Renvoit le vecteur unitaire de la startPos à la endPos
	static sf::Vector2f getVecUnit(const sf::Vector2f& startPos, const sf::Vector2f& endPos);
	// Renvoit la distance entre 2 coordonnées
	static double distance(const sf::Vector2f& pos1, const sf::Vector2f& pos2);
	// Fonction split pour les strings, renvoit les différents morceaux de la chaine entre les delimiter
	static std::vector<std::string> split(std::string stringToSplit, std::string delimiter);
	// True en mode debug
	static bool debugMode;
};


template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
	std::hash<T> hasher;
	seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}
//Structure de hashage pour un vector2i
template<typename T>
struct vector2i_hash
{
	std::size_t operator()(const sf::Vector2<T>& k) const
	{
		std::size_t seed = 0;
		hash_combine(seed, k.x);
		hash_combine(seed, k.y);
		return seed;
	}
};
