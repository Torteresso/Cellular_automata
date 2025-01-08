#ifndef CELL_GRID_H
#define CELL_GRID_H

#include <array>
#include "cell.h"
#include <SFML/Graphics.hpp>
#include <cassert>
#include <memory>
#include "random.h"
#include <SFML/System/Vector2.hpp>
#include <iostream>


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
		m_cells.push_back(std::move(generateCell(Cell::Type::empty)));

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
				if (random)
				{
					const int r{ Random::get(0, 1000) };
					Cell::Type t;

					if (r <= 1) t = Cell::Type::empty;
					else if (r <= 500) t = Cell::Type::alive;
					else t = Cell::Type::dead;

					row.emplace_back(t);
				}
				else
				{
					row.emplace_back(Cell::Type::dead);
				}

				nextRow.emplace_back(Cell::Type::dead);

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
		for (int i{}; i < m_nbRow; i++)
		{
			for (int j{}; j < m_nbCol; j++)
			{
				m_nextGrid[i][j] = m_cells[m_grid[i][j]].get()->generateNextType(m_grid, i, j);
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
		for (int i{}; i < m_nbRow; i++)
		{
			for (int j{}; j < m_nbCol; j++)
			{
				m_grid[i][j] = m_nextGrid[i][j];

				sf::Vertex* triangles = &m_vertices[(i + j * m_nbRow) * 6];

				sf::Color cellColor = m_cells[m_grid[i][j]].get()->getColor();
				triangles[0].color = cellColor;
				triangles[1].color = cellColor;
				triangles[2].color = cellColor;
				triangles[3].color = cellColor;
				triangles[4].color = cellColor;
				triangles[5].color = cellColor;
			}
		}
	}

	void setCell(Cell::Type type, const sf::Vector2i& pos)
	{
		int i{ pos.y / m_cellSize };
		int j{ pos.x / m_cellSize };

		if (i < 0 || i >= m_nbRow || j < 0 || j >= m_nbCol) return;

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

	int m_nbRow{};
	int m_nbCol{};
	int m_cellSize{};

	sf::VertexArray m_vertices;
	
	sf::Time m_tBetweenUdpates;
	sf::Time m_tRemaining;
	
};

#endif