#include <SFML/Graphics.hpp>
#include <iostream>
#include "events.h"
#include "configuration.h"
#include "cellGrid.h"

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({Config::windowSize.x, Config::windowSize.y}), "CMake SFML Project", 
                                   sf::Style::Default, sf::State::Fullscreen);

    window.setFramerateLimit(Config::maxFrameRate);

    constexpr std::size_t row{ Config::windowSize.y / Config::cellSize };
    constexpr std::size_t col{ Config::windowSize.x / Config::cellSize };

    std::cout << row << ", " << col;
    CellGrid<row, col> cellGrid(window.getSize());

    while (window.isOpen())
    {
        processEvents(window);
       
        window.clear();
        
        window.draw(cellGrid);

        window.display();
    }
}
