#include "Game.hpp"
#include <random>
#include <iostream>
#include <queue>
#include <map>

struct State {
    sf::Vector2i pos;
    bool carrying;

    bool operator<(State const& o) const {
        if (pos.x != o.pos.x)    return pos.x < o.pos.x;
        if (pos.y != o.pos.y)    return pos.y < o.pos.y;
        return carrying < o.carrying;
    }
    bool operator==(State const& o) const {
        return pos == o.pos && carrying == o.carrying;
    }
};


Game::Game()
    : window()
    , grid(21, 21, 32.f)
    , player(sf::Color::Blue, 1, 1, 32.f)
{
    unsigned int w = static_cast<unsigned int>(grid.getCols() * grid.getCellSize());
    unsigned int h = static_cast<unsigned int>(grid.getRows() * grid.getCellSize());
    window.create(
        sf::VideoMode({ static_cast<unsigned>(w), static_cast<unsigned>(h) }),
        "Ruinas Ancentrales",
        sf::State::Windowed
    );
    window.setFramerateLimit(60);

    if (!font.openFromFile("assets/OpenSans-Regular.ttf")) {
        std::cerr << "Error: No se pudo cargar la fuente 'assets/OpenSans-Regular.ttf'.\n";
    }

    solveText.emplace(font, "Resolver", 16u);
    solveText->setFillColor(sf::Color::White);
    solveText->setPosition({ 15.f, 12.f });

    goalText.emplace(font, "¡Meta alcanzada!", 40u);
    goalText->setFillColor(sf::Color::White);
    goalText->setStyle(sf::Text::Bold);
    auto& txt = *goalText;
    sf::FloatRect bounds = txt.getLocalBounds();
    txt.setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
        });
    txt.setPosition({ w / 2.f, h / 2.f });

    setup_level();

    solveButton.setSize({ 100.f, 30.f });
    solveButton.setFillColor({ 50,50,50 });
    solveButton.setPosition(sf::Vector2f(10.f, 10.f));


}

void Game::setup_level() {
    grid.generate_maze_dfs(1, 1);
    std::mt19937 mt{ std::random_device{}() };

    auto playerPos = grid.find_empty_cell(mt);
    player.set_position_grid(playerPos.y, playerPos.x);
    player.set_start_position(playerPos.y, playerPos.x);

    auto exitPos = grid.find_empty_cell(mt, playerPos);
    grid.set_cell(exitPos.y, exitPos.x, CellType::Goal);
    grid.set_goal_pos(exitPos);

    auto boxPos = grid.find_empty_cell(mt, playerPos);
    auto platePos = grid.find_empty_cell(mt, playerPos);
    grid.set_cell(boxPos.y, boxPos.x, CellType::Box);
    grid.set_cell(platePos.y, platePos.x, CellType::Plate);

    auto doorPos = grid.find_empty_cell(mt, playerPos);
    grid.set_cell(doorPos.y, doorPos.x, CellType::DoorClosed);
    grid.link_plate_to_door(platePos, doorPos);

    for (int i = 0; i < 5; ++i) {
        auto sw = grid.find_empty_cell(mt, playerPos);
        grid.set_cell(sw.y, sw.x, CellType::Switch);
    }
}

void Game::run() {
    while (window.isOpen()) {
        if (autoSolve && solveIndex < solution.size()) {
            auto step = solution[solveIndex++];
            if (step.x == -1 && step.y == -1) {
                player.pick_box();
                grid.set_cell(player.get_row(), player.get_col(), CellType::Empty);
            }
            else {
                player.try_move_to(step.y, step.x, grid);
            }
            next_turn();
        }

        processEvents();
        update();
        render();
    }
}


void Game::processEvents() {
    std::optional<sf::Event> ev;
    while ((ev = window.pollEvent())) {
        if (ev->is<sf::Event::Closed>()) {
            window.close();
        }
        else if (ev->is<sf::Event::MouseButtonPressed>()) {
            const auto* mb = ev->getIf<sf::Event::MouseButtonPressed>();
            if (mb->button == sf::Mouse::Button::Left) {
                sf::Vector2f world = window.mapPixelToCoords({ mb->position.x, mb->position.y });

                if (solveText && solveButton.getGlobalBounds().contains(world)) {
                    solveGame();
                    continue;
                }

                int c = static_cast<int>(world.x / grid.getCellSize());
                int r = static_cast<int>(world.y / grid.getCellSize());

                if (!grid.is_inside(r, c)) continue;

                if (player.try_move_to(r, c, grid)) {
                    next_turn();
                    continue;
                }

                int pr = player.get_row(), pc = player.get_col();
                int dr = r - pr, dc = c - pc;
                if (std::abs(dr) + std::abs(dc) == 1) {
                    auto cell = grid.get_cell(r, c);
                    if (!player.is_carrying_box() && cell == CellType::Box) {
                        player.pick_box();
                        grid.set_cell(r, c, CellType::Empty);
                        next_turn();
                    }
                    else if (player.is_carrying_box() &&
                        (cell == CellType::Empty || cell == CellType::Plate))
                    {
                        player.drop_box();
                        grid.set_cell(r, c, CellType::Box);
                        next_turn();
                    }
                }
            }
        }
    }
}


void Game::update() {
    int r = player.get_row(), c = player.get_col();
    if (grid.get_cell(r, c) == CellType::Trap) {
        player.reset_to_start();
    }

    grid.update_plates();

    if (grid.get_cell(r, c) == CellType::Goal && !showGoalMessage) {
        showGoalMessage = true;
    }
}

void Game::next_turn() {
    ++turnCounter;
    std::cout << "Turno: " << turnCounter << std::endl;

    if (turnCounter % SWITCH_WALL_INTERVAL == 0) {
        grid.toggle_switch_walls();
        std::cout << "→ Switch walls toggled\n";
    }
    if (turnCounter % GOAL_MOVE_INTERVAL == 0) {
        std::mt19937 rng{ std::random_device{}() };
        grid.move_goal(rng);
        std::cout << "→ Goal moved\n";
    }
}

std::vector<sf::Vector2i> Game::findFullPath(
    const sf::Vector2i& start,
    const sf::Vector2i& goal
) {
    std::queue<State> q;
    std::map<State, State>     prev;
    std::map<State, sf::Vector2i> how;

    State s0{ start, false };
    q.push(s0);
    prev[s0] = s0;

    int dr[4] = { 1,-1,0,0 }, dc[4] = { 0,0,1,-1 };
    while (!q.empty()) {
        auto [curPos, carry] = q.front(); q.pop();
        if (curPos == goal) {

            std::vector<sf::Vector2i> path;
            State v{ curPos, carry };
            while (!(v == s0)) {
                auto step = how[v];
                path.push_back(step);
                v = prev[v];
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        if (!carry && grid.get_cell(curPos.y, curPos.x) == CellType::Box) {
            State nxt{ curPos, true };
            if (!prev.count(nxt)) {
                prev[nxt] = { curPos, carry };
                how[nxt] = { -1,-1 };  
                q.push(nxt);
            }
        }

        for (int i = 0; i < 4; ++i) {
            sf::Vector2i np{ curPos.x + dc[i], curPos.y + dr[i] };
            if (!grid.is_inside(np.y, np.x)) continue;

            CellType t = grid.get_cell(np.y, np.x);
            bool pass =
                t == CellType::Empty ||
                t == CellType::Plate ||
                t == CellType::PlateOn ||
                t == CellType::Goal ||
                t == CellType::DoorOpen ||
                (t == CellType::DoorClosed && carry) ||
                (t == CellType::Switch && !grid.are_switches_active());
            if (!pass) continue;

            State nxt{ np, carry };
            if (!prev.count(nxt)) {
                prev[nxt] = { curPos, carry };
                how[nxt] = np;
                q.push(nxt);
            }
        }
    }

    return {}; 
}



void Game::solveGame() {
    solution.clear();
    solveIndex = 0;

    sf::Vector2i p{ player.get_col(), player.get_row() };

    sf::Vector2i boxPos{ -1,-1 };

    for (int r = 0; r < grid.getRows(); ++r)
        for (int c = 0; c < grid.getCols(); ++c)
            if (grid.get_cell(r, c) == CellType::Box)
                boxPos = { c,r };

    solution = findFullPath(
        { player.get_col(), player.get_row() },
        grid.get_goal_pos()
    );
    autoSolve = true;
    solveIndex = 0;


    std::cout << "Solver: hallados " << solution.size() << " pasos.\n";

    autoSolve = true;
}


void Game::render() {
    window.clear({ 30,30,40 });
    grid.draw(window);
    player.draw(window);
    if (showGoalMessage && goalText) {
        window.draw(*goalText);
    }
    window.draw(solveButton);
    if (solveText) {  
        window.draw(*solveText);  
    }
    window.display();
}
