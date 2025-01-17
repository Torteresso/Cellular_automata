#include "common/events.h"
#include <iostream>
#include <cmath>

bool g_mouseLeftDown{ false };

void Event::processEvents(sf::RenderWindow& window, CellGrid& cellGrid, sf::View& view, Cell::Type& cellSelection, sf::CircleShape& cellSelectionCircle)
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

			if (keyPressed->scancode == sf::Keyboard::Scancode::R)
			{
				if (Config::startAsRandomGrid) cellGrid = CellGrid(window.getSize(), Config::cellSize, Config::dt);
				else cellGrid = CellGrid(window.getSize(), Config::cellSize, Config::dt, false);

				view = window.getDefaultView();
			}
		}
		else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
		{
			if (mouseButtonPressed->button == sf::Mouse::Button::Left)
			{
				g_mouseLeftDown = true;
				cellGrid.setCell(cellSelection, static_cast<sf::Vector2i>(window.mapPixelToCoords(mouseButtonPressed->position)));
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
				if (static_cast<int>(cellSelection) + 1 - static_cast<int>(Cell::Type::maxType) >= 0) cellSelection = static_cast<Cell::Type>(0);
				else cellSelection = static_cast<Cell::Type>(static_cast<int>(cellSelection) + 1);

				cellSelectionCircle.setFillColor(Cell(cellSelection).getColor());
			}
		}
		else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
		{
			if (g_mouseLeftDown) cellGrid.setCell(cellSelection, static_cast<sf::Vector2i>(window.mapPixelToCoords(mouseMoved->position)));

		}
		else if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
		{
			/*	if (std::abs(view.getCenter().x - window.getDefaultView().getCenter().x) + view.getSize().x/2 > window.getDefaultView().getSize().x/2
				 || std::abs(view.getCenter().y - window.getDefaultView().getCenter().y) + view.getSize().y/2 > window.getDefaultView().getSize().y/2)
				{
					sf::Vector2i test {0, 0};
					zoomViewAt(test, window, view, 1.0f - mouseWheelScrolled->delta / 20.0f);
				}
				else
				{*/
			//}
			view.zoom(1.0f - mouseWheelScrolled->delta / 20.0f);

			//if (view.getSize().lengthSquared() > window.getDefaultView().getSize().lengthSquared()) view.setSize(window.getDefaultView().getSize());
		}
	}
}

void Event::moveView(sf::RenderWindow& window, sf::View& view)
{
	sf::Vector2f scroll = window.mapPixelToCoords(sf::Mouse::getPosition()) - view.getCenter();
	const float velocity{ view.getSize().x / 800};
	const float dissipation{ 0.2f };

	const float halfWidth{ view.getSize().x / 2 };
	const float halfHeight{ view.getSize().y / 2};

	const float portionTriger{ 0.9 };

	if (scroll.x * scroll.x > halfWidth * halfWidth * portionTriger * portionTriger
		|| scroll.y * scroll.y > halfHeight * halfHeight * portionTriger * portionTriger)
	{
		const int signX{ (scroll.x > 0) ? 1 : ((scroll.x < 0) ? -1 : -1) };
		const int signY{ (scroll.y > 0) ? 1 : ((scroll.y < 0) ? -1 : -1) };

		if (scroll.x > 0)
		{
			(scroll.x > halfWidth * portionTriger) ? scroll.x -= halfWidth * portionTriger : scroll.x = 0;
		}
		else (scroll.x < -halfWidth *portionTriger) ? scroll.x += halfWidth * portionTriger : scroll.x = 0;
		if (scroll.y > 0) (scroll.y > halfHeight * portionTriger) ? scroll.y -= halfHeight * portionTriger : scroll.y = 0;
		else (scroll.y < -halfHeight * portionTriger) ? scroll.y -= halfHeight * portionTriger : scroll.y = 0;

		view.move({ signX *std::pow(std::abs(scroll.x), dissipation) * velocity , signY * std::pow(std::abs(scroll.y), dissipation) * velocity});
	}
}
