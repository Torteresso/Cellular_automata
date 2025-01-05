#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <SFML/System/Vector2.hpp>

namespace Config
{
	sf::Vector2u constexpr windowSize {1920, 1200};
	sf::Vector2f const windowSizef{ static_cast<sf::Vector2f>(windowSize) };
	int const maxFrameRate{ 60 };
	sf::Time dt{sf::seconds(1.0f / static_cast<float>(maxFrameRate)) };

	int const cellSize{ 6 };
}

#endif