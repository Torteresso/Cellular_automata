#include "events.h"
#include <iostream>

bool g_mouseLeftDown{ false };
bool g_mouseRightDown{ false };

void processEvents(sf::Window& window, CellGrid& cellGrid)
{
	while (const std::optional event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			window.close();
		}
		else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) window.close();

			if (keyPressed->scancode == sf::Keyboard::Scancode::Space) cellGrid.instantUpdate();
		}
		else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
		{
			if (mouseButtonPressed->button == sf::Mouse::Button::Left)
			{
				g_mouseLeftDown = true;
				cellGrid.setCell(Cell::alive, mouseButtonPressed->position);
			}
			else if (mouseButtonPressed->button == sf::Mouse::Button::Right)
			{
				g_mouseRightDown = true;
				cellGrid.setCell(Cell::dead, mouseButtonPressed->position);
			}
		}
		else if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
		{
			if (mouseButtonReleased->button == sf::Mouse::Button::Left)
			{
				g_mouseLeftDown = false;
			}
			else if (mouseButtonReleased->button == sf::Mouse::Button::Right)
			{
				g_mouseRightDown = false;
			}
		}
		else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
		{
			if (g_mouseLeftDown) cellGrid.setCell(Cell::alive, mouseMoved->position);
			else if (g_mouseRightDown) cellGrid.setCell(Cell::dead, mouseMoved->position);
		}
	}
}

