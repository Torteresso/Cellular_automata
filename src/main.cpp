#include <SFML/Graphics.hpp>
#include <iostream>
#include "events.h"
#include "cellGrid.h"
#include "utility.h"

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({Config::windowSize.x, Config::windowSize.y}), "CMake SFML Project", 
                                   sf::Style::Default, sf::State::Fullscreen);

    window.setFramerateLimit(Config::maxFrameRate);
    //window.setKeyRepeatEnabled(false);

    CellGrid cellGrid(window.getSize(), Config::cellSize, Config::dt);

    if (!Config::startAsRandomGrid) cellGrid = CellGrid(window.getSize(), Config::cellSize, Config::dt, false); //start with all dead cell
    
    sf::Clock clock;

	std::vector<float> fpsVector;

    sf::CircleShape cellSelectionCircle(Config::windowSize.x / 50);
    cellSelectionCircle.setPosition({ Config::windowSize.x / 100, Config::windowSize.y / 100 });
    cellSelectionCircle.setOutlineThickness(5.0f);
    cellSelectionCircle.setOutlineColor(sf::Color::White);

    Cell::Type cellSelection{ Cell::Type::alive };

    sf::View mainView = window.getDefaultView();
    sf::View editorView = window.getDefaultView();

    if (Config::showFps)
    {
        fpsVector.reserve(10e4);
    }

    while (window.isOpen())
    {
        Event::processEvents(window, cellGrid, mainView, cellSelection, cellSelectionCircle);
        Event::moveView(window, mainView);

        sf::Time elapsed = clock.restart();

        if (Config::showFps)
        {
            float fps{ 1.0f / elapsed.asSeconds() };
            if (fps <= Config::maxFrameRate)
                fpsVector.push_back(fps);

            if (fpsVector.size() > 50) fpsVector.clear();

            std::cout << Utility::average(fpsVector) << "  -----------  " << fps << "\n";
        }

        if (!Config::manualUpdateOnly) cellGrid.update(elapsed);

        window.clear();

        window.setView(mainView);
        window.draw(cellGrid);

        window.setView(editorView);
        window.draw(cellSelectionCircle);

        window.setView(mainView);

        window.display();
    }
}
