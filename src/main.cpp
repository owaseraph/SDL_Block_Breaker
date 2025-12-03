#include <game.hpp>

Game* game = nullptr;

int main(int argc, char* argv[]) {
    
    //FPS
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    
    //Ticks
    Uint64 frameStart;
    Uint64 frameTime;

    game = new Game();
    //initializing
    game->init("SDL3 Block Breaker", SCREEN_WIDTH, SCREEN_HEIGHT, false);

    while (game->running()) {
        frameStart = SDL_GetTicks();

        game->handleEvents();
        game->update();
        game->render();

        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    delete game;
    return 0;
}