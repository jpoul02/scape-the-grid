#pragma once
#include <SFML/Graphics.hpp>

class Grid; // forward

class Player {
public:
    Player(const sf::Color& color, int row, int col, float cellSize);

    void draw(sf::RenderWindow& window) const;

    bool try_move_to(int targetRow, int targetCol, const Grid& grid);
    void set_position_grid(int r, int c);
    void set_start_position(int r, int c);
    void reset_to_start();
    int  get_row() const;
    int  get_col() const;

    // Objetos pesados
    void pick_box();
    void drop_box();
    bool is_carrying_box() const;

private:
    bool move(int dr, int dc, const Grid& grid);

    sf::Color originalColor;

    sf::RectangleShape shape;
    sf::Vector2f      position;
    float             step;

    int row, col;
    int startRow, startCol;

    bool carrying_box = false;
};
