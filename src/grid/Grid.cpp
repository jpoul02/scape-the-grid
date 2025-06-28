#include "Grid.hpp"
#include <algorithm>
#include <queue>
#include <iostream>

Grid::Grid(int r, int c, float cs)
    : rows(r), cols(c), cellSize(cs)
    , cells(r, std::vector<CellType>(c, CellType::Wall))
{
}

int    Grid::getRows()      const { return rows; }
int    Grid::getCols()      const { return cols; }
float  Grid::getCellSize()  const { return cellSize; }
bool   Grid::is_inside(int r, int c) const {
    return r >= 0 && c >= 0 && r < rows && c < cols;
}
CellType Grid::get_cell(int r, int c) const {
    return is_inside(r, c) ? cells[r][c] : CellType::Wall;
}
void Grid::set_cell(int r, int c, CellType t) {
    if (is_inside(r, c)) cells[r][c] = t;
}

void Grid::draw(sf::RenderWindow& w) const {
    sf::RectangleShape shape({ cellSize - 1.f,cellSize - 1.f });
    for (int r = 0; r < rows; ++r) for (int c = 0; c < cols; ++c) {
        // --- DESPUÉS ---
        shape.setPosition({ c * cellSize + 1.f,
                            r * cellSize + 1.f });

        switch (cells[r][c]) {
        case CellType::Empty:   shape.setFillColor({ 200,200,200 }); break;
        case CellType::Wall:    shape.setFillColor({ 100,100,100 }); break;
        case CellType::Goal:    shape.setFillColor(sf::Color::Green); break;
        case CellType::Trap:    shape.setFillColor({ 255,100,100 }); break;
        case CellType::Box:     shape.setFillColor({ 139,69,19 }); break;
        case CellType::Plate:   shape.setFillColor({ 0,100,100 }); break;
        case CellType::PlateOn: shape.setFillColor(sf::Color::Cyan); break;
        case CellType::DoorClosed: shape.setFillColor({ 128,0,0 }); break;
        case CellType::DoorOpen:   shape.setFillColor({ 120,80,40 }); break;
        case CellType::Switch:
            shape.setFillColor(switch_walls_active
                ? sf::Color::Yellow
                : sf::Color(100, 100, 0));
            break;
        default: shape.setFillColor(sf::Color::Magenta); break;
        }
        w.draw(shape);
    }
}

void Grid::generate_maze_dfs(int sr, int sc) {
    std::vector<std::pair<int, int>> dirs{ {-2,0},{2,0},{0,-2},{0,2} };
    auto dfs = [&](auto&& self, int r, int c)->void {
        cells[r][c] = CellType::Empty;
        std::shuffle(dirs.begin(), dirs.end(), std::mt19937{ std::random_device{}() });
        for (auto [dr, dc] : dirs) {
            int nr = r + dr, nc = c + dc, wr = r + dr / 2, wc = c + dc / 2;
            if (is_inside(nr, nc) && cells[nr][nc] == CellType::Wall) {
                cells[wr][wc] = CellType::Empty;
                self(self, nr, nc);
            }
        }
        };
    for (auto& row : cells) for (auto& c : row) c = CellType::Wall;
    dfs(dfs, sr, sc);
}

bool Grid::has_path(int sr, int sc, int tr, int tc) const {
    if (!is_inside(sr, sc) || !is_inside(tr, tc)) return false;
    std::vector vis(rows, std::vector<bool>(cols, false));
    std::queue<std::pair<int, int>> q;
    q.push({ sr,sc }); vis[sr][sc] = true;
    int dr[4] = { 1,-1,0,0 }, dc[4] = { 0,0,1,-1 };
    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();
        if (r == tr && c == tc) return true;
        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i], nc = c + dc[i];
            if (is_inside(nr, nc) && !vis[nr][nc] && get_cell(nr, nc) != CellType::Wall) {
                vis[nr][nc] = true;
                q.push({ nr,nc });
            }
        }
    }
    return false;
}

sf::Vector2i Grid::find_empty_cell(std::mt19937& rng, sf::Vector2i exclude) {
    std::uniform_int_distribution<int> dr(1, rows - 2), dc(1, cols - 2);
    sf::Vector2i p;
    do {
        p.y = dr(rng);
        p.x = dc(rng);
    } while (cells[p.y][p.x] != CellType::Empty || p == exclude);
    return p;
}

void Grid::link_plate_to_door(const sf::Vector2i& pp, const sf::Vector2i& dp) {
    plate_door_links[pp] = dp;
}

void Grid::update_plates() {
    for (auto& [pp, dp] : plate_door_links) {
        bool active = (get_cell(pp.y, pp.x) == CellType::Box);
        set_cell(pp.y, pp.x, active ? CellType::PlateOn : CellType::Plate);
        set_cell(dp.y, dp.x, active ? CellType::DoorOpen : CellType::DoorClosed);
    }
}

void Grid::toggle_switch_walls() {
    switch_walls_active = !switch_walls_active;
}

bool Grid::are_switches_active() const {
    return switch_walls_active;
}

void Grid::set_goal_pos(const sf::Vector2i& pos) {
    goal_pos = pos;
}

void Grid::move_goal(std::mt19937& rng) {
    set_cell(goal_pos.y, goal_pos.x, CellType::Empty);
    goal_pos = find_empty_cell(rng);
    set_cell(goal_pos.y, goal_pos.x, CellType::Goal);
}
