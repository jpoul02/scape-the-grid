#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include "../grid/Grid.hpp"
#include "Player.hpp"

class Game {
public:
    Game();
    void run();

private:
    sf::RenderWindow window;
    Grid grid;
    Player player;

    void processEvents();
    void update();
    void render();
};
