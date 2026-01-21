#pragma once
#include <iostream>
#include <algorithm>
#include <random>
#include <string>
#include <vector>
#include "AssetManager.h"

// Card suit enumeration
enum class Suit
{
    BATONS = 0,
    SPADES = 1,
    CUPS = 2,
    GOLDS = 3
};

// Card name enumeration
enum class CardName
{
    ACE = 1,
    TWO = 2,
    THREE = 3,
    FOUR = 4,
    FIVE = 5,
    SIX = 6,
    SEVEN = 7,
    JACK = 8,
    HORSE = 9,
    KING = 10
};

// Card class
class Card
{
public:
    Card();

    CardName name;
    Suit suit;
    int value;
    int strength;
    bool hidden;
    bool selected;

    //Top left position
    SDL_Point mPosition;

    std::string getName();
    std::string getSuit();

    void handleEvent(SDL_Event* e);

private:
    
};

// Deck class
class Deck
{
public:
    std::vector<Card> cards;

    // Initializes a new deck of cards with 'usual' order
    void init();

    // Mixes cards in the deck
    void shuffle();
};

// Game Player class
class Player
{
public:
    // Player's cards in 'hand'
    std::vector<Card> hand;

    int selCardIdx = -1;

    // Initializes player with specified name
    void init(std::string name);

    void setName(std::string name);
    void setScore(int val);

    std::string getName();
    int getScore();

    // Adds card to player's 'hand' at given index
    void addCardToHand(Card& card, int index, int xStart, int yStart, int cardWidth, int cardHeight, int widthMultiplier, bool hidden);

    // Gets card from hand at given index
    Card pickCardFromHand(int index);

    // Removes all cards from 'hand'
    void clearHand();

private:
    std::string name;

    // Player's match score
    int score;

    // Updates score
    void updateScore();
};

// Briscola Game class
class Briscola
{
public:
    // Deck of cards
    Deck deck;

    Player cpu;
    Player player;

    // Briscola card
    Card briscolaCard;

    Card p1DroppedCard;
    Card cpuDroppedCard;

    std::string lastRoundWinner;

    // Determines the winner of a match
    int checkWinner(int plScore, int cpuScore);

    // Creates a new mixed deck of cards. Clearers player's & cpu's 'hand'.
    void createNewGame();

    // Determines the winner of a round
    void roundWinner(std::string lastWinner, Card pl, Card cpu, Card b);

    // Handles mouse event
    void handleEvent(SDL_Event* e);

private:
    
    //SDL_FRect bCardRect;
    // Briscola card texture
    //SDL_Texture* briscTex;
    //SDL_Renderer* renderer;
};

// Game PlayerDrawer class. Renders player's hand on screen
class PlayerDrawer
{
public:
    // Initializes drawer with specified texture, given location & animation state.
    // I
    // Parameters:
    //     pl                    : Pointer to the player
    //     xStart, yStart        : top left rendering position of the 'hand' 
    //     cardWidth, cardHeight : width & height of one card, rendered on screen
    // I
    // animationDirection:
    //    -1  : Smooth appearance from top to bottom
    //     0  : No smooth appearance
    //     1  : Smooth appearance from bottom to top
    bool init(SDL_Texture* deckTex, SDL_Renderer* ren, Player* pl, int xStart, int yStart, int cardWidth, int cardHeight, int animationDirection);

    bool initBriscola(SDL_Texture* brisc, SDL_Renderer* ren, int xStart, int yStart, int cardWidth, int cardHeight);

    // Upload texture
    bool useTexture(SDL_Texture* tex);

    // Clears variables used for hand drawing
    void clearHand();

    // Draws player hand
    void drawHand();

    // Draws briscola card
    void drawBriscola(Card c);

    // Draws dropped card
    void drawDropped(Card c, int dstX, int dstY);
    void drawDroppedCpu(Card c, int dstX, int dstY);

    // Returns true if any card started animating
    bool animationStarted();

    // Returns true if currently animating
    bool animatingStatus();

    // Returns top left x render position
    int getXStart();

    // Returns top left y render position
    int getYStart();

private:
    SDL_Renderer* renderer;

    // Card deck texture
    SDL_Texture* deckTex;

    // Briscola card texture
    SDL_Texture* briscTex;

    // Discarded card texture
    SDL_Texture* discardedTex;
    SDL_Texture* discardedTex2;

    // Pointer to the player
    Player* player;

    // Card source & destination rectangles
    SDL_FRect srcCard;
    SDL_FRect dstScreen;

    // Briscola card source & destination rectangles
    SDL_FRect brisCardRect;
    SDL_FRect brisDstScreen;

    // Discarded card rectangle
    SDL_FRect discardedRectSrc;
    SDL_FRect discardedRectDst;
    SDL_FRect discardedRectSrc2;
    SDL_FRect discardedRectDst2;


    // Top left render position
    int xStart, yStart;

    // Width & height of on card on screen
    int cardWidth, cardHeight;

    // Current Y position of card, that is animating
    float animatedCardY;

    // Number of the card, that is animating
    int lastCardCount;

    // Animating direction.
    //    -1  : Smooth appearance from top to bottom
    //     0  : No smooth appearance
    //     1  : Smooth appearance from bottom to top
    int animationDirection;

    // Trigger-bool on beginning of a new card animation
    bool animatingStartedbl;

    // Current animating status
    bool animating;

    // Used for trigger-bool
    bool block;
};