#include "Game.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>

Game::Game()
    : window(), grid(10, 10, 60.f), player(sf::Color::Red, { 0, 0 }, 58.f)
{
    sf::VideoMode mode(sf::Vector2u(600, 600), 32);
    window.create(mode, "Escape the Grid");
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    std::optional<sf::Event> ev;
    while ((ev = window.pollEvent())) {
        if (ev->is<sf::Event::Closed>()) {
            window.close();
        }

        if (ev->is<sf::Event::KeyPressed>()) {
            const auto* keyEvent = ev->getIf<sf::Event::KeyPressed>();
            if (keyEvent) {
                switch (keyEvent->scancode) {
                case sf::Keyboard::Scan::W: player.move(0, -1, 60.f); break;
                case sf::Keyboard::Scan::S: player.move(0, 1, 60.f); break;
                case sf::Keyboard::Scan::A: player.move(-1, 0, 60.f); break;
                case sf::Keyboard::Scan::D: player.move(1, 0, 60.f); break;
                default: break;
                }
            }
        }
    }
}



void Game::update() {
    // lógica adicional más adelante
}

void Game::render() {
    window.clear(sf::Color::Black);
    grid.draw(window);
    player.draw(window);
    window.display();
}
