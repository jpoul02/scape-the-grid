#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

enum class CellType {
    Empty,
    Wall,
    // futuros: PressurePlate, SwitchableWall, Exit, etc.
};

class Grid {
public:
    Grid(int rows, int cols, float cellSize);

    void draw(sf::RenderWindow& window) const;
    CellType getCell(int row, int col) const;
    void setCell(int row, int col, CellType type);
    bool isInside(int row, int col) const;

    int getRows() const { return rows; }
    int getCols() const { return cols; }
    float getCellSize() const { return cellSize; }

private:
    int rows, cols;
    float cellSize;
    std::vector<std::vector<CellType>> cells;
};
