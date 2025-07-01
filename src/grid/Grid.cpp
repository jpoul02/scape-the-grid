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

void Grid::draw(sf::RenderWindow& w, const std::map<CellType, sf::Texture>& textures) {
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            CellType type = cells[r][c];
            auto it = textures.find(type);

            if (it != textures.end()) {
                sf::Sprite sprite(it->second);

                sprite.setTexture(it->second);

                // Escalado correcto usando Vector2f
                float texW = static_cast<float>(it->second.getSize().x);
                float texH = static_cast<float>(it->second.getSize().y);
                sprite.setScale(sf::Vector2f(cellSize / texW, cellSize / texH));

                // Posición correcta usando Vector2f
                sprite.setPosition(sf::Vector2f(c * cellSize, r * cellSize));

                w.draw(sprite);
            }
            else {
                // fallback si no hay textura
                sf::RectangleShape shape({ cellSize - 1.f, cellSize - 1.f });
                shape.setPosition({ c * cellSize + 1.f, r * cellSize + 1.f });
                shape.setFillColor(sf::Color::Magenta);
                w.draw(shape);
            }
        }
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

//bool Grid::has_path(int sr, int sc, int tr, int tc) const {
//    if (!is_inside(sr, sc) || !is_inside(tr, tc)) return false;
//    std::vector vis(rows, std::vector<bool>(cols, false));
//    std::queue<std::pair<int, int>> q;
//    q.push({ sr,sc }); vis[sr][sc] = true;
//    int dr[4] = { 1,-1,0,0 }, dc[4] = { 0,0,1,-1 };
//    while (!q.empty()) {
//        auto [r, c] = q.front(); q.pop();
//        if (r == tr && c == tc) return true;
//        for (int i = 0; i < 4; ++i) {
//            int nr = r + dr[i], nc = c + dc[i];
//            if (is_inside(nr, nc) && !vis[nr][nc] && get_cell(nr, nc) != CellType::Wall) {
//                vis[nr][nc] = true;
//                q.push({ nr,nc });
//            }
//        }
//    }
//    return false;
//}

bool Grid::has_path(int sr, int sc, int tr, int tc) const {
    if (!is_inside(sr, sc) || !is_inside(tr, tc)) return false;

    std::vector<std::vector<bool>> vis(rows, std::vector<bool>(cols, false));
    std::queue<std::pair<int, int>> q;
    q.push({ sr, sc });
    vis[sr][sc] = true;

    int dr[4] = { 1, -1, 0, 0 };
    int dc[4] = { 0, 0, 1, -1 };

    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();
        if (r == tr && c == tc) return true;

        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            if (!is_inside(nr, nc) || vis[nr][nc]) continue;

            CellType t = get_cell(nr, nc);
            bool passable =
                t == CellType::Empty ||
                t == CellType::Goal ||
                t == CellType::Plate ||
                t == CellType::PlateOn ||
                t == CellType::DoorOpen;

            if (passable) {
                vis[nr][nc] = true;
                q.push({ nr, nc });
            }
        }
    }

    return false;
}

std::vector<std::pair<sf::Vector2i, sf::Vector2i>>
Grid::getPlateDoorLinks() const {
    std::vector<std::pair<sf::Vector2i, sf::Vector2i>> v;
    for (auto const& [platePos, doorPos] : plate_door_links) {
        v.emplace_back(platePos, doorPos);
    }
    return v;
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
        CellType under = get_cell(pp.y, pp.x);
        bool active = (under == CellType::Box);
        // Si hay caja, la dejamos ahí y abrimos la puerta
        if (active) {
            set_cell(dp.y, dp.x, CellType::DoorOpen);
        }
        // Si no hay caja, restauramos la placa y cerramos la puerta
        else {
            set_cell(pp.y, pp.x, CellType::Plate);
            set_cell(dp.y, dp.x, CellType::DoorClosed);
        }
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

sf::Vector2i Grid::get_goal_pos() const {
    return goal_pos;
}
