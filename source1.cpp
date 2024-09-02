#include <iostream>
#include <SFML/Graphics.hpp>
#include "Header.h"
#include <vector>

int main(){
    // setup any shapes
    sf::RenderWindow window(sf::VideoMode(1200, 675), "Tien Len");
   
    // loading in font
    sf::Font font;
    font.loadFromFile("assets/zh-cn.ttf");

    // play button
    sf::Texture texture1;
    texture1.loadFromFile("assets/play_button.png");
    
    sf::Sprite play;
    play.setTexture(texture1);
    play.setScale(sf::Vector2f(0.1f, 0.1f));
    play.setPosition(1050.f, 375.f);

    // pass button
    sf::Texture texture2;
    texture2.loadFromFile("assets/pass_button.png");

    sf::Sprite pass;
    pass.setTexture(texture2);
    pass.setScale(sf::Vector2f(0.13f, 0.13f));
    pass.setPosition(910.f, 375.f);

    //hide button
    sf::Texture hide_texture;
    hide_texture.loadFromFile("assets/hide_button.png");

    sf::Sprite hide;
    hide.setTexture(hide_texture);
    hide.setScale(sf::Vector2f(0.1f, 0.1f));
    hide.setPosition(750.f, 375.f);
    
    // card hider banner
    sf::Texture banner_texture;
    banner_texture.loadFromFile("assets/banner.png");

    sf::Sprite banner;
    banner.setTexture(banner_texture);
    //banner.setScale(sf::Vector2f(0.f, 0.1f));
    banner.setPosition(110.f, 465.f);
    
    //creating the cards
    char rank[] = { '3','4','5','6','7','8','9','T','J','Q','K','A','2' };
    std::string suit[] = { "Spades","Clubs","Diamonds","Hearts" };

    sf::Texture texture;
    std::vector<sf::Texture> allTextures(52, texture);
    for (int i = 1; i <= allTextures.size(); i++) {
        allTextures[i-1].loadFromFile("assets/" + std::to_string(i) + ".png");
    }

    sf::Sprite sprite;
    //Vector Deck making
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            sprite.setTexture(allTextures[(i * 13) + (j)]);
            card newCard(rank[j], suit[i], j, i, 1, sprite);
            vectdeck.push_back(newCard);
        }
        std::cout << std::endl;
    }
    
    float resizer = 0.15f;
    for (int i = 0; i < vectdeck.size(); i++) {
        vectdeck[i].resizeCard(resizer, resizer);
    }

    //shuffle();
    //vectShuffle();
    //printdeck(); 
    // Define players
    player p1(1, false, false);
    player p2(2, false, false);
    player p3(3, false, false);
    player p4(4, false, false);

    //Dealing the cards to each player
    
    for (int i = 0; i < 52; i++)
    {
        if (i % 4 == 0) { p1.addCard(vectdeck[i]); }
        if (i % 2 == 0) { p2.addCard(vectdeck[i]); }
        if (i % 1 == 0) { p3.addCard(vectdeck[i]); }
        if (i % 1 == 0) { p4.addCard(vectdeck[i]); }
    }

    //array for all players
    std::vector<player*> players = { &p1, &p2, &p3, &p4 };

    //sorts and assigns hand
    for (auto p : players) {
        p->sortHand();
        p->numberHand();
    }
    //3 of Spades player gains prio
    for (auto p : players) {                
        if (p->has3S()) {
            std::cout << std::endl;
            p->setPrio(true);
            break;
        }
    }

    // GAME VARIABLES
    int playersLeft = 4;
    int counter = 0;
    int passCount = 0;
    int numberPlayed = 0;
    bool firstPlay = true;
    int current = 0;
    bool isHide = true;
    int currPlayed = 0;
    bool isValid = true;
    bool isPlayable = false;
    bool cut = false;

    //Setting up the intial hand on screen and in terminal
    players[0]->printHand();
    players[0]->setupHand(470.f);

    //Prints your cards remaining
    sf::Text text;
    text.setFont(font);
    text.setString(std::to_string(players[0]->yourHand.size()));
    text.setCharacterSize(52);
    text.setFillColor(sf::Color::White);
    text.setPosition(window.getSize().x / 2 - text.getCharacterSize() / 2, 360);

    // Prints the Current Player
    sf::Text pnum;
    pnum.setFont(font);
    pnum.setString("P" + std::to_string(p1.number));
    pnum.setCharacterSize(52);
    pnum.setFillColor(sf::Color::White);
    pnum.setPosition(30.f, 525.f);

    // The Play Pile
    player playPile(0, false, false);

    // prints the width of the resized 
    //std::cout << p1.yourHand[0].sprite.getTexture()->getSize().x * resizer << std::endl;  

    //main application loop
    while (window.isOpen()){

        //handle events
        sf::Event event;
        
        while (window.pollEvent(event))
        {
            //terminate application
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.scancode == sf::Keyboard::Scan::Escape) {
                    window.close();
                }
            }
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            //resets the round after sufficient passes
            if (passCount == playersLeft - 1) {
                // rotates players based on prio
                std::cout << "you have been rotated" << std::endl;
                for (int i = 0; i < playersLeft; i++) {
                    if (players[i]->getPrio()) {
                        std::rotate(players.begin(), players.begin() + i, players.end());
                    }
                    if (!players[i]->getIsOut()) {
                        players[i]->setHasPassed(false);
                    }
                    
                }
                passCount = 0;
                current = 0;
                numberPlayed = 0;
                firstPlay = true;
                playPile.yourHand.clear();
                currPlayed = 0;
                cut = false;
            }
            //constantly updating mouse position
            auto mouse_pos = sf::Mouse::getPosition(window); // Mouse position relative to the window
            auto translated_pos = window.mapPixelToCoords(mouse_pos); // Mouse position translated into world coordinates
            /*
            if (event.type == sf::Event::MouseButtonPressed) {
                std::cout << "(" << translated_pos.x << ", " << translated_pos.y << " )";
            }
            */
            if (play.getGlobalBounds().contains(translated_pos)) {
                // PLAY BUTTON
                if (event.type == sf::Event::MouseButtonPressed) {
                    std::cout << std::endl;
                    std::cout << "You pressed play" << std::endl;
                    for (int i = 0; i < players[current]->yourHand.size(); i++) {
                        if (players[current]->yourHand[i].getIsSelected()) {
                            currPlayed += 1;
                            // sets the number of cards each player has to play
                            if (firstPlay) {
                                numberPlayed = currPlayed;
                            }
                        }
                    }
                    std::cout << "this is the numberPlayed: " << numberPlayed << std::endl;
                    std::cout << "this is the currPlayed: " << currPlayed << std::endl;
                    //playing cuts
                    
                    if (!firstPlay) {
                        if (playPile.has2() || cut) {
                            if (players[current]->isDubSeq(currPlayed) && (players[current]->getCutStrength(currPlayed) >= playPile.getCutStrength(playPile.yourHand.size()))) {
                                std::cout << "You played a double sequence" << std::endl;
                                isPlayable = true;
                                cut = true;
                            }
                            else if (players[current]->isQuad(currPlayed) && (players[current]->getCutStrength(currPlayed) > playPile.getCutStrength(playPile.yourHand.size()))) {
                                std::cout << "You played a quad" << std::endl;
                                isPlayable = true;
                                cut = true;
                            }
                            else {
                                isPlayable = false;
                            }
                        }
                    }
                    if (cut || currPlayed == numberPlayed ) {
                        // check isHigher
                        card temp1;
                        for (int i = players[current]->yourHand.size() - 1; i >= 0; i--) {
                            if (players[current]->yourHand[i].getIsSelected()) {
                                temp1 = players[current]->yourHand[i];
                                break;
                            }
                        }
                        bool cardBigger = true;
                        if (!firstPlay) { cardBigger = temp1.isHigher(playPile.yourHand.back()); }
                        // checks if you played a duplicate 
                        if (players[current]->isQuad(currPlayed)) {
                            std::cout << "You played a Quad." << std::endl;
                            isPlayable = true;
                        }
                        else if (players[current]->isDupe(currPlayed)) {
                            std::cout << "You played " << currPlayed << " duplicates." << std::endl;
                            isPlayable = true;
                        }
                        // checks if you played a sequence
                        else if (players[current]->isSeq(currPlayed) && !cut) {
							std::cout << "You played a sequence of " << currPlayed << "." << std::endl;
							isPlayable = true;
                        }
                        else if (players[current]->isSingle(currPlayed)){
							std::cout << "You played a single of " << currPlayed << "." << std::endl;
                            isPlayable = true;
                            
                        }
                        
                        if ((isPlayable && cardBigger) || (isPlayable && cut)) {
                            // Playing Cards Logic
                            playPile.yourHand.clear();
                            for (int i = players[current]->yourHand.size() - 1; i >= 0; i--) {
                                if (players[current]->yourHand[i].getIsSelected()) {
                                    playPile.yourHand.push_back(players[current]->yourHand[i]);
                                    players[current]->playCard(i + 1);
                                }
                            }
                            playPile.sortHand();
                            playPile.numberHand();
                            // prints play pile in terminal
                            for (int i = 0; i < playPile.yourHand.size(); i++) {
                                playPile.yourHand[i].printCard();
                            }
                            currPlayed = 0;
                            isValid = true;
                            isPlayable = false;
                        }
                        else {
                            // might use a goto here
                            isValid = false;
                            currPlayed = 0;
                        }
                        
                    }
                    // You played more cards than the first player played
                    else if (currPlayed > numberPlayed) 
                    {
                        std::cout << "You have too many cards selected" << std::endl;
                        currPlayed = 0;
                        isValid = false;
                    }
                    // you played less cards than the first player
                    else 
                    {
                        std::cout << "You have too little cards selected" << std::endl;
                        currPlayed = 0;
                        isValid = false;
                    }

                    //if you win, you are accounted for 
                    if (players[current]->yourHand.empty()) {
                        std::cout << "YOU ARE OUT\n";
                        playersLeft -= 1;
                        players[current]->setIsOut(true);
                        players[current]->setHasPassed(true);
                    }

                    if (isValid) 
                    {
                        //moving the last hand offscreen
                        players[current]->setupHand(-100.f);
                        // moves us pass players that have passed already
                        do 
                        {
                            // if it's the last player in rotation we have to loop back to start
                            if (current == 3) 
                            {
                                current = -1;
                            }
                            current += 1;
                        } while (players[current]->getHasPassed());

                        //updates revealed info to the current players
                        players[current]->print();
                        players[current]->setupHand(470.f);
                        std::cout << "Current current: " << current << std::endl;
                        pnum.setString("P" + std::to_string(players[current]->getNumber()));
                        text.setString(std::to_string(players[current]->yourHand.size()));
                        // hides the next player's cards
                        banner.setPosition(110.f, 465.f);
                        isHide = true;
                        firstPlay = false;
                    }
                }
            }
            //  PASS BUTTON
            if (pass.getGlobalBounds().contains(translated_pos)) 
            {
                if (event.type == sf::Event::MouseButtonPressed) 
                {
                    std::cout << "You pressed pass" << std::endl;
                    if (!firstPlay) {
                        //moving the last hand offscreen
                        players[current]->setupHand(-100.f);
                        players[current]->setHasPassed(true);
                        players[current]->setPrio(false);
                        // moves us pass players that have passed already
                        do
                        {
                            // if it's the last player in rotation we have to loop back to start
                            if (current == 3)
                            {
                                current = -1;
                            }
                            current += 1;
                        } while (players[current]->getHasPassed());
                        //updates revealed info to the current players
                        players[current]->print();
                        players[current]->setupHand(470.f);
                        std::cout << "Current current: " << current << std::endl;
                        pnum.setString("P" + std::to_string(players[current]->getNumber()));
                        text.setString(std::to_string(players[current]->yourHand.size()));
                        // hides the next player's cards
                        banner.setPosition(110.f, 465.f);
                        isHide = true;
                        //updates passCount
                        passCount += 1;
                        std::cout << "This is the pass count: " << passCount << std::endl;
                        if (passCount == playersLeft - 1)
                        {
                            players[current]->setPrio(true);
                        }
                    }
                }
            }
            //  HIDE BUTTON
            if (hide.getGlobalBounds().contains(translated_pos)) 
            {
                if (event.type == sf::Event::MouseButtonPressed) 
                {
                    if (isHide) 
                    {
                        std::cout << "You revealed the cards" << std::endl;
                        banner.setPosition(110.f, 800.f);
                        isHide = false;
                    }
                    else 
                    {
                        std::cout << "You hid the cards" << std::endl;
                        banner.setPosition(110.f, 465.f);
                        isHide = true;
                    }
                }
            }
            // while the turn player has cards in hand
            if (!players[current]->yourHand.empty()) 
            {
                //check for a mouseclick
                if (event.type == sf::Event::MouseButtonPressed) 
                {
                    // for each card in your hand, it checks if the mouseclick was on them
                    // this checks from high cards to low
                    for (int i = players[current]->yourHand.size() - 1; i >= 0; i--) {
                        if (players[current]->yourHand[i].sprite.getGlobalBounds().contains(translated_pos)) 
                        {
                            // moves the card dwon
                            if (players[current]->yourHand[i].getIsSelected()) 
                            {
                                std::cout << "You deselected this card" << std::endl;
                                players[current]->yourHand[i].setIsSelected(false);
                                players[current]->yourHand[i].sprite.move(0.f, 40.f);
                                break;
                            }
                            else 
                            {
                                std::cout << "You selected this card" << std::endl;
                                players[current]->yourHand[i].printCard();
                                std::cout << std::endl;
                                players[current]->yourHand[i].setIsSelected(true);
                                players[current]->yourHand[i].sprite.move(0.f, -40.f);
                                break;
                            }
                        }
                    }

                }
            }
            
            
            
        }

        //clear window
        
        window.clear();
        //draw shapes
        for (int i = 0; i < players[current]->yourHand.size(); i++) 
        {
            window.draw(players[current]->yourHand[i].sprite);
        }

        for (int i = 0; i < playPile.yourHand.size(); i++) 
        {
            window.draw(playPile.yourHand[i].sprite);
        }
        window.draw(play);
        window.draw(pass);
        window.draw(hide);
        window.draw(banner);

        window.draw(text);
        window.draw(pnum);
        players[current]->updateHand();
        playPile.setupHand(100.f);

        window.display();
    }

    return 0;
}