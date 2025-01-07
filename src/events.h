#ifndef EVENTS_H
#define EVENTS_H

#include <SFML/Window.hpp>
#include "cellGrid.h"


namespace Event
{
	void processEvents(sf::RenderWindow& window, CellGrid& cellGrid, sf::View& view);

	void moveView(sf::RenderWindow& window, sf::View& view);

	void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, sf::View& view, float zoom);
}

#endif