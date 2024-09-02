#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <SFML/Graphics.hpp>

class card
{
    
public:
    char rank;
    std::string suit;
    int ranklevel;
    int suitlevel;
    int handnumber;
    sf::Sprite sprite;
    bool isSelected = false;

    card() { rank = 0; suit = "Duece"; ranklevel = 0; suitlevel = 0; handnumber = 1; }

    card(char r, std::string s, int rlevel, int slevel, int hnumber, sf::Sprite prite)
    {
        rank = r;
        suit = s;
        ranklevel = rlevel;
        suitlevel = slevel;
        handnumber = hnumber;
        sprite = prite;
    }

    void handNumSet(int num)
    {
        handnumber = num;
    }

    void printCard()
    {
        if (handnumber > 9) {
            std::cout << handnumber << ".   " << rank << " of " << suit << "\n";
        }
        else {
            std::cout << handnumber << ".    " << rank << " of " << suit << "\n";
        }
    }

    char getRank() {
        return rank;
    }

    int getRankLevel() {
        return ranklevel;
    }

    std::string getSuit() {
        return suit;
    }
    int getHnumber() {
        return handnumber;
    }

    bool isHigher(card x) {
        if (ranklevel > x.ranklevel) {
            return true;
        }
        else if (ranklevel == x.ranklevel) {
            if (suitlevel > x.suitlevel) {
                return true;
            }
        }
        return false;
    }

    void resizeCard(double w, double h) {
        sprite.setScale(sf::Vector2f(w, h));
    }

    sf::Sprite getSprite() {
        return sprite;
    }
    bool getIsSelected() {
        return isSelected;
    }

    void setIsSelected(bool selected) {
        isSelected = selected;
    }
};

card temp;
std::vector<card> vectdeck;

class hand : public card
{
public:
    std::vector<card> yourHand;

    hand() {}

    void addCard(card added)
    {
        yourHand.push_back(added);
    }

    virtual void printHand()
    {
        for (card x : yourHand) {
            x.printCard();
        }
    }

    void setupHand(float y) {
        int overlap = 50;
        int cardWidth = 120;
        int sideBuffer = 1200 - cardWidth - ((cardWidth - overlap) * (yourHand.size() - 1));
        for (int i = 0; i < yourHand.size(); i++) {
            yourHand[i].sprite.setPosition((float)(sideBuffer / 2 + (yourHand[i].getHnumber() - 1) * (cardWidth - overlap)), y);
        }
    }
    void updateHand() {
        int overlap = 50;
        int cardWidth = 120;
        int sideBuffer = 1200 - cardWidth - ((cardWidth - overlap) * (yourHand.size() - 1));
        for (int i = 0; i < yourHand.size(); i++) {
            yourHand[i].sprite.setPosition((float)(sideBuffer / 2 + (yourHand[i].getHnumber() - 1) * (cardWidth - overlap)), yourHand[i].sprite.getPosition().y);
        }
    }

    void numberHand() {
        int cardnum = 1;
        for (int i = 0; i < yourHand.size(); i++) {
            yourHand[i].handNumSet(cardnum);
            cardnum++;
        }
    }

    void sortHand() {
        card swap;
        for (int i = 0; i < yourHand.size()-1; i++)
        {
            int min_idx = i;
            for (int j = i + 1; j < yourHand.size(); j++)
            {
                if (yourHand[j].ranklevel <= yourHand[min_idx].ranklevel)
                {
                    if (yourHand[j].ranklevel == yourHand[min_idx].ranklevel)
                    {
                        if (yourHand[j].suitlevel < yourHand[min_idx].suitlevel)
                        {
                            min_idx = j;
                        }
                    }
                    else
                    {
                        min_idx = j;
                    }
                }
            }
            if (min_idx != i)
            {
                swap = yourHand[i];
                yourHand[i] = yourHand[min_idx];
                yourHand[min_idx] = swap;
            }
        }
    }

    //Single Play
    void playCard(int num) {
        yourHand.erase(yourHand.begin() + num - 1);
        numberHand();
    }
    //Standard Pattern Checks
    // 3 of Spades
    bool has3S() {
        if (yourHand[0].ranklevel == 0 && yourHand[0].suitlevel == 0) {
            return true;
        }
        return false;
    }
    
    bool isSingle(int cardsPlayed) {
        if (cardsPlayed == 1) {
            return true;
        }
        return false;
    }

    bool has2() {
        if (yourHand.back().getRankLevel() == 12) {
            return true;
        }
        return false;
    }

    bool cutHas2() {
        for (int i = 0; i < yourHand.size(); i++) {
            if (yourHand[i].getIsSelected()) {
                if (yourHand[i].getRankLevel() == 12) {
                    return true;
                }
            }
        }   
        return false;

    }


    bool isDupe(int cardsPlayed) {
        card compare;
        bool first = true;
        int correct = 1;
        if (cardsPlayed > 1) {
            for (int i = 0; i < yourHand.size(); i++) {
                if (yourHand[i].getIsSelected()) {
                    if (first) {
                        compare = yourHand[i];
                        first = false;
                        continue;
                    }
                    if (compare.getRank() != yourHand[i].getRank()) {
                        return false;
                    }
                    correct++;
                }
                if (correct == cardsPlayed) {
                    return true;
                }
            }
        }
        return false;
        
    }

    bool isSeq(int cardsPlayed) {
		card compare;
		bool first = true;
		int correct = 1;
        if (cardsPlayed > 2) {
            for (int i = 0; i < yourHand.size(); i++) {
                if (yourHand[i].getIsSelected()) {
                    std::cout << yourHand[i].getRankLevel() << std::endl;
                    if (yourHand[i].getRankLevel() == 12) {
                        return false;
                    }
                    if (first) {
                        compare = yourHand[i];
                        first = false;
                        continue;
                    }
                    if (compare.getRankLevel() != yourHand[i].getRankLevel() - 1) {
                        return false;
                    }
                    correct++;
                    compare = yourHand[i];
                }
                if (correct == cardsPlayed) {
                    return true;
                }
            }
        }
        return false;
    }

    // Cut Detectors
    bool isDubSeq(int cardsPlayed) {
        card compare;
        bool first = true;
        int correct = 1;
        if (cardsPlayed > 5) {
            for (int i = 0; i < yourHand.size(); i++) {
                if (yourHand[i].getIsSelected()) {
                    if (yourHand[i].getRankLevel() == 12) {
                        return false;
                    }
                    if (first) {
                        compare = yourHand[i];
                        first = false;
                        continue;
                    }
                    if (correct%2==1){
						if (compare.getRankLevel() != yourHand[i].getRankLevel()) {
							return false;
						}
                    }
                    else{
						if (compare.getRankLevel() != yourHand[i].getRankLevel() - 1) {
							return false;
						}
                    }
                    correct++;
                    compare = yourHand[i];
                }
                if (correct == cardsPlayed) {
                    return true;
                }
            }
        }
        return false;
    }

    bool isQuad(int cardsPlayed) {
        if (cardsPlayed == 4) {
            return isDupe(4);
        }
        return false;
    }

    // return a given selection's cut strength, this will be written within the cut logic 
    // so we can omit situations involving non-cuts
    int getCutStrength(int cardsPlayed) {
        int cutStrength = 0;
        // Lets check for 2s (for playPile usage)
        if (!cutHas2()) {
            if (cardsPlayed == 4) {
                cutStrength = cardsPlayed;
                std::cout << "CutStrength: " << cutStrength << std::endl;
            }
            else {
                cutStrength = cardsPlayed - 3;
                std::cout << "CutStrength: " << cutStrength << std::endl;
            }
            
        }
        return cutStrength;
    }

    // Instant Wins
    // Dragon Check
    bool dragonCheck() {
        for (int i = 0; i < 13; i++) {
            if (yourHand[i].ranklevel != i) {
                return false;
            }
        }
        return true;
    }
    // All Doubles
    bool aDoublesCheck() {
        int doublesCount = 0;
        for (int i = 0; i < 12; i++) {
            if (yourHand[i].ranklevel == yourHand[i + 1].ranklevel) {
                doublesCount += 1;
                i++;
            }
        }
        if (doublesCount == 6) {
            return true;
        }
        else {
            return false;
        }
    }
    // 4 2s Check
    bool four2sCheck() {
        for (int i = 9; i < 13; i++) {
            if (yourHand[i].ranklevel != 12) {
                return false;
            }
        }
        return true;
    }
    
};

class player :public hand {
public:
    int number;
    bool prio;
    bool hasPassed;
    bool isOut;
    player()
    {
        number = 0;
        prio = false;
        hasPassed = false;
        isOut = false;
    }
    player(int num, bool pri, bool pass) {
        number = num;
        prio = pri;
        hasPassed = pass;
        isOut = false;

    }
    void print() {
        std::cout << "Player " << number << ":\n";
    }

    bool getPrio() {
        return prio;
    }
    void setPrio(bool prior) {
        prio = prior;
    }
    bool getHasPassed() {
        return hasPassed;
    }
    void setHasPassed(bool pass) {
        hasPassed = pass;
    }
    int getNumber() {
        return number;
    }
    bool getIsOut() {
        return isOut;
    }
    void setIsOut(bool out) {
        isOut = out;
    }
};


void vectShuffle()
{
    srand(time(0));
    int n, m;
    for (int n = 0; n < 52; n++) {
        m = rand() % 52;
        temp = vectdeck[n];
        vectdeck[n] = vectdeck[m];
        vectdeck[m] = temp;
    }
}

void printVectDeck()
{
    int k;
    for (k = 0; k < 52; k++) {
        vectdeck[k].printCard();
        std::cout << "\n";
    }
}