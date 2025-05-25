#include "Game.h"

int main(int argc, char* argv[]) {
    Game game("Arkanoid", 800, 600);

    if (game.init()) {
        game.run();
    }

    game.close();

    return 0;
}