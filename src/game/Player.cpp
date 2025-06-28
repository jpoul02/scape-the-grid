#include "Player.hpp"
#include "../grid/Grid.hpp"

Player::Player(const sf::Color& color, int r, int c, float cellSize)
    : row(r), col(c)
    , startRow(r), startCol(c)
    , step(cellSize)
{
    position = { col * step + step * 0.1f, row * step + step * 0.1f };
    shape.setSize({ step * 0.8f, step * 0.8f });
	originalColor = color;
    shape.setFillColor(color);
    shape.setPosition(position);
}

void Player::draw(sf::RenderWindow& window) const {
    window.draw(shape);
    // --- DESPUÉS ---
    if (carrying_box) {
        sf::CircleShape indicator(step * 0.2f);
        // setOrigin con Vector2f
        indicator.setOrigin({ indicator.getRadius(), indicator.getRadius() });
        // setPosition con Vector2f
        indicator.setPosition({ position.x + step * 0.4f,
                                position.y + step * 0.4f });
        window.draw(indicator);
    }

}

bool Player::move(int dr, int dc, const Grid& grid) {
    int nr = row + dr, nc = col + dc;
    if (!grid.is_inside(nr, nc))
        return false;

    CellType t = grid.get_cell(nr, nc);

    if (t == CellType::DoorClosed && carrying_box) {
        const_cast<Grid&>(grid).set_cell(nr, nc, CellType::DoorOpen);
        t = CellType::DoorOpen;
    }

    bool wall =
        t == CellType::Wall ||
        t == CellType::DoorClosed ||  // ya no saltará aquí si llevaba la caja
        t == CellType::Box ||
        (t == CellType::Switch && grid.are_switches_active());

    if (wall)
        return false;

    row = nr; col = nc;
    position = { col * step + step * 0.1f, row * step + step * 0.1f };
    shape.setPosition(position);
    return true;
}

bool Player::try_move_to(int r, int c, const Grid& grid) {
    int dr = r - row, dc = c - col;
    if (std::abs(dr) + std::abs(dc) != 1) return false;
    return move(dr, dc, grid);
}

void Player::set_position_grid(int r, int c) {
    row = r; col = c;
    position = { col * step + step * 0.1f, row * step + step * 0.1f };
    shape.setPosition(position);
}

void Player::set_start_position(int r, int c) {
    startRow = r; startCol = c;
}

void Player::reset_to_start() {
    set_position_grid(startRow, startCol);
}

int Player::get_row() const { return row; }
int Player::get_col() const { return col; }

void Player::pick_box() {
    carrying_box = true;
    shape.setFillColor(sf::Color::Green);
}

void Player::drop_box() {
    carrying_box = false;
    shape.setFillColor(originalColor);
}

bool Player::is_carrying_box() const {
    return carrying_box;
}
