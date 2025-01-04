#ifndef CELL_GRID_H
#define CELL_GRID_H

#include <array>
#include "cell.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <cassert>
#include <memory>
#include "random.h"

class CellGrid : public sf::Drawable, public sf::Transformable
{
	using Vector2d = std::vector<std::vector<std::unique_ptr<Cell>>>;

public:
	CellGrid() = delete;

	CellGrid(sf::Vector2u windowSize, int cellSize) 
		: m_cellSize {cellSize}, 
		  m_nbCol{static_cast<int>(windowSize.x/cellSize)}, 
		  m_nbRow{static_cast<int>(windowSize.y/cellSize)}
	{
		m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
		m_vertices.resize(6 * m_nbRow * m_nbCol);

		//m_grid.reserve(m_nbRow);
		for (int i{}; i < m_nbRow; i++)
		{
			std::vector<std::unique_ptr<Cell>> row;
			//row.reserve(m_nbCol);
			for (int j{}; j < m_nbCol; j++)
			{
				row.emplace_back(std::move(generateCell(static_cast<Cell::Type>(Random::get(0, static_cast<int>(Cell::Type::maxType) - 1)))));

				sf::Vertex* triangles = &m_vertices[(i + j * m_nbRow) * 6];

				triangles[0].position = sf::Vector2f(j * m_cellSize, i * m_cellSize);
				triangles[1].position = sf::Vector2f((j + 1) * m_cellSize, i * m_cellSize);
				triangles[2].position = sf::Vector2f(j * m_cellSize, (i + 1) * m_cellSize);
				triangles[3].position = sf::Vector2f(j * m_cellSize, (i + 1) * m_cellSize);
				triangles[4].position = sf::Vector2f((j + 1) * m_cellSize, i * m_cellSize);
				triangles[5].position = sf::Vector2f((j + 1) * m_cellSize, (i + 1) * m_cellSize);

				triangles[0].color = row.back().get()->getColor();
				triangles[1].color = row.back().get()->getColor();
				triangles[2].color = row.back().get()->getColor();
				triangles[3].color = row.back().get()->getColor();
				triangles[4].color = row.back().get()->getColor();
				triangles[5].color = row.back().get()->getColor();
			}

			m_grid.emplace_back(std::move(row));
		}
	}

	void update()
	{
		for (int i{}; i < m_nbRow; i++)
		{
			for (int j{}; j < m_nbCol; j++)
			{

			}
		}
	}

private:

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
        states.transform *= getTransform();

        states.texture = nullptr;

        target.draw(m_vertices, states);
	}

	Vector2d m_grid;

	int m_nbRow{};
	int m_nbCol{};
	int m_cellSize{};

	sf::VertexArray m_vertices;
	
};

#endif