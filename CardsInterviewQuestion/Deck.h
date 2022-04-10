#include <vector>
#include <iostream>
#include "Cards.h"
#include <map>
#define NUM_CARDS   52
class Deck
{
private:
    /* data */
    std::vector<Card*> deck;
    map<string, string> lastShuffle;
public:
    Deck(/* args */);
    ~Deck();

    void Print();
    void Shuffle();
    bool isTwoCardValid();
    void recallPrevShuffle();
    void insertCard(Card * card);
};


