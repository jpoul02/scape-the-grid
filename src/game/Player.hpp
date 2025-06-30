#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <optional>

class Grid;

class Player {
    int row, col;
    int startRow, startCol;
    float step;
    sf::Vector2f position;
    bool carrying_box = false;

    std::array<sf::Texture, 4> textures; // [0,1] = idle | [2,3] = carry
    std::optional<sf::Sprite> sprite;
    std::size_t currentFrame = 0;

    sf::Clock animationClock;
    const float frameDuration = 1.f / 6.f; // 6 FPS

public:
    Player(const std::string& idle0, const std::string& idle1,
        const std::string& carry0, const std::string& carry1,
        int r, int c, float cellSize);

    void draw(sf::RenderWindow& window);
    void updateAnimation();

    bool try_move_to(int r, int c, const Grid& grid);
    bool move(int dr, int dc, const Grid& grid);

    void set_position_grid(int r, int c);
    void set_start_position(int r, int c);
    void reset_to_start();

    int get_row() const;
    int get_col() const;

    void pick_box();
    void drop_box();
    bool is_carrying_box() const;
};
