#ifndef CELL_H
#define CELL_H

#include <memory>
#include <vector>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include "common/random.h"

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

	Cell(Type t, sf::Vector2i target = {100, 100})
		: m_type{ t }, m_target {target}
	{
		if (t == propagator && target == sf::Vector2i{ 100, 100 })
		{
			//m_target = { Random::get(0, 100), Random::get(0, 100) };
			//std::cout << m_target.x << ", " << m_target.y << "\n";
		}

		setColorFromType(t);
	}

	Cell(Type t, sf::Color c) : m_type {t}, m_color {c} {}


	const Type& getType() const { return m_type; }
	const sf::Color& getColor() const { return m_color; }

	Cell generateNext(const Vector2d& grid, const int row, const int col)
	{
		switch (m_type)
		{
		case dead:
			return generateFromDead(grid, row, col);
		case alive:
			return generateFromAlive(grid, row, col);
		case empty:
			return generateFromEmpty(grid, row, col);
		case propagator:
			return generateFromPropagator(grid, row, col);
		default:
			return Cell(dead);
		}
	}
	
private:
	static void transformCoordodinates(sf::Vector2i& v, const Vector2d& grid)
	{
		int nbRow{ static_cast<int>(grid.size()) };
		int nbCol{ static_cast<int>(grid[0].size()) };

		if (v.x < 0) v.x += nbRow;
		if (v.x >= nbRow) v.x -= nbRow;
		if (v.y < 0) v.y += nbCol;
		if (v.y >= nbCol) v.y -= nbCol;
	}

	void setColorFromType(const Type& t);

	Cell generateFromDead(const Vector2d& grid, const int row, const int col);
	Cell generateFromAlive(const Vector2d& grid, const int row, const int col);
	Cell generateFromEmpty(const Vector2d& grid, const int row, const int col);
	Cell generateFromPropagator(const Vector2d& grid, const int row, const int col);


	Type			m_type;
	sf::Color		m_color;
	sf::Vector2i	m_target;

};

#endif