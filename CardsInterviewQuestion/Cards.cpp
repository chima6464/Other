#include "Cards.h"
#include <string>
#include <iostream>

Card::Card(Ranks RANK, Suits SUIT)
    : Rank(RANK), Suit(SUIT)
{

}

Card::~Card()
{

}

Card::Ranks Card::GetRank()
{
    return Rank;
}

Card::Suits Card::GetSuit()
{
    return Suit;
}

string Card::GetFaceValue()
{
   switch (Card::GetRank())
   {
    case ACE:
        return "Ace";
    case TWO:
        return "Two";
    case THREE:
        return "Three";
    case FOUR:
        return "Four";
    case FIVE:
        return "FIVE";
    case SIX:
        return "SIX";
    case SEVEN:
        return "Seven";
    case EIGHT:
        return "Eight";
    case NINE:
        return "Nine";
    case TEN:
        return "Ten";
    case JACK:
        return "Jack";
    case QUEEN:
        return "Queen";
    case KING:
        return "King";
    default:
        return NULL;
   }
}

string Card::GetSuitValue()
{
   switch (Card::GetSuit())
   {
    case HEARTS:
        return "HEARTS";
    case DIAMONDS:
        return "DIAMONDS";
    case CLUBS:
        return "CLUBS";
    case SPADES:
        return "SPADES";
    default:
        return NULL;
   }
}

void Card::PrintCard()
{
   int rank = static_cast<int>(Card::GetRank());
   int suit = static_cast<int>(Card::GetSuit());
   std::cout << to_string(rank) << "of" << to_string(suit);
}