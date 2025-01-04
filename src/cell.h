#ifndef CELL_H
#define CELL_H

#include <memory>
#include <SFML/Graphics/Color.hpp>

class Cell
{
public:

	enum Type
	{
		dead,
		alive,
	};

	Cell(Type t, sf::Color c) : m_type{ t }, m_color{ c } {}

	const Type& getType() const { return m_type; }

	virtual Type update() = 0;

protected:
	Type m_type;
	sf::Color m_color;
};

std::unique_ptr<Cell> generateCell(Cell::Type t);

class DeadCell : public Cell
{
public:
	DeadCell() : Cell(Cell::Type::dead, sf::Color::Black) {}

	Type update() override
	{
		return m_type;
	}
private:

};

class AliveCell : public Cell
{
public:
	AliveCell() : Cell(Cell::Type::alive, sf::Color::White) {}

	Type update() override
	{
		return m_type;
	}
private:

};
#endif