#include "Grid.hpp"

Grid::Grid(unsigned int rows, unsigned int cols, float cellSize) {
    for (unsigned int r = 0; r < rows; ++r) {
        for (unsigned int c = 0; c < cols; ++c) {
            sf::RectangleShape cell(sf::Vector2f(cellSize - 1, cellSize - 1));
            cell.setPosition(sf::Vector2f(c * cellSize, r * cellSize)); // Fix: Use sf::Vector2f constructor
            cell.setFillColor(sf::Color::Black);
            cell.setOutlineThickness(1);
            cell.setOutlineColor(sf::Color::White);
            cells.push_back(cell);
        }
    }
}

void Grid::draw(sf::RenderWindow& window) const {
    for (const auto& cell : cells)
        window.draw(cell);
}
