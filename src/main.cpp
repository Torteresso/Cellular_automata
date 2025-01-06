#include <SFML/Graphics.hpp>
#include <iostream>
#include "events.h"
#include "configuration.h"
#include "cellGrid.h"
#include "utility.h"

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({Config::windowSize.x, Config::windowSize.y}), "CMake SFML Project", 
                                   sf::Style::Default, sf::State::Fullscreen);

    window.setFramerateLimit(Config::maxFrameRate);
    //window.setKeyRepeatEnabled(false);

    //CellGrid cellGrid(window.getSize(), Config::cellSize, Config::dt);
    CellGrid cellGrid(window.getSize(), Config::cellSize, Config::dt, false); //start with all dead cell
    
    sf::Clock clock;

    std::vector<float> fpsVector;
    fpsVector.reserve(10e4);

    while (window.isOpen())
    {
        processEvents(window, cellGrid);
       
        sf::Time elapsed = clock.restart();

        float fps{ 1.0f / elapsed.asSeconds() };
        if (fps <= Config::maxFrameRate)
            fpsVector.push_back(fps);

        if (fpsVector.size() > 50) fpsVector.clear();

        std::cout << Utility::average(fpsVector) << "  -----------  " << fps << "\n";

        //cellGrid.update(elapsed);

        window.clear();
        
        window.draw(cellGrid);

        window.display();
    }
}
