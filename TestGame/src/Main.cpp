#include "Game.h"

int main() {
    const auto game = std::make_unique<Game>();
    game->Run();

    return 0;
}