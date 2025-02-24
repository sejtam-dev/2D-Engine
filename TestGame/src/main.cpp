#include "game.h"

int main() {
    const std::unique_ptr<Game> game(new Game());
    game->Run();

    return 0;
}