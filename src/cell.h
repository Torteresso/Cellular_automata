#ifndef CELL_H
#define CELL_H

#include <memory>
#include <vector>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

class Cell
{
public:

	using Vector2d = std::vector<std::vector<std::unique_ptr<Cell>>>;

	enum Type
	{
		dead,
		alive,
		maxType,
	};

	Cell(Type t, sf::Color c) : m_type{ t }, m_nextType{ t }, m_color { c } {}

	const Type& getType() const { return m_type; }
	const Type& getNexType() const { return m_nextType; }
	const sf::Color& getColor() const { return m_color; }

	virtual void generateNextType(const Vector2d& grid, const int row, const int col) = 0;
	
protected:
	Type m_type;
	Type m_nextType;

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

std::unique_ptr<Cell> generateCell(Cell::Type t);


class DeadCell : public Cell
{
public:
	DeadCell() : Cell(Type::dead, sf::Color::Black) {}

	void generateNextType(const Vector2d& grid, const int row, const int col) override
	{
		int nbCloseAlive{};

		for (int i{ row - 1 }; i <= row + 1; i++)
		{
			for (int j{ col - 1 }; j <= col + 1; j++)
			{

				sf::Vector2i coord{ i, j };
				transformCoordodinates(coord, grid);
				
				if ((coord.x == row && coord.y == col) || grid[coord.x][coord.y].get()->getType() == Type::dead) continue;

				nbCloseAlive += 1;
			}
		}

		if (nbCloseAlive == 3)
		{
			m_nextType = Type::alive;
			return;
		}

		m_nextType = Type::dead;
	}

private:

};

class AliveCell : public Cell
{
public:
	AliveCell() : Cell(Type::alive, sf::Color::White) {}

	void generateNextType(const Vector2d& grid, const int row, const int col) override
	{
		int nbCloseAlive{};

		for (int i{ row - 1 }; i <= row + 1; i++)
		{
			for (int j{ col - 1 }; j <= col + 1; j++)
			{

				sf::Vector2i coord{ i, j };
				transformCoordodinates(coord, grid);
				
				if ((coord.x == row && coord.y == col) || grid[coord.x][coord.y].get()->getType() == Type::dead) continue;

				nbCloseAlive += 1;
			}
		}

		if (nbCloseAlive == 3 || nbCloseAlive == 2)
		{
			m_nextType = Type::alive;
			return;
		}

		m_nextType = Type::dead;
	}

private:

};
#endif