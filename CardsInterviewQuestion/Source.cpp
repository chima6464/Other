#include "Deck.h"
#include <ctime>
#include <iostream>
using namespace std;

int main()
{
    //ability to randomize
    srand(time(nullptr));
    //1) create a deck of cards, and print/display the cards
    Deck * d = new Deck();
    d->Print();
    //2) shuffle the deck of cards, and display the cards (in their shuffled order)
    d->Shuffle();
    d->Print();
    //3) reshuffle the deck, and display the cards (in their shuffled order)
    d->Shuffle();
    d->Print();
    //4) look for any 2-card sequence that was also present in the previous shuffle. If any are found, go back to step 3
    while (d->isTwoCardValid())
    {
        d->Shuffle();
        d->Print();
    }
    
    return 0;
}


//Answers to Questions

/*1) How might you adapt your code to check for sequences of 3, 4, or 
more cards from the previous shuffle?

A : Since I used a map, I could check for whether any given card matches a specific
identity it maps to (queen of diamonds) to see if its not the current iteration

2) How might you adapt your code to check for combinations of N cards, rather than sequences?
A: I would change every card to have an array of cards that it neighbored in the map. This would enable me to see if its the same combo as before
I'm not sure of any game where tracking previous arragements is necessary 

3) How might you adapt your code to check for card sequences that were present in any previous shuffle? 
(i.e. comparing against all previous shuffles, not just the most recent one)

A: While the space complexity of this solution would be greater, I would map of cards to vectors which store all previous sequences. The issue here is that
going through the map with an example like "Ace of diamonds"->"2 of heartss"->"3 of clubs->7 of diamonds" could yield a sequence that never happened

4)If necessary, how might you change your code to accommodate more than 4 suits, or more than 13 different card values.
A : The good news is, I created a #define for NUM_CARDS to accomodate for extra cards. Additionally, as long as you add the card value and/or the suit to the proper
enumeration, the code can accomodate for it

5) Would you do things any differently if you were optimizing purely for speed? How about if you were trying to minimize memory usage?
A : I am not sure, but the nested loop needed to generate the deck is O(n^2) and everywhere we print the deck is O(N). Since we implement a hashing technique, we're
at O(1). All card swapping is also O(1)

As for memory usage, additional memory comes from the map and any reduction of memory, w/o compromising for speed, would come from a smaller deck. 
*/