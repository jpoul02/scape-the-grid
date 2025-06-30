#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <random>

// Tipos de celda
enum class CellType {
    Empty,
    Wall,
    Goal,
    Trap,
    Switch,
    Plate,
    PlateOn,
    DoorClosed,
    DoorOpen,
    Box
};

struct Vector2iCompare {
    bool operator()(const sf::Vector2i& a, const sf::Vector2i& b) const {
        if (a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    }
};

class Grid {
    std::map<CellType, sf::Sprite>  sprites;

public:
    Grid(int rows, int cols, float cellSize);

    // Dibujo
    void draw(sf::RenderWindow& window, const std::map<CellType, sf::Texture>& textures);

    // Acceso
    int  getRows() const;
    int  getCols() const;
    float getCellSize() const;
    bool is_inside(int r, int c) const;
    CellType get_cell(int r, int c) const;
    void set_cell(int r, int c, CellType t);

    // Generación y búsqueda
    void generate_maze_dfs(int sr = 1, int sc = 1);
    bool has_path(int sr, int sc, int tr, int tc) const;
    sf::Vector2i find_empty_cell(std::mt19937& rng, sf::Vector2i exclude = { -1,-1 });

    // Mecánicas
    void update_plates();
    void toggle_switch_walls();
    void move_goal(std::mt19937& rng);
    void set_goal_pos(const sf::Vector2i& pos);
    void link_plate_to_door(const sf::Vector2i& platePos, const sf::Vector2i& doorPos);
    bool are_switches_active() const;
    sf::Vector2i get_goal_pos() const;

private:
    int rows, cols;
    float cellSize;
    std::vector<std::vector<CellType>> cells;

    bool switch_walls_active = true;
    sf::Vector2i goal_pos;
    std::map<sf::Vector2i, sf::Vector2i, Vector2iCompare> plate_door_links;
};
