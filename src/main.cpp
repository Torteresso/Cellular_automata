#include <SFML/Graphics.hpp>
#include <iostream>
#include "events.h"
#include "configuration.h"

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({Config::windowSize.x, Config::windowSize.y}), "CMake SFML Project", 
                                   sf::Style::Default, sf::State::Fullscreen);

    window.setFramerateLimit(Config::maxFrameRate);

    while (window.isOpen())
    {
        processEvents(window);
       
        window.clear();
        window.display();
    }
}
