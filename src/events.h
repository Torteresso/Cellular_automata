#ifndef EVENTS_H
#define EVENTS_H

#include <SFML/Window.hpp>
#include "cellGrid.h"
#include "configuration.h"


namespace Event
{
	void processEvents(sf::RenderWindow& window, CellGrid& cellGrid, sf::View& view, Cell::Type& cellSelection, sf::CircleShape& cellSelectionCircle);

	void moveView(sf::RenderWindow& window, sf::View& view);
}

#endif