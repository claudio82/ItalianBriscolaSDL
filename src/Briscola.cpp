#include "Briscola.h"

Card::Card()
{
    name = CardName::ACE;
    suit = Suit::BATONS;
    value = 1;
    strength = 0;
    hidden = false;
}

std::string Card::getName()
{
    // Convert card name to the string name
    switch (name)
    {
    case CardName::ACE:
        return std::string("ACE");
        break;

    case CardName::JACK:
        return std::string("JACK");
        break;

    case CardName::HORSE:
        return std::string("HORSE");
        break;

    case CardName::KING:
        return std::string("KING");
        break;
    
    default:
        return std::to_string((int)name);
        break;
    }
}

std::string Card::getSuit()
{
    // Convert card Suit to the string name
    switch (suit)
    {
    case Suit::BATONS:
        return std::string("BATONS");
        break;

    case Suit::SPADES:
        return std::string("SPADES");
        break;

    case Suit::CUPS:
        return std::string("CUPS");
        break;

    case Suit::GOLDS:
        return std::string("GOLDS");
        break;
    
    default:
        return std::string("NONE");
        break;
    }
}

void Card::handleEvent(SDL_Event* e)
{
    //If mouse event happened
	if (e->type == SDL_EVENT_MOUSE_MOTION || e->type == SDL_EVENT_MOUSE_BUTTON_DOWN || e->type == SDL_EVENT_MOUSE_BUTTON_UP)
	{
		//Get mouse position
		float x, y;
		SDL_GetMouseState(&x, &y);
        
        //std::cerr << "x=" << x << ", y=" << y << std::endl;

		//Check if mouse is in card
		bool inside = true;

        //Mouse is right of the card icon
		//if (x > mPosition.x + CARDTOCARDOFFSET - 1)
		//{
		//	inside = false;
		//}

        //Mouse is right of the entire card
		if (x > mPosition.x + 200)
		{
			inside = false;
		}

        //Mouse is left of the card
		if (x < mPosition.x)
		{
			inside = false;
		}

		//Mouse above the card
		else if (y < mPosition.y)
		{
			inside = false;
		}
		//Mouse below the card
		else if (y > mPosition.y + 280)
		{
			inside = false;
		}

        //Mouse is outside card
		if (!inside)
		{
			//cardCurrentSprite = CARD_SPRITE_MOUSE_OUT;
		}
		//Mouse is inside card
		else
		{
			//Set mouse over sprite
			switch (e->type)
			{
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    selected = true;
                    std::cerr << getName() << " of " << getSuit() << " Clicked ! ! !" << std::endl;
                    break;
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    break;
            }
        }
    }
}

void Deck::init()
{
    // Remove old cards
    cards.clear();

    // Initializes cards with standard order
	for (int group = (int)Suit::BATONS; group <= (int)Suit::GOLDS; group++) {
		for (int card = (int)CardName::ACE; card <= (int)CardName::KING; card++) {
            Card c;
            c.suit = (Suit)group;
            c.name = (CardName)card;

            if (c.name == CardName::ACE)
            {
                c.value = 11;
                c.strength = 10;
            }
            else if (c.name == CardName::THREE)
            {
                c.value = 10;
                c.strength = 9;
            }
            else if (c.name == CardName::JACK)
            {
                c.value = 2;
                c.strength = 6;
            }
            else if (c.name == CardName::HORSE)
            {
                c.value = 3;
                c.strength = 7;
            }
            else if (c.name == CardName::KING)
            {
                c.value = 4;
                c.strength = 8;
            }
            else
            {
                c.value = 0;
                if (c.name == CardName::TWO)
                    c.strength = 1;
                else
                    c.strength = card - 2;
            }

            int index = group * 10 + card - 1;
            cards.insert(cards.begin() + index, c);
        }
    }
}

void Deck::shuffle()
{
    // Shuffle the deck
    std::random_device rd;
    std::mt19937 gen(rd());

    std::shuffle(cards.begin(), cards.end(), gen);
}

void Player::updateScore()
{
}

void Player::addCardToHand(Card& card, int index, int xStart, int yStart, int cardWidth, int cardHeight, int widthMultiplier, bool hidden)
{
    card.mPosition.x = xStart + cardWidth * widthMultiplier * index;
    card.mPosition.y = yStart;

    card.hidden = hidden;

    // Adds card to'hand'
    hand.insert(hand.begin() + index, card); //.push_back(card);
}

Card Player::pickCardFromHand(int index)
{
    Card c;
    if (hand.size() > 0 && (index >= 0 && index < 3))
    {
        c = hand.at(index);
        hand.erase(hand.begin() + index);
    }
    return c;
}

void Player::clearHand()
{
    hand.clear();
}

void Player::init(std::string name)
{
    score = 0;
    this->name = name;
}

void Player::setName(std::string name)
{
    this->name = name;
}

void Player::setScore(int val)
{
    score = val;
}

std::string Player::getName()
{
    return name;
}

int Player::getScore()
{
    return score;
}

int Briscola::checkWinner(int plScore, int cpuScore)
{
    // Determines winner
    if (plScore == cpuScore)
        return 0;
    else if (plScore > cpuScore)
        return 1;
    else
        return -1;
}

void Briscola::createNewGame()
{
    // Clear player's & cpu's 'hand'
    cpu.hand.clear();
    player.hand.clear();

    cpu.setScore(0);
    player.setScore(0);

    // Create a new mixed deck of cards
    deck.init();
    deck.shuffle();
}

void Briscola::roundWinner(std::string lastWinner, Card pl, Card cpu, Card b)
{
    // set cards 'weights'
    if (lastRoundWinner == "P1")
    {
        if ((int)pl.suit == (int)b.suit)
        {
            pl.strength += 20;
        }
        else
        {
            if ((int)pl.suit != (int)cpu.suit)
            {
                pl.strength += 10;
            }
        }
        if ((int)cpu.suit == (int)b.suit)
        {
            cpu.strength += 20;
        }
    }
    else if (lastRoundWinner == "CPU")
    {
        if ((int)cpu.suit == (int)b.suit)
        {
            cpu.strength += 20;
        }
        else
        {
            if ((int)cpu.suit != (int)pl.suit)
            {
                cpu.strength += 10;
            }
        }
        if ((int)pl.suit == (int)b.suit)
        {
            pl.strength += 20;
        }
    }

    std::cout << "(PLAYER card strength = " << pl.strength << ")" << std::endl;
    std::cout << "(CPU card strength = " << cpu.strength << ")"<< std::endl;

    // determines the round winner
    if (pl.strength > cpu.strength)
    {
        lastRoundWinner = "P1";
    }
    else if (cpu.strength > pl.strength) {
        lastRoundWinner = "CPU";
    }
}

void Briscola::handleEvent(SDL_Event* e)
{
    //If mouse event happened
	if (e->type == SDL_EVENT_MOUSE_MOTION || e->type == SDL_EVENT_MOUSE_BUTTON_DOWN || e->type == SDL_EVENT_MOUSE_BUTTON_UP)
	{
        //Get mouse position
		float x, y;
		SDL_GetMouseState(&x, &y);

		for (int i = 0; i < player.hand.size(); i++)
		{
			player.hand.at(i).handleEvent(e);

			//if a card was clicked on, select that card, deselect all other cards
			if (player.hand.at(i).selected == true)
			{
				player.selCardIdx = i;
				for (int j = 0; j < player.hand.size(); j++)
				{
					if (j != player.selCardIdx)
						player.hand.at(j).selected = false;
				}
			}
		}
    }
}

void PlayerDrawer::drawBriscola(Card c)
{
    //std::cout << "Briscola is: " << c.getSuit() << std::endl;
    brisCardRect.w = 356;
	brisCardRect.h = 499;

    // Change suit
    brisCardRect.x = (int)c.suit * brisCardRect.w + (int)c.suit * 2;

    // Change value
    brisCardRect.y = 10 * brisCardRect.h + (10 - (int)c.name) * 2 - (int)c.name * brisCardRect.h;

    //std::cout << "srcCard.x = " << srcCard.x << std::endl;
    //std::cout << "srcCard.y = " << srcCard.y << std::endl;

    brisDstScreen.y = yStart;
    brisDstScreen.x = xStart;
    brisDstScreen.w = cardWidth;
    brisDstScreen.h = cardHeight;

    //std::cout << "dstScreen.w = " << dstScreen.w << std::endl;
    //std::cout << "dstScreen.h = " << dstScreen.h << std::endl;

    //dstScreen.x = 200;

    //brisCardRect.x = 30;
    //brisCardRect.y = 50;
    //brisCardRect.w = cardWidth;
	//brisCardRect.h = cardHeight;

    //bool res = 
    SDL_RenderTexture(renderer, briscTex, &brisCardRect, &brisDstScreen);
    //if (!res)
    //{
        
    //    std::cout << "SDL_RenderTexture() ERROR: " << SDL_GetError() << std::endl;
    //}
}

void PlayerDrawer::drawDropped(Card c, int dstX, int dstY)
{
    discardedRectSrc.w = 356;
    discardedRectSrc.h = 499;
    discardedRectSrc.x = (int)c.suit * discardedRectSrc.w + (int)c.suit * 2;
    discardedRectSrc.y = 10 * discardedRectSrc.h + (10 - (int)c.name) * 2 - (int)c.name * discardedRectSrc.h;

    discardedRectDst.x = dstX;
    discardedRectDst.y = dstY;
    discardedRectDst.w = cardWidth;
    discardedRectDst.h = cardHeight;

    SDL_RenderTexture(renderer, discardedTex, &discardedRectSrc, &discardedRectDst);
}

void PlayerDrawer::drawDroppedCpu(Card c, int dstX, int dstY)
{
    discardedRectSrc2.w = 356;
    discardedRectSrc2.h = 499;
    discardedRectSrc2.x = (int)c.suit * discardedRectSrc2.w + (int)c.suit * 2;
    discardedRectSrc2.y = 10 * discardedRectSrc2.h + (10 - (int)c.name) * 2 - (int)c.name * discardedRectSrc2.h;

    discardedRectDst2.x = dstX;
    discardedRectDst2.y = dstY;
    discardedRectDst2.w = cardWidth;
    discardedRectDst2.h = cardHeight;

    SDL_RenderTexture(renderer, discardedTex2, &discardedRectSrc2, &discardedRectDst2);
}

bool PlayerDrawer::init(SDL_Texture* deck, SDL_Renderer* ren, Player* pl, int xStart, int yStart, int cardWidth, int cardHeight, int animationDirection)
{
    bool error = false;

    // Renderer initialization
	renderer = ren;
	if (!renderer) {
		//std::cerr << "[ERROR] PlayerDrawer: SDL_Renderer* = nullptr" << std::endl;
		error = true;
	}

	// Loading card deck texture
    deckTex = deck;
	//deckTex = AssetManager::getTexture("deck");  //TextureLoader::loadTexture(texturePath.c_str(), renderer);
	if (!deckTex) {
		//std::cerr << "[ERROR] PlayerDrawer: Unable to load texture" << std::endl;
		error = true;
	}

    discardedTex = discardedTex2 = deck;
    if (!discardedTex || !discardedTex2)
    {
        error = true;
    }

	// Initialize player
	player = pl;
	if (!player) {
		//std::cerr << "[ERROR] PlayerDrawer: Player* = nullptr" << std::endl;
		error = true;
	}

	// Initialize animation variables
	this->animationDirection = animationDirection;
	animatingStartedbl = false;
	animating = false;
	block = false;
	lastCardCount = 0;

    // Deck on screen position initialization
	this->xStart = xStart;
	this->yStart = yStart;
	this->cardWidth = cardWidth;
	this->cardHeight = cardHeight;

	// Setting card hidden position, dependent on animation direction 
	animatedCardY = animationDirection == -1 ? yStart - cardWidth * 2 : animationDirection == 1 ? yStart + cardWidth * 2 : yStart;


	// Return 'true' if object is initialized successful
	// Return 'false' if error occurred with Player* or texture loading or SDL_Renderer*
	return !error;
}

bool PlayerDrawer::initBriscola(SDL_Texture* brisc, SDL_Renderer* ren, int xStart, int yStart, int cardWidth, int cardHeight)
{
    bool error = false;
    
    // Renderer initialization
	renderer = ren;
	if (!renderer) {
		//std::cerr << "[ERROR] PlayerDrawer: SDL_Renderer* = nullptr" << std::endl;
		error = true;
	}

    // Loading card deck texture
    briscTex = brisc;
    if (!briscTex)
    {
        error = true;
    }

    // Deck on screen position initialization
	this->xStart = xStart;
	this->yStart = yStart;
	this->cardWidth = cardWidth;
	this->cardHeight = cardHeight;

    return !error;
}

bool PlayerDrawer::useTexture(SDL_Texture* tex)
{
    // Reload card texture
    bool isError = false;

	SDL_DestroyTexture(deckTex);

	// Loading texture
    deckTex = tex;

    if (!deckTex)
    {
        isError = true;
    }

    return !isError;
}

void PlayerDrawer::clearHand()
{
    lastCardCount = 0;
}

void PlayerDrawer::drawHand()
{
    // Used to avoid some pixels in texture. 
	// Change this to 0 if you have good texture.
    int xOffset = 2;
    int yOffset = 2;

    // Cards count in texture
    int cardsInTex = 10;

    // Card overlapping multiplier
	float widthMultiplier = 1.0;

	// Texture parameters. (I used vertical texture)
	srcCard.w = 356;
	srcCard.h = 499;

    dstScreen.y = yStart;
    dstScreen.w = cardWidth;
    dstScreen.h = cardHeight;

    // Display card
    for (int i = 0; i < player->hand.size() /*lastCardCount*/ ; i++)
    {
        // Change suit
        srcCard.x = (int)player->hand.at(i).suit * srcCard.w + (int)player->hand.at(i).suit * xOffset;

        // Change value
        srcCard.y = cardsInTex * srcCard.h + (cardsInTex - (int)player->hand.at(i).name) * yOffset - (int)player->hand.at(i).name * srcCard.h;

        // Change card on screen displacement by X
		dstScreen.x = xStart + cardWidth * widthMultiplier * i;
		
		// Use back of the card, if card is hidden
		if (player->hand.at(i).hidden) {
			srcCard.y = (cardsInTex) * srcCard.h + (cardsInTex + 1) * yOffset;
		}

		// Render
		SDL_RenderTexture(renderer, deckTex, &srcCard, &dstScreen);
    }

    animating = false;
}

bool PlayerDrawer::animationStarted()
{
	return animatingStartedbl;
}

bool PlayerDrawer::animatingStatus()
{
	return animating;
}

int PlayerDrawer::getXStart()
{
	return xStart;
}

int PlayerDrawer::getYStart()
{
	return yStart;
}
