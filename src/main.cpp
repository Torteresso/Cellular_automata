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

    CellGrid cellGrid(window.getSize(), Config::cellSize, Config::dt);
    //CellGrid cellGrid(window.getSize(), Config::cellSize, Config::dt, false); //start with all dead cell
    
    sf::Clock clock;

    bool show_fps{ false };
	std::vector<float> fpsVector;

    sf::View view = window.getDefaultView();

    if (show_fps)
    {
        fpsVector.reserve(10e4);
    }

    while (window.isOpen())
    {
        Event::processEvents(window, cellGrid, view);
        Event::moveView(window, view);

        sf::Time elapsed = clock.restart();

        if (show_fps)
        {
            float fps{ 1.0f / elapsed.asSeconds() };
            if (fps <= Config::maxFrameRate)
                fpsVector.push_back(fps);

            if (fpsVector.size() > 50) fpsVector.clear();

            std::cout << Utility::average(fpsVector) << "  -----------  " << fps << "\n";
        }

        //cellGrid.update(elapsed);

        window.clear();

        window.setView(view);

        window.draw(cellGrid);

        window.display();
    }
}
