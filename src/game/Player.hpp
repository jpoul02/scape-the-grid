#pragma once
#include <SFML/Graphics.hpp>

class Player {
public:
    Player(const sf::Color& color, sf::Vector2f position, float size);
    void draw(sf::RenderWindow& window) const;
    void move(int dx, int dy, float step);

private:
    sf::RectangleShape shape;
};
