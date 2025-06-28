#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <optional>

int main() {
    sf::RenderWindow window;
    sf::VideoMode mode(sf::Vector2u(800, 600), 32);
    window.create(mode, "Escape the Grid!");

    while (window.isOpen()) {
        std::optional<sf::Event> ev;
        while ((ev = window.pollEvent())) {
            if (ev->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::Black);
        window.display();
    }

    return 0;
}
