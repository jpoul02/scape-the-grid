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

    // Para el solver
    std::vector<sf::Vector2i> solution;
    size_t solveIndex = 0;
    bool autoSolve = false;

    // Botón Solver
    sf::RectangleShape solveButton;
    std::optional<sf::Text>          solveText;

    std::vector<sf::Vector2i> findFullPath(
        const sf::Vector2i& start,
        const sf::Vector2i& goal
    );

    std::mt19937 rng;

    void solveGame();
};
