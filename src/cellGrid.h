#ifndef CELL_GRID_H
#define CELL_GRID_H

#include <array>
#include "cell.h"
#include <SFML/Graphics.hpp>
#include <cassert>
#include <memory>
#include "random.h"
#include <SFML/System/Vector2.hpp>
#include <unordered_set>
#include <iostream>


namespace std {
template <> struct hash<std::pair<int, int>> {
    inline size_t operator()(const std::pair<int, int> &v) const {
        std::hash<int> int_hasher;
        return int_hasher(v.first) ^ int_hasher(v.second);
    }
};

}

class CellGrid : public sf::Drawable, public sf::Transformable
{
	using Vector2d = std::vector<std::vector<Cell::Type>>;

public:
	CellGrid() = delete;

	CellGrid(sf::Vector2u windowSize, int cellSize, sf::Time tBetweenUpdates, bool random=true) 
		: m_cellSize {cellSize}, 
		  m_nbCol{static_cast<int>(windowSize.x/cellSize)}, 
		  m_nbRow{static_cast<int>(windowSize.y/cellSize)},
		  m_tBetweenUdpates {tBetweenUpdates},
		  m_tRemaining {tBetweenUpdates}
	{
		m_cells.push_back(std::move(generateCell(Cell::Type::dead)));
		m_cells.push_back(std::move(generateCell(Cell::Type::alive)));

		m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
		m_vertices.resize(6 * m_nbRow * m_nbCol);

		m_grid.reserve(m_nbRow);
		m_nextGrid.reserve(m_nbRow);
		for (int i{}; i < m_nbRow; i++)
		{
			std::vector<Cell::Type> row;
			std::vector<Cell::Type> nextRow;
			row.reserve(m_nbCol);
			nextRow.reserve(m_nbCol);
			for (int j{}; j < m_nbCol; j++)
			{
				m_indexToUpdate.insert(std::make_pair(i, j));

				if (random)
				{
					row.emplace_back(static_cast<Cell::Type>(Random::get(0, static_cast<int>(Cell::Type::maxType) - 1)));
				}
				else
				{
					row.emplace_back(Cell::Type::dead);
				}

				nextRow.emplace_back(row.back());

				sf::Vertex* triangles = &m_vertices[(i + j * m_nbRow) * 6];

				triangles[0].position = sf::Vector2f(j * m_cellSize, i * m_cellSize);
				triangles[1].position = sf::Vector2f((j + 1) * m_cellSize, i * m_cellSize);
				triangles[2].position = sf::Vector2f(j * m_cellSize, (i + 1) * m_cellSize);
				triangles[3].position = sf::Vector2f(j * m_cellSize, (i + 1) * m_cellSize);
				triangles[4].position = sf::Vector2f((j + 1) * m_cellSize, i * m_cellSize);
				triangles[5].position = sf::Vector2f((j + 1) * m_cellSize, (i + 1) * m_cellSize);

				sf::Color cellColor = m_cells[row.back()].get()->getColor();
				triangles[0].color = cellColor;
				triangles[1].color = cellColor;
				triangles[2].color = cellColor;
				triangles[3].color = cellColor;
				triangles[4].color = cellColor;
				triangles[5].color = cellColor;
			}

			m_grid.emplace_back(row);
			m_nextGrid.emplace_back(nextRow);
		}
	}

	void prepareNextGrid()
	{
		m_nextIndexToUpdate.reserve(2*m_nextGrid.size());
		for (const auto& coord : m_indexToUpdate)
		{
			Cell::Type newType = m_cells[m_grid[coord.first][coord.second]].get()->generateNextType(m_grid, coord.first, coord.second);
			if (newType != m_grid[coord.first][coord.second])
			{
				m_nextGrid[coord.first][coord.second] = newType;
				insertCoordSquare(m_nextIndexToUpdate, coord);
				//m_nextIndexToUpdate.insert({ coord.first, coord.second });
			}
		}
	}
	
	void instantUpdate()
	{
		update(m_tBetweenUdpates);
	}

	void update(const sf::Time& elapsed)
	{
		m_tRemaining -= elapsed;
		if (m_tRemaining > sf::Time::Zero) return;
		m_tRemaining += m_tBetweenUdpates;

		prepareNextGrid();

		for (const auto& coord : m_indexToUpdate)
		{
			m_grid[coord.first][coord.second] = m_nextGrid[coord.first][coord.second];

			sf::Vertex* triangles = &m_vertices[(coord.first + coord.second * m_nbRow) * 6];

			sf::Color cellColor = m_cells[m_grid[coord.first][coord.second]].get()->getColor();
			triangles[0].color = cellColor;
			triangles[1].color = cellColor;
			triangles[2].color = cellColor;
			triangles[3].color = cellColor;
			triangles[4].color = cellColor;
			triangles[5].color = cellColor;

		}
		m_indexToUpdate = m_nextIndexToUpdate;
		m_nextIndexToUpdate.clear();
	}

	void setCell(Cell::Type type, const sf::Vector2i& pos)
	{
		int i{ pos.y / m_cellSize };
		int j{ pos.x / m_cellSize };

		m_grid[i][j] = type;

		sf::Vertex* triangles = &m_vertices[(i + j * m_nbRow) * 6];

		sf::Color cellColor = m_cells[m_grid[i][j]].get()->getColor();
		triangles[0].color = cellColor;
		triangles[1].color = cellColor;
		triangles[2].color = cellColor;
		triangles[3].color = cellColor;
		triangles[4].color = cellColor;
		triangles[5].color = cellColor;
	}

	void insertCoordSquare(std::unordered_set<std::pair<int, int>>& set, const std::pair<int, int>& coord)
	{
		//std::pair<int, int> test{ 1, 2 };
		for (int i{ coord.first - 1 }; i <= coord.first + 1; i++)
		{
			for (int j{ coord.second - 1 }; j <= coord.second + 1; j++)
			{
				set.insert(transformToValidPair(std::make_pair(i, j)));
				//std::make_pair(i, j);
			}
		}
	}

	std::pair<int, int>& transformToValidPair(std::pair<int, int>& coord)
	{
		if (coord.first < 0) coord.first += m_nbRow;
		if (coord.first >= m_nbRow) coord.first -= m_nbRow;
		if (coord.second < 0) coord.second += m_nbCol;
		if (coord.second >= m_nbCol) coord.second -= m_nbCol;

		return coord;
	}
	
private:

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
        states.transform *= getTransform();

        states.texture = nullptr;

        target.draw(m_vertices, states);
	}

	std::vector<std::unique_ptr<Cell>> m_cells;
	Vector2d m_grid;
	Vector2d m_nextGrid;
	std::unordered_set<std::pair<int, int>> m_indexToUpdate;
	std::unordered_set<std::pair<int, int>> m_nextIndexToUpdate;
	int m_nbRow{};
	int m_nbCol{};
	int m_cellSize{};

	sf::VertexArray m_vertices;
	
	sf::Time m_tBetweenUdpates;
	sf::Time m_tRemaining;
	
};

#endif