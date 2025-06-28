#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <optional>
#include "../grid/Grid.hpp"
#include "Player.hpp"

class Game {
public:
    Game();
    void run();

private:
    // --- Miembros del juego ---
    sf::RenderWindow    window;
    Grid                grid;
    Player              player;
    sf::Font            font;
    std::optional<sf::Text> goalText;
    bool                showGoalMessage = false;

    // --- Lógica de Turnos (NUEVO) ---
    int turnCounter = 0;
    static const int SWITCH_WALL_INTERVAL = 5;
    static const int GOAL_MOVE_INTERVAL = 10;

    // --- Bucle principal ---
    void processEvents();
    void update();
    void render();

    // --- Nivel y turnos ---
    void setup_level();
    void next_turn();
};
