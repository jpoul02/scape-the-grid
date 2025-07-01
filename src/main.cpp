#include "game/Game.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char** argv) {
#ifdef _WIN32
    AllocConsole();
    FILE* unused;
    freopen_s(&unused, "CONOUT$", "w", stdout);
    freopen_s(&unused, "CONOUT$", "w", stderr);
#endif

    std::string mapFile;
    if (argc > 1)
        mapFile = argv[1];

    Game game(mapFile);
    game.run();
    return 0;
}
