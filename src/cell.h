#ifndef CELL_H
#define CELL_H

#include <memory>
#include <vector>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>

class Cell
{
public:
	enum Type
	{
		dead,
		alive,
		empty,
		propagator,
		maxType,
	};

	using Vector2d = std::vector<std::vector<Cell>>;

	Cell(Type t) : m_type{ t }
	{
		switch (t)
		{
		case dead:
			m_color = sf::Color::Black;
			break;
		case alive:
			m_color = sf::Color::White;
			break;
		case empty:
			m_color = sf::Color{ 116, 109, 105 };
			break;
		case propagator:
			m_color = sf::Color::Red;
			break;
		default:
			m_color = sf::Color::Green;
			break;
		}
	}

	const Type& getType() const { return m_type; }
	const sf::Color& getColor() const { return m_color; }

	Type generateNextType(const Vector2d& grid, const int row, const int col)
	{
		int nbCloseAlive{};

		switch (m_type)
		{
		case dead:


			for (int i{ row - 1 }; i <= row + 1; i++)
			{
				for (int j{ col - 1 }; j <= col + 1; j++)
				{

					sf::Vector2i coord{ i, j };
					transformCoordodinates(coord, grid);
					
					if (coord.x == row && coord.y == col) continue;
					
					if (grid[coord.x][coord.y].getType() == Type::alive) nbCloseAlive += 1;
				}
			}

			if (nbCloseAlive == 3)
			{
				return Type::alive;
			}

			return Type::dead;

		case alive:
			for (int i{ row - 1 }; i <= row + 1; i++)
			{
				for (int j{ col - 1 }; j <= col + 1; j++)
				{

					sf::Vector2i coord{ i, j };
					transformCoordodinates(coord, grid);
					
					if (coord.x == row && coord.y == col) continue;
					
					if (grid[coord.x][coord.y].getType() == Type::alive) nbCloseAlive += 1;
				}
			}

			if (nbCloseAlive == 3 || nbCloseAlive == 2)
			{
				return Type::alive;
			}

			return Type::dead;

		case empty:

			return Type::empty;

		case propagator:

			return Type::propagator;

		default:

			return Type::alive;
		}
	}
	
private:
	Type m_type;

	sf::Color m_color;

	static void transformCoordodinates(sf::Vector2i& v, const Vector2d& grid)
	{
		int nbRow{ static_cast<int>(grid.size()) };
		int nbCol{ static_cast<int>(grid[0].size()) };

		if (v.x < 0) v.x += nbRow;
		if (v.x >= nbRow) v.x -= nbRow;
		if (v.y < 0) v.y += nbCol;
		if (v.y >= nbCol) v.y -= nbCol;
	}

};

#endif