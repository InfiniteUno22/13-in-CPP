#include <iostream>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <cstdlib>
//#include <vector>

int i, j, l;

class card
{
public:
    char rank;
    std::string suit;
    int suitlevel;
    int ranklevel;

    card() {}

    void printCard()
    {
        std::cout << rank << " of " << suit <<"\n";
    }
};
card deck[52], temp; 
//vector<card> vectdeck(52, card filler());

class hand : public card
{
    public:
        card yourHand[13];

        void addCard(card added, int slot) 
        {
            yourHand[slot] = added;
        }
        hand() {}

        virtual void printHand() 
        {
            for (card x:yourHand) {
                x.printCard();
            }
        }
};

void shuffle()
{
    int n, m;
    for (int n = 0; n < 52; n++) {
        m = rand() % 52;
        temp = deck[n];
        deck[n] = deck[m];
        deck[m] = temp;
    }
}
void printdeck() 
{
    int k;
    for (k = 0; k < 52; k++) {
        deck[k].printCard();
        std::cout << "\n";
    }
}

void sortHand(hand & x) {
    card swap;

    for (int i = 0; i < 12; i++) 
    {
        int min_idx = i;
        for (int j = i + 1; j < 13; j++) 
        {
            if (x.yourHand[j].ranklevel <= x.yourHand[min_idx].ranklevel) 
            {
                if (x.yourHand[j].ranklevel == x.yourHand[min_idx].ranklevel)
                {
					if (x.yourHand[j].suitlevel < x.yourHand[min_idx].suitlevel){
                        min_idx = j;
                    }
                }
                else{
					min_idx = j;
                }
            }
            
        }
        if (min_idx != i) {

            swap = x.yourHand[i];
            x.yourHand[i] = x.yourHand[min_idx];
            x.yourHand[min_idx] = swap;
        }
    }
}


int main()
{
    srand(time(0));
    //creating the cards
    char rank[] = {'3','4','5','6','7','8','9','T','J','Q','K','A','2' };
    std::string suit[] = {"Spades","Clubs","Diamonds","Hearts"};
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            deck[l].rank = rank[j];
            deck[l].suit = suit[i];
            deck[l].ranklevel = j;
            deck[l].suitlevel = i;
            l++;
        }
    }
    shuffle();
    //printdeck(); 
    // Define players
    hand p1;
    hand p2;
    hand p3;
    hand p4;
    
    int x;
    int cardSpot = 0;
    for (int i = 0; i < 52; i++)
    {
        x = i % 4;
        if (x == 0) { p1.addCard(deck[i], cardSpot); }
        if (x == 1) { p2.addCard(deck[i], cardSpot); }
        if (x == 2) { p3.addCard(deck[i], cardSpot); }
        if (x == 3) { 
            p4.addCard(deck[i], cardSpot);
            cardSpot++; 
        }
    }

    sortHand(p1);
    sortHand(p2);
    sortHand(p3);
    sortHand(p4);
    std::cout << "Player 1 Hand\n";
    p1.printHand();
    /*
	std::cout << "\nPlayer 2 Hand\n";
    p2.printHand();
	std::cout << "\nPlayer 3 Hand\n";
    p3.printHand();
	std::cout << "\nPlayer 4 Hand\n";
    p4.printHand();
    */
 
    //GAME LOOP
    
    bool gameStart = true;
    std:: string input;
    while (gameStart) {
            std::cout << "Game has Started\n";
            std::cin >> (input);
            if (input == "End"){
                gameStart = false; 
                std::cout << "Game has Ended\n";
            }
                

    }
    
    return 0;
}
