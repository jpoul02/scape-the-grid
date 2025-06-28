#include "Player.hpp"
#include "../grid/Grid.hpp"

Player::Player(const sf::Color& color, sf::Vector2f position, float size) {  
    shape.setSize(sf::Vector2f(size, size));  
    shape.setFillColor(color);  
    shape.setPosition(position);  
}  

void Player::draw(sf::RenderWindow& window) const {  
    window.draw(shape);  
}  

bool Player::move(int dx, int dy, float step, const Grid& grid) {  
    int newRow = row + dy;  
    int newCol = col + dx;  

    if (!grid.isInside(newRow, newCol)) return false;  
    if (grid.getCell(newRow, newCol) == CellType::Wall) return false;  

    row = newRow;  
    col = newCol;  
    shape.setPosition(sf::Vector2f(col * step + 1, row * step + 1)); 
    return true;  
}  

int Player::getRow() const { return row; }  
int Player::getCol() const { return col; }
