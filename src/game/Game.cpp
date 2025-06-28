#include "Game.hpp"
#include <random>
#include <iostream>

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
}

void Game::setup_level() {
    grid.generate_maze_dfs(1, 1);
    std::mt19937 mt{ std::random_device{}() };

    // Jugador
    auto playerPos = grid.find_empty_cell(mt);
    player.set_position_grid(playerPos.y, playerPos.x);
    player.set_start_position(playerPos.y, playerPos.x);

    // Meta
    auto exitPos = grid.find_empty_cell(mt, playerPos);
    grid.set_cell(exitPos.y, exitPos.x, CellType::Goal);
    grid.set_goal_pos(exitPos);

    // Caja y placa
    auto boxPos = grid.find_empty_cell(mt, playerPos);
    auto platePos = grid.find_empty_cell(mt, playerPos);
    grid.set_cell(boxPos.y, boxPos.x, CellType::Box);
    grid.set_cell(platePos.y, platePos.x, CellType::Plate);

    // Puerta ligada a placa
    auto doorPos = grid.find_empty_cell(mt, playerPos);
    grid.set_cell(doorPos.y, doorPos.x, CellType::DoorClosed);
    grid.link_plate_to_door(platePos, doorPos);

    // Switchable walls
    for (int i = 0; i < 5; ++i) {
        auto sw = grid.find_empty_cell(mt, playerPos);
        grid.set_cell(sw.y, sw.x, CellType::Switch);
    }
}

void Game::run() {
    while (window.isOpen()) {
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
                auto world = window.mapPixelToCoords({ mb->position.x, mb->position.y });
                int c = static_cast<int>(world.x / grid.getCellSize());
                int r = static_cast<int>(world.y / grid.getCellSize());
                if (!grid.is_inside(r, c)) continue;

                // 1) Mover jugador
                if (player.try_move_to(r, c, grid)) {
                    next_turn();
                    continue;
                }

                // 2) Interactuar con caja
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
    // Trampas
    int r = player.get_row(), c = player.get_col();
    if (grid.get_cell(r, c) == CellType::Trap) {
        player.reset_to_start();
    }

    // Placas de presión cada turno
    grid.update_plates();

    // Meta alcanzada
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

void Game::render() {
    window.clear({ 30,30,40 });
    grid.draw(window);
    player.draw(window);
    if (showGoalMessage && goalText) {
        window.draw(*goalText);
    }
    window.display();
}
