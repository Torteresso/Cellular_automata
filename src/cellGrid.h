#ifndef CELL_GRID_H
#define CELL_GRID_H

#include <array>
#include "cell.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <cassert>

template <std::size_t Row, std::size_t Col>
class CellGrid : public sf::Drawable, public sf::Transformable
{
	using Array2d = std::array<std::array<std::unique_ptr<Cell>, Col>, Row>;

public:
	CellGrid() = delete;

	CellGrid(sf::Vector2u windowSize) : m_windowSize {windowSize}, m_cellSize {static_cast<int>(windowSize.x/Col)}
	{

		m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
		m_vertices.resize(6 * Row * Col);
		for (int i{}; i < static_cast<int>(Row); i++)
		{
			for (int j{}; j < static_cast<int>(Col); j++)
			{
				m_grid[i][j] = generateCell(Cell::Type::dead);



				sf::Vertex* triangles = &m_vertices[(i + j * Row) * 6];

				triangles[0].position = sf::Vector2f(j * m_cellSize, i * m_cellSize);
				triangles[1].position = sf::Vector2f((j + 1) * m_cellSize, i * m_cellSize);
				triangles[2].position = sf::Vector2f(j * m_cellSize, (i + 1) * m_cellSize);
				triangles[3].position = sf::Vector2f(j * m_cellSize, (i + 1) * m_cellSize);
				triangles[4].position = sf::Vector2f((j + 1) * m_cellSize, i * m_cellSize);
				triangles[5].position = sf::Vector2f((j + 1) * m_cellSize, (i + 1) * m_cellSize);

				triangles[0].color = sf::Color::Green;
				triangles[1].color = sf::Color::Green;
				triangles[2].color = sf::Color::Green;
				triangles[3].color = sf::Color::Green;
				triangles[4].color = sf::Color::Green;
			}
		}
	}

	void update()
	{
		for (int i{}; i < static_cast<int>(Row); i++)
		{
			for (int j{}; j < static_cast<int>(Col); j++)
			{
				
				const Cell& currentCell{ m_grid[i][j] };

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

	Array2d m_grid{};

	sf::Vector2u m_windowSize{};
	int m_cellSize{};

	sf::VertexArray m_vertices;
	
};

#endif