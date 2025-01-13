#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <SFML/System/Vector2.hpp>

namespace Config
{
	sf::Vector2u constexpr windowSize {1920, 1200};
	sf::Vector2f const windowSizef{ static_cast<sf::Vector2f>(windowSize) };
	int const maxFrameRate{ 60 };
	static sf::Time dt{sf::seconds(1.0f / static_cast<float>(maxFrameRate)) };

	int const cellSize{ 2 };

	/* BEST PERFORMANCE YET FOR GAME OF LIFE IN REALEASE MODE WITH PC POWER EFFICIENCY PLUNGED IN
	* dt = 1.0f
	* cellSize = 3
	* This correspond to 256 000 cells evolving at 60 fps
	*/

	constexpr bool showFps{ true };
	constexpr bool manualUpdateOnly{ false };
	constexpr bool startAsRandomGrid{ true };

}

#endif