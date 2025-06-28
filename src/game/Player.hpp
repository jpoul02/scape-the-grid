#pragma once

#include <SFML/Graphics.hpp>
class Grid;  // ✅ Adelantamos la declaración de Grid (forward declaration)

class Player {
public:
    Player(const sf::Color& color, sf::Vector2f position, float size);

    void draw(sf::RenderWindow& window) const;

    bool move(int dx, int dy, float step, const Grid& grid);  // ✅ Ahora Grid está declarado

    int getRow() const;
    int getCol() const;

private:
    sf::RectangleShape shape;
    int row = 0;
    int col = 0;
};
