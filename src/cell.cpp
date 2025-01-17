#include "cell.h"
#include "utility.h"

void Cell::setColorFromType(const Type& t)
{
	switch (t)
	{
	case dead:
		m_color = sf::Color::Black;
		break;
	case alive:
		m_color = sf::Color{ 195, 255, 255 };
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

Cell Cell::generateFromDead(const Vector2d& grid, const int row, const int col)
{
	int nbCloseAlive{};

	for (int i{ row - 1 }; i <= row + 1; i++)
	{
		for (int j{ col - 1 }; j <= col + 1; j++)
		{

			sf::Vector2i coord{ i, j };
			transformCoordodinates(coord, grid);
			
			if (coord.x == row && coord.y == col) continue;
			
			if (grid[coord.x][coord.y].getType() == alive) nbCloseAlive += 1;
		}
	}

	if (nbCloseAlive == 3)
	{
		return Cell(alive, sf::Color{static_cast<uint8_t>(255 - 30 * nbCloseAlive), 255, 255});
	}

	return Cell(dead);

}


Cell Cell::generateFromAlive(const Vector2d& grid, const int row, const int col)
{
	int nbCloseAlive{};

	for (int i{ row - 1 }; i <= row + 1; i++)
	{
		for (int j{ col - 1 }; j <= col + 1; j++)
		{

			sf::Vector2i coord{ i, j };
			transformCoordodinates(coord, grid);
			
			if (coord.x == row && coord.y == col) continue;
			
			if (grid[coord.x][coord.y].getType() == alive) nbCloseAlive += 1;
		}
	}

	if (nbCloseAlive == 3 || nbCloseAlive == 2)
	{
		return Cell(alive, sf::Color{static_cast<uint8_t>(255 - 30 * nbCloseAlive), 255, 255});
	}

	return Cell(dead);


}

Cell Cell::generateFromEmpty(const Vector2d& grid, const int row, const int col)
{
	int nbTest{};
	int nbCloserPropagatorNeighbour{ };
	int nbFurtherPropagatorNeighbour{ };
	sf::Vector2i target;

	for (int i{ row - 1 }; i <= row + 1; i++)
	{
		for (int j{ col - 1 }; j <= col + 1; j++)
		{
			sf::Vector2i coord{ i, j };
			transformCoordodinates(coord, grid);

			if (coord.x == row && coord.y == col) continue;

			if (grid[coord.x][coord.y].getType() == propagator)
			{
				nbTest += 1;
				target = grid[coord.x][coord.y].m_target;
					
				sf::Vector2i neighbourVToTarget = grid[coord.x][coord.y].m_target - coord;
				sf::Vector2f neighbourCellCorner{ coord.x + 0.5f * Utility::sign(neighbourVToTarget.x), 
												  coord.y + 0.5f * Utility::sign(neighbourVToTarget.y) };
				
				sf::Vector2i vToTarget = grid[coord.x][coord.y].m_target - sf::Vector2i{row, col};
				sf::Vector2f cellCorner{ row - 0.5f * Utility::sign(vToTarget.x), 
												  col - 0.5f * Utility::sign(vToTarget.y) };

				//std::cout << Utility::distance(cellCorner, grid[coord.x][coord.y].m_target)
					//<< ", " << Utility::distance(neighbourCellCorner, grid[coord.x][coord.y].m_target) << "\n";

				if (Utility::distance(cellCorner, grid[coord.x][coord.y].m_target)
					<= Utility::distance(neighbourCellCorner, grid[coord.x][coord.y].m_target))
				{
					nbFurtherPropagatorNeighbour += 1;
					continue;
				}

				nbCloserPropagatorNeighbour += 1;
			}

		}
	}

	if (nbFurtherPropagatorNeighbour >= nbCloserPropagatorNeighbour 
	   && nbCloserPropagatorNeighbour + nbFurtherPropagatorNeighbour >= 1)
	{
		//std::cout << target.x << "-----" << target.y << "\n";
		return Cell(propagator, target);
	}
	if (nbTest >= 5) return Cell(propagator, target);


	return Cell(empty);
}

Cell Cell::generateFromPropagator(const Vector2d& grid, const int row, const int col)
{
	int nbClosePropgator{};

	for (int i{ row - 1 }; i <= row + 1; i++)
	{
		for (int j{ col - 1 }; j <= col + 1; j++)
		{

			sf::Vector2i coord{ i, j };
			transformCoordodinates(coord, grid);
			
			if (coord.x == row && coord.y == col) continue;
			
			if (grid[coord.x][coord.y].getType() == propagator) nbClosePropgator += 1;
		}
	}

	if (nbClosePropgator == 1)
	{
		return Cell(propagator, m_target);
	}

	//return Cell(empty);
	return Cell(propagator, m_target);
}

