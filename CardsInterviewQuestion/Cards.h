#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

class Card
{

public:
    enum Suits
    {
        HEARTS = 3,
        DIAMONDS,
        CLUBS,
        SPADES
    };

    enum Ranks
    {
        TWO = 2,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
        NINE,
        TEN,
        JACK,
        QUEEN,
        KING,
        ACE //depending on game, could be highest or lowest card
    };

    Card(Ranks RANK, Suits SUIT);
    virtual ~Card();

    Ranks GetRank();
    Suits GetSuit();

    string GetFaceValue();
    string GetSuitValue();
    void PrintCard();


private:
    /* data */
    Ranks Rank;
    Suits Suit;

};

