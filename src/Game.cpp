#include "Game.h"
#include "AssetManager.h"
#include "TextureManager.h"
#include "ErrorMessage.h"

AssetManager* Game::assets = new AssetManager();
TextureManager* manager;

Game::Game()
{
    //assets = new AssetManager();
}

Game::~Game()
{
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flags = 0;
    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_VIDEO) == true)
    {
        // Creating game window
        window = SDL_CreateWindow(title, width, height, flags);

        if (window)
        {
            winRect.x = 0;
            winRect.y = 0;
            winRect.w = width;
            winRect.h = height;
        }

        // Creating SDL renderer
        renderer = SDL_CreateRenderer(window, NULL);
        if (renderer)
        {
            SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
        }
        

        running = true;

        loadAssets();

        bgTex = assets->getTexture("bg");
        if (bgTex)
        {
            bgImageRect.x = 0;
            bgImageRect.y = 0;
            bgImageRect.w = bgTex->w;
            bgImageRect.h = bgTex->h;
        }

        tableTex = assets->getTexture("ui");
        if (tableTex)
        {
            srcTableRect.x = 0;
			srcTableRect.y = 0;
            srcTableRect.w = tableTex->w;
            srcTableRect.h = tableTex->h;
        }

        SDL_Texture* deck = assets->getTexture("deck");
        // Player's & Cpu's hand drawers initialization 
		if (playerDrawer.init(deck, renderer, &briscola.player, 490, 440, 200, 280, 0))
        {
            std::cout << "[INFO] PlayerDrawer created." << std::endl;
        }
        if (cpuDrawer.init(deck, renderer, &briscola.cpu, 490, 50, 200, 280, 0))
        {
            std::cout << "[INFO] CpuDrawer created." << std::endl;
        }
        if (briscolaDrawer.initBriscola(deck, renderer, 50, 80, 200, 280))
        {
            std::cout << "[INFO] BriscolaDrawer created." << std::endl;   
        }

        /*
        cardTex = assets->getTexture("deck");
        if (cardTex)
        {
            srcTableRect.x = 0;
			srcTableRect.y = 0;
            srcTableRect.w = cardTex->w;
            srcTableRect.h = cardTex->h;
        }
        */
        
        if (textRenFS20.init("assets/font/STENCIL.TTF", 20, renderer))
        {
            std::cout << "[INFO] TextRenderer(FontSize 20) created." << std::endl;
        }

        if (textRenFS50.init("assets/font/STENCIL.TTF", 50, renderer))
        {
            std::cout << "[INFO] TextRenderer(FontSize 50) created." << std::endl;
        }

        // Create Briscola logic game
        briscola.createNewGame();
		briscola.player.init("Anonyms");

        briscola.p1DroppedCard.hidden = true;
        briscola.cpuDroppedCard.hidden = true;

        endTurnExecuted = false;

        startANewGame();

        playerTurn = true;
        briscola.lastRoundWinner = "P1";
        running = true;
    }
    else
    {
        running = false;
    }
}

void Game::loadAssets()
{
    //assets = new AssetManager();
    if (assets)
    {
        assets->addTexture("bg", "assets/img/table-bg.png", renderer);
        assets->addTexture("ui", "assets/img/table-ui.png", renderer);
        assets->addTexture("deck", "assets/img/deck.png", renderer);
        //assets->addFont("stencil", "assets/font/STENCIL.TTF", 20);
    }
    else
    {
        ErrorMessage::showSDlError("ERROR: assets is NULL !");
    }
}

void Game::handleEvents()
{
    SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_EVENT_QUIT:
		running = false;
		break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        if (playerTurn 
            && briscola.player.hand.size() > 0
            && briscola.player.hand.size() == briscola.cpu.hand.size())
        {
            briscola.handleEvent(&event);

            if (briscola.player.selCardIdx != -1)
            {
                std::cout << "Player selected card index = " << briscola.player.selCardIdx << std::endl;
                playerTurn = false;

                endTurnExecuted = false;

                briscola.p1DroppedCard = briscola.player.pickCardFromHand(briscola.player.selCardIdx); //briscola.player.selCardIdx);
                briscola.p1DroppedCard.hidden = false;

                SDL_Delay(200);

                // cpu plays
                int cpuChosenIdx = cpuDefenseLogic(); 
                briscola.cpuDroppedCard = briscola.cpu.pickCardFromHand(cpuChosenIdx);
                briscola.cpuDroppedCard.hidden = false;

                playedTurnTimer.setTimer(3000);
            }
        }
        else
        {
            if (!playerTurn 
                && briscola.player.hand.size() > briscola.cpu.hand.size()
                && briscola.player.hand.size() > 0)
            {
                briscola.handleEvent(&event);

                if (briscola.player.selCardIdx != -1)
                {
                    std::cout << "Player selected card index = " << briscola.player.selCardIdx << std::endl;

                    endTurnExecuted = false;

                    briscola.p1DroppedCard = briscola.player.pickCardFromHand(briscola.player.selCardIdx); //briscola.player.selCardIdx);
                    briscola.p1DroppedCard.hidden = false;

                    playedTurnTimer.setTimer(3000);
                }
            }
        }
        break;
	default:
		break;
	}
}

void Game::update()
{
    if (!briscola.p1DroppedCard.hidden && !briscola.cpuDroppedCard.hidden)
    {
        if (!endTurnExecuted && !playedTurnTimer.active())
        {
            // std::cout << "Timer elapsed." << std::endl;
            endTurnExecuted = true;

            briscola.p1DroppedCard.hidden = true;
            briscola.cpuDroppedCard.hidden = true;

            std::cout << "-- PLAYER played card: " << briscola.p1DroppedCard.getName() << " of " << briscola.p1DroppedCard.getSuit() << std::endl;
            std::cout << "-- CPU played card: " << briscola.cpuDroppedCard.getName() << " of " << briscola.cpuDroppedCard.getSuit() << std::endl;

            briscola.roundWinner(briscola.lastRoundWinner, briscola.p1DroppedCard, briscola.cpuDroppedCard, briscola.briscolaCard);
            if (briscola.lastRoundWinner == "P1")
            {
                // player won
                std::cout << "-- PLAYER won round #" << roundNo << std::endl;

                // Calculate points
                int actualScore = briscola.player.getScore();
                briscola.player.setScore(actualScore + briscola.p1DroppedCard.value + briscola.cpuDroppedCard.value);

                playerTurn = true;
            }
            else if (briscola.lastRoundWinner == "CPU")
            {
                // cpu won
                std::cout << "CPU won round #" << roundNo << std::endl;

                // Calculate points
                int actualScore = briscola.cpu.getScore();
                briscola.cpu.setScore(actualScore + briscola.p1DroppedCard.value + briscola.cpuDroppedCard.value);

                playerTurn = false;
            }

            std::cout << "-- PLAYER points = " << std::to_string(briscola.player.getScore());
            std::cout << ", CPU points = " << std::to_string(briscola.cpu.getScore()) << std::endl;

            if (roundNo < 18)
            {
                // players can draw from deck
                if (playerTurn)
                {
                    briscola.player.addCardToHand(briscola.deck.cards.back(), briscola.player.selCardIdx, 490, 440, 200, 280, 1, false);
                    briscola.deck.cards.pop_back();

                    briscola.cpu.addCardToHand(briscola.deck.cards.back(), 0, 490, 50, 200, 280, 1, true);
                    briscola.deck.cards.pop_back();
                }
                else
                {
                    briscola.cpu.addCardToHand(briscola.deck.cards.back(), 0, 490, 50, 200, 280, 1, true);
                    briscola.deck.cards.pop_back();

                    briscola.player.addCardToHand(briscola.deck.cards.back(), briscola.player.selCardIdx, 490, 440, 200, 280, 1, false);
                    briscola.deck.cards.pop_back();
                    
                }
                if (roundNo == 17)
                    briscola.briscolaCard.hidden = true;
            }
            else
            {
                //briscola.player.hand.size() < 3
                for (int i = 0; i < briscola.player.hand.size(); i++)
                {
                    Card c = briscola.player.hand.at(i);
                    c.mPosition.x = 490 + i * 200; 
                }
            }
            ++roundNo;

            //briscola.player.selCardIdx = -1;

            if (!playerTurn)
            {
                if (briscola.cpu.hand.size() > 0
                    && briscola.cpu.hand.size() == briscola.player.hand.size())
                {
                    // cpu plays first
                    int cpuChosenIdx = cpuAttackLogic();
                    briscola.cpuDroppedCard = briscola.cpu.pickCardFromHand(cpuChosenIdx);
                    briscola.cpuDroppedCard.hidden = false;

                    std::cout << "-- CPU played card: " << briscola.cpuDroppedCard.getName() << " of " << briscola.cpuDroppedCard.getSuit() << std::endl;
                }
            }
            //playerTurn = true;
        }
        
    }
    else
    {
        if (roundNo == 21 && endTurnExecuted)
        {
            textRenFS50.setTimer(1000 * 5);
            winMessageTimer.setTimer(5000);
            endTurnExecuted = false;
        }
        if (roundNo == 21 && !endTurnExecuted && !textRenFS50.active())
        {
            // Create Briscola logic game
            briscola.createNewGame();
		    briscola.player.init("Anonyms");

            briscola.p1DroppedCard.hidden = true;
            briscola.cpuDroppedCard.hidden = true;

            endTurnExecuted = false;

            startANewGame();

            playerTurn = true;
            briscola.lastRoundWinner = "P1";
        }
    }
}

void Game::render()
{
    SDL_RenderClear(renderer);

    SDL_RenderTexture(renderer, bgTex, &bgImageRect, &winRect);

    // Render player's & cpu's 'hand'
	playerDrawer.drawHand();
	cpuDrawer.drawHand();

    // Render briscola card (if not hidden)
    if (!briscola.briscolaCard.hidden)
        briscolaDrawer.drawBriscola(briscola.briscolaCard);

    // Render played cards for current round
    if (!briscola.p1DroppedCard.hidden)
        playerDrawer.drawDropped(briscola.p1DroppedCard, 70, 260);
    
    if (!briscola.cpuDroppedCard.hidden)
        cpuDrawer.drawDroppedCpu(briscola.cpuDroppedCard, 140, 260);

    SDL_RenderTexture(renderer, tableTex, &srcTableRect, &srcTableRect);

    //SDL_RenderTexture(renderer, cardTex, &srcTableRect, &srcTableRect);

    textRenFS20.renderText("CPU POINTS: " + std::to_string(briscola.cpu.getScore()), 20, 10, grey);
    textRenFS20.renderText("PLR POINTS: " + std::to_string(briscola.player.getScore()), 20, 35, grey);

    textRenFS20.renderText(std::to_string(briscola.deck.cards.size()), 30, 380, white);
    //textRenFS20.renderText("Round: " + std::to_string(roundNo), 60, 400, white);

    // Render win message
	if (winMessageTimer.active()) {
		renderWinMessage();
	}

    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    delete assets;

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

bool Game::isRunning()
{
    return running;
}

SDL_Window* Game::getWindow()
{
    return window;
}

void Game::startANewGame()
{
    // Create a new game, as on game startup
    briscola.createNewGame();
    playerDrawer.clearHand();
    cpuDrawer.clearHand();

    roundNo = 0;
    briscola.player.setScore(0);
    briscola.cpu.setScore(0);

    setFirstHand();
}

void Game::setFirstHand()
{
    // Give 3 cards to each
	if (briscola.player.hand.size() == 0 && briscola.cpu.hand.size() == 0)
    {
        //std::cout << "[INFO] Giving cards..." << std::endl;
        for (int i = 0; i < 3; i++)
        {
            briscola.player.addCardToHand(briscola.deck.cards.back(), i, 490, 440, 200, 280, 1, false);
            briscola.deck.cards.pop_back();

            briscola.cpu.addCardToHand(briscola.deck.cards.back(), i, 490, 50, 200, 280, 1, true);
            briscola.deck.cards.pop_back();
        }
        //std::cout << "[INFO] " << briscola.player.hand.size() << " cards given to player." << std::endl;
        //std::cout << "[INFO] " << briscola.cpu.hand.size() << " cards given to cpu." << std::endl;

        //briscola.cpu.hand.at(0).hidden = true;

        // Set briscola card
        briscola.briscolaCard = briscola.deck.cards.back();
        briscola.deck.cards.pop_back();
        briscola.briscolaCard.hidden = false;

        briscola.deck.cards.emplace(briscola.deck.cards.begin(), briscola.briscolaCard);

        ++roundNo;
    }
}

int Game::cpuAttackLogic()
{
    //int chose = 0;
    int i;
    int minpts = 12, minstr = 31;

    //find the greatest card which is not a briscola and not a "carico"
    for (i=briscola.cpu.hand.size()-1; i<briscola.cpu.hand.size() && (briscola.cpu.hand.at(i).value>4 || briscola.briscolaCard.suit == briscola.cpu.hand.at(i).suit); i--);
    if (i>briscola.cpu.hand.size()) {
        //se non c'e' gioca la carta piu' piccola
        for (int j = 0; j < briscola.cpu.hand.size(); j++)
        {
            if (briscola.cpu.hand.at(j).value < minpts)
            {
                minpts = briscola.cpu.hand.at(j).value;
                i = j;
            }
        }
        if (minpts == 12)
		    i = 0;
    }

    /*
    for (int i = 0; i < briscola.cpu.hand.size(); i++)
    {
        if ((int)briscola.cpu.hand.at(i).suit == (int)briscola.briscolaCard.suit)
        {
            //briscola.cpu.hand.at(i).strength += 20;
        }

        if (briscola.cpu.hand.at(i).value < minpts)
        {
            minpts = briscola.cpu.hand.at(i).value;
            minstr = briscola.cpu.hand.at(i).strength;
            chose = i;
        }
        else
        {
            if (briscola.cpu.hand.at(i).value == minpts && briscola.cpu.hand.at(i).strength < minstr)
            {
                minpts = briscola.cpu.hand.at(i).value;
                minstr = briscola.cpu.hand.at(i).strength;
                chose = i;
            }
        }
    }
    
    return chose;
    */

    return i;
}

/*Cerca la piu' grande carta dello stesso seme che prende, o la piu' piccola che non prende
 PARAMETRI DI INPUT:
	mano: le carte in mano al giocatore da "aiutare"
	c: carta giocata dall'altro giocatore
	maggiore: flag che identifica se bisogna trovare "la piu' grande che prende" (true) o "la piu' piccola che non prende" (false)
 retituisce l'indice della carta da giocare
*/
size_t Game::getSoprataglio(const std::vector<Card> &mano, Card c, bool maggiore) {
    bool trovata=false;
    size_t i;
    if (maggiore) { //si cerca la carta piu' grande che puo' prendere la carta giocata
        for (i=mano.size()-1; i<mano.size(); i--)
            if ((int)c.suit == (int)briscola.cpu.hand.at(i).suit && c.value<briscola.cpu.hand.at(i).value) {
                trovata=true;
                break;
            } else if ((int)c.suit == (int)briscola.cpu.hand.at(i).suit && briscola.cpu.hand.at(i).value<c.value)
                break;
    } else { //si cerca la carta piu' piccola che non puo' prendere la carta giocata
        for (i=0; i<mano.size(); i++)
            if ((int)c.suit == (int)briscola.cpu.hand.at(i).suit && c.value<briscola.cpu.hand.at(i).value) {
                trovata=true;
                break;
            }
    }
    if (trovata)
        return i;
    else
        return mano.size();
}

int Game::cpuDefenseLogic()
{
    int i = 3;
    //int chose = 0;
    bool briscolaFnd = false;
    int minBriscolaPts = 12;
    int maxpts = -1, minpts = 23, minstr = 31, incstr = 0;
    bool taken = false;


    if ((int)briscola.p1DroppedCard.suit != (int)briscola.briscolaCard.suit)
    {
        for (int j = 0; j < briscola.cpu.hand.size(); j++)
        {
            if ((int)briscola.cpu.hand.at(j).suit == (int)briscola.briscolaCard.suit)
            {
                if (briscola.cpu.hand.at(j).value < minBriscolaPts)
                {
                    minBriscolaPts = briscola.cpu.hand.at(j).value;
                    briscolaFnd = true;
                    i = j;
                }
            }
        }

        int k = getSoprataglio(briscola.cpu.hand, briscola.p1DroppedCard, true);
        if (k < briscola.cpu.hand.size())
            return k;

        if (briscola.p1DroppedCard.value>0 && i<briscola.cpu.hand.size()) {//se non si puo' sopratagliare e la carta ha un punteggio e si ha una carta di briscola
            if (briscola.p1DroppedCard.value>4) //se la carta giocata e' un carico
                return i; //si gioca la briscola
            if (briscola.cpu.hand.at(i).value>0) //se la carta che si vuole giocare e' briscola ed ha un punteggio
                return i;
        }
        else
        {
            for (int j = 0; j < briscola.cpu.hand.size(); j++)
            {
                if (briscola.cpu.hand.at(j).value == 0)
                    return j;
            }
        }
    }
    else
    {
        for (int j = 0; j < briscola.cpu.hand.size(); j++)
        {
            if ((int)briscola.cpu.hand.at(j).suit != (int)briscola.briscolaCard.suit)
            {
                if (briscola.cpu.hand.at(j).value == 0)
                    return j;
            }
        }
    }
    i = 0;
    return i;

    /*
    if ((int)briscola.p1DroppedCard.suit == (int)briscola.briscolaCard.suit)
    {
        briscola.p1DroppedCard.strength += 20;
    }

    for (int i = 0; i < briscola.cpu.hand.size(); i++)
    {
        if ((int)briscola.cpu.hand.at(i).suit == (int)briscola.briscolaCard.suit)
        {
            briscola.cpu.hand.at(i).strength += 20;
        }
        if ((int)briscola.cpu.hand.at(i).suit != (int)briscola.p1DroppedCard.suit && (int)briscola.p1DroppedCard.suit != (int)briscola.briscolaCard.suit)
        {
            incstr = 10;
        }
        else
        {
            incstr = 0;
        }

        if (briscola.cpu.hand.at(i).strength > briscola.p1DroppedCard.strength + incstr)
        {
            taken = true;

            if (briscola.cpu.hand.at(i).value + briscola.p1DroppedCard.value > maxpts)
            {
                maxpts = briscola.cpu.hand.at(i).value + briscola.p1DroppedCard.value;
                minstr = briscola.cpu.hand.at(i).strength;
                chose = i;
            }
            else
            {
                if (briscola.cpu.hand.at(i).value + briscola.p1DroppedCard.value == maxpts && briscola.cpu.hand.at(i).strength < minstr)
                {
                    minstr = briscola.cpu.hand.at(i).strength;
                    chose = i;
                }
            }
        }
        else
        {
            if (!taken)
            {
                if (briscola.cpu.hand.at(i).value + briscola.p1DroppedCard.value < minpts)
                {
                    minpts = briscola.cpu.hand.at(i).value + briscola.p1DroppedCard.value;
                    minstr = briscola.cpu.hand.at(i).strength;
                    chose = i;
                }
                else {
                    if (briscola.cpu.hand.at(i).value + briscola.p1DroppedCard.value == minpts && briscola.cpu.hand.at(i).strength < minstr) {
                        minstr = briscola.cpu.hand.at(i).strength;
                        chose = i;
                    }
                }
            }
        }
    }

    return chose;
    */
}

void Game::renderWinMessage()
{
    // Message rectangle position
	messageRect = { 690, 335, 240, 61 };

	// Display message if winner message timer is active
	if (textRenFS50.active()) {
		// Rectangle color
		SDL_SetRenderDrawColor(renderer, grey.r, grey.r, grey.b, grey.a);

		// Displaying 'a draw' message
		if (briscola.checkWinner(briscola.player.getScore(), briscola.cpu.getScore()) == 0) {

			textX = messageRect.x + xTextOffset;
			textY = messageRect.y + yTextOffset;
			SDL_RenderFillRect(renderer, &messageRect);
			textRenFS50.renderText("A Draw!", textX, textY, white);
		}
        else if (briscola.checkWinner(briscola.player.getScore(), briscola.cpu.getScore()) == 1) {
            messageRect.x -= 49;
			messageRect.w = 270;

			textX = messageRect.x + xTextOffset;
			textY = messageRect.y + yTextOffset;
			SDL_RenderFillRect(renderer, &messageRect);
			textRenFS50.renderText("You win!", textX, textY, lightGold);
        }
        else if (briscola.checkWinner(briscola.player.getScore(), briscola.cpu.getScore()) == -1) {
            messageRect.w = 340;
			SDL_RenderFillRect(renderer, &messageRect);
	
			textX = messageRect.x + xTextOffset;
			textY = messageRect.y + yTextOffset;
			textRenFS50.renderText("You lose!", textX, textY, lightBlue);
        }
    }
}
