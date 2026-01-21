#include <string>
#include "Game.h"

Game* game = nullptr;

int main(int argc, char** argv)
{
    // Fullscreen mode
    bool fullscreen = false;
    int FPS = 24;

    // Initialize the game
    game = new Game();
    game->init("ItalianBriscolaSDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, fullscreen);

    const int frameDelay = 1000 / FPS;
    Uint32 frameStart, frameTime;
    Uint32 frameCount = 0;
    Uint32 fpsTimer = SDL_GetTicks(); // Initialize the timer with current ticks

    // Game loop
    while (game->isRunning()) {
        frameStart = SDL_GetTicks();

        // Handle events, update game state, and render the frame
        game->handleEvents();
        game->update();
        game->render();

        frameCount++;

        // Calculate and display FPS every second
        if (SDL_GetTicks() - fpsTimer >= 1000) {
            std::string windowTitle = "ItalianBriscolaSDL - FPS: " + std::to_string(frameCount);
            SDL_SetWindowTitle(game->getWindow(), windowTitle.c_str());

            frameCount = 0;
            fpsTimer = SDL_GetTicks(); // Reset the timer
        }

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    game->clean();

    game = nullptr;
    return 0;
} 