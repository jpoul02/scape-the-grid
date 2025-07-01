#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <fstream>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>                       // para std::string
#include <map>                          // para std::map
#include <random>                       // para std::mt19937
#include "../grid/Grid.hpp"
#include "Player.hpp"

enum class GameState {
    StartScreen,
    Playing
};

class Game {
    // — Tus texturas y estado de pantalla de inicio —
    std::map<CellType, sf::Texture> textures;
    GameState state = GameState::StartScreen;

public:
    // --- CONSTRUCTOR: si le pasas ruta, intentará cargar mapa, si no, lo genera ---
    explicit Game(const std::string& mapFile = "");
    void run();
    void load_textures();

private:
    // --- Carga de mapa externo ---
    bool mapLoaded = false;                 // marca si loadMap() tuvo éxito
    bool loadMap(const std::string& path);  // lee .txt/.json y rellena grid.cells
    bool saveMap(const std::string& path);

    // --- Pantalla de inicio ---
    sf::RectangleShape startButton;
    std::optional<sf::Text> startText;

    // --- Miembros del juego ---
    sf::RenderWindow    window;
    Grid                grid;
    Player              player;
    sf::Font            font;
    std::optional<sf::Text> goalText;
    bool                showGoalMessage = false;

    // --- Lógica de turnos ---
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

    // --- Solver ---
    std::vector<sf::Vector2i> solution;
    size_t solveIndex = 0;
    bool autoSolve = false;

    // --- Botón “Resolver” ---
    sf::RectangleShape solveButton;
    std::optional<sf::Text> solveText;

    // --- Botón “Exportar Mapa” ---
    sf::RectangleShape exportButton;
    std::optional<sf::Text> exportText;

    // --- Pathfinders ---
    std::vector<sf::Vector2i> findFullPath(
        const sf::Vector2i& start,
        const sf::Vector2i& goal
    );
    std::vector<sf::Vector2i> findPathWithoutPickup(
        const sf::Vector2i& start,
        const sf::Vector2i& goal
    );

    std::mt19937 rng;  // para aleatoriedad

    void solveGame();
};
