#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "TextRenderer.h"
#include "TextRendererTimer.h"
#include "Timer.h"
#include "Briscola.h"

class AssetManager;

class Game
{
public:
    Game();
    ~Game();

    void loadAssets();

    void handleEvents();
    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void update();
    void render();

    void clean();

    bool isRunning();

    SDL_Window* getWindow();

    //static SDL_Renderer* renderer;
    //static bool isRunning;
    static AssetManager* assets;

private:
    void startANewGame();
    void setFirstHand();

    int cpuAttackLogic();
    size_t getSoprataglio(const std::vector<Card> &mano, Card c, bool maggiore);
    int cpuDefenseLogic();

    void renderWinMessage();
    
    // Game logic object
    Briscola briscola;

    PlayerDrawer playerDrawer;
    PlayerDrawer cpuDrawer;

    PlayerDrawer briscolaDrawer;

    // Gameplay state flags
    bool playerTurn;
    int roundNo;

    // Timers
    Timer playedTurnTimer;
    bool endTurnExecuted;
    Timer winMessageTimer;

    bool running;
    SDL_Texture* bgTex;
    SDL_Texture* tableTex;
    SDL_Texture* cardTex;
    TextRenderer textRenFS20;
    TextRendererTimer textRenFS50;
    SDL_FRect winRect;
    SDL_FRect bgImageRect;
    SDL_FRect srcTableRect;

    SDL_FRect messageRect;

    // Colors used for TextRenderer objects (messages)
	SDL_Color white = { 230, 230 ,230 };
	SDL_Color lightGold = { 255, 236, 132 };
	SDL_Color lightBlue = { 150, 140, 234 };
	SDL_Color grey = { 71, 71, 71 };

    // TextRenderer (messages) variables
	int xTextOffset = 15;
	int yTextOffset = 10;
	int textX = 0;
	int textY = 0;

    SDL_Event event;

    SDL_Window* window;
    SDL_Renderer* renderer;
};