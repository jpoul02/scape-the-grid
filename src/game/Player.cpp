#include "Player.hpp"

Player::Player(const sf::Color& color, sf::Vector2f position, float size) {
    shape.setSize(sf::Vector2f(size, size));
    shape.setFillColor(color);
    shape.setPosition(position);
}

void Player::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

void Player::move(int dx, int dy, float step) {
    shape.move(sf::Vector2f(dx * step, dy * step)); 
}
