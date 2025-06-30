#include "Player.hpp"
#include "../grid/Grid.hpp"
#include <iostream>

Player::Player(const std::string& idle0, const std::string& idle1,
    const std::string& carry0, const std::string& carry1,
    int r, int c, float cellSize)
    : row(r), col(c), startRow(r), startCol(c), step(cellSize)
{
    if (!textures[0].loadFromFile(idle0))  std::cerr << "Error cargando " << idle0 << '\n';
    if (!textures[1].loadFromFile(idle1))  std::cerr << "Error cargando " << idle1 << '\n';
    if (!textures[2].loadFromFile(carry0)) std::cerr << "Error cargando " << carry0 << '\n';
    if (!textures[3].loadFromFile(carry1)) std::cerr << "Error cargando " << carry1 << '\n';

    position = { col * step + step * 0.1f, row * step + step * 0.1f };

    sprite.emplace(textures[0]);
    sprite->setScale({ cellSize / textures[0].getSize().x,
        cellSize / textures[0].getSize().y });
    sprite->setPosition(position);
}

void Player::draw(sf::RenderWindow& window) {
    updateAnimation();
    if (sprite) window.draw(*sprite);
}

void Player::updateAnimation() {
    if (animationClock.getElapsedTime().asSeconds() >= frameDuration) {
        currentFrame = (currentFrame + 1) % 2;
        std::size_t offset = carrying_box ? 2 : 0;
        if (sprite) sprite->setTexture(textures[offset + currentFrame]);
        animationClock.restart();
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
        t == CellType::DoorClosed ||
        t == CellType::Box ||
        (t == CellType::Switch && grid.are_switches_active());

    if (wall)
        return false;

    row = nr; col = nc;
    position = { col * step + step * 0.1f, row * step + step * 0.1f };
    if (sprite) sprite->setPosition(position);
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
    if (sprite) sprite->setPosition(position);
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
    currentFrame = 0;
    animationClock.restart();
}

void Player::drop_box() {
    carrying_box = false;
    currentFrame = 0;
    animationClock.restart();
}

bool Player::is_carrying_box() const {
    return carrying_box;
}
