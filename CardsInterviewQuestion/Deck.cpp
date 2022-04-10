#include "Deck.h"
#include <iostream>
#include <algorithm>

Deck::Deck(/* args */)
    :deck()
{
    deck.reserve(NUM_CARDS);
    for(Card::Ranks r = Card::TWO; r <=Card::ACE; r = static_cast<Card::Ranks>(static_cast<int>(r) + 1))
    {
        for(Card::Suits s = Card::HEARTS; s <=Card::SPADES; s = static_cast<Card::Suits>(static_cast<int>(s) + 1))
        {
            deck.push_back(new Card(r,s));
           
        }
    }
}

Deck::~Deck()
{
    for(Card* card : deck)
    {
        delete card;
    }
    //delete data
    deck.clear();
}

void Deck::Shuffle()
{
    //Make sure to record each shuffle before shuffling again
    recallPrevShuffle();
    std::random_shuffle(deck.begin(), deck.end());
}

void Deck::Print()
{
    for(Card* card : deck)
    {
        card->PrintCard();
    }
}

void Deck::insertCard(Card * card)
{
    deck.push_back(card);
}

void Deck::recallPrevShuffle()
{
    for (int i = 0; i < deck.size() - 1; i++) {
        lastShuffle[deck[i]->GetFaceValue() + " of " + 
                         deck[i]->GetSuitValue()] = 
          deck[i+1]->GetFaceValue() + " of " + 
          deck[i+1]->GetFaceValue();
      }
      lastShuffle[deck[deck.size() - 1]->GetFaceValue() + " of " +
                       deck[deck.size() - 1]->GetSuitValue()] = "";
}

bool Deck::isTwoCardValid()
{
    for (int i = 0; i < deck.size() - 1; i++) 
    {
        if (lastShuffle[deck[i]->GetFaceValue() + " of " + 
                             deck[i]->GetSuitValue()] == 
            deck[i+1]->GetFaceValue() + " of " + 
            deck[i+1]->GetSuitValue()) 
        {
          return true;
        }
    }
    return false;
}