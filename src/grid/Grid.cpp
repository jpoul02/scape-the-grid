#include "Grid.hpp"

Grid::Grid(int rows, int cols, float cellSize)
    : rows(rows), cols(cols), cellSize(cellSize), cells(rows, std::vector<CellType>(cols, CellType::Empty)) {
}

void Grid::draw(sf::RenderWindow& window) const {  
    sf::RectangleShape shape(sf::Vector2f(cellSize, cellSize));  
    for (int r = 0; r < rows; ++r) {  
        for (int c = 0; c < cols; ++c) {  
            shape.setPosition(sf::Vector2f(c * cellSize, r * cellSize)); 
            switch (cells[r][c]) {  
            case CellType::Empty: shape.setFillColor(sf::Color(50, 50, 50)); break;  
            case CellType::Wall: shape.setFillColor(sf::Color(100, 100, 100)); break;  
            }  
            window.draw(shape);  
        }  
    }  
}

CellType Grid::getCell(int row, int col) const {
    if (!isInside(row, col)) return CellType::Wall;
    return cells[row][col];
}

void Grid::setCell(int row, int col, CellType type) {
    if (isInside(row, col)) {
        cells[row][col] = type;
    }
}

bool Grid::isInside(int row, int col) const {
    return row >= 0 && col >= 0 && row < rows && col < cols;
}
