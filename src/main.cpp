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

    CellGrid cellGrid(window.getSize(), Config::cellSize);

    while (window.isOpen())
    {
        processEvents(window);
       
        window.clear();
        
        window.draw(cellGrid);

        window.display();
    }
}
