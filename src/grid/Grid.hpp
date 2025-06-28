#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Grid {
public:
    Grid(unsigned int rows, unsigned int cols, float cellSize);
    void draw(sf::RenderWindow& window) const;

private:
    std::vector<sf::RectangleShape> cells;
};
