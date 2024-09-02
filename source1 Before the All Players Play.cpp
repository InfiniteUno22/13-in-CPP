#include <iostream>
#include <SFML/Graphics.hpp>
#include "Header.h"
#include <vector>

int main()
{
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
    vectShuffle();
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
        if (i % 4 == 1) { p2.addCard(vectdeck[i]); }
        if (i % 4 == 2) { p3.addCard(vectdeck[i]); }
        if (i % 4 == 3) { p4.addCard(vectdeck[i]); }
    }

    //array for all players
    std::vector<player*> players = { &p1, &p2, &p3, &p4 };

    //sorts and assigns hand
    for (auto p : players) {
        p->sortHand();
        p->numberHand();
    }

    int counter = 0;
    p1.printHand();
    p1.setupHand();
    sf::Text text;
    text.setFont(font);
    text.setString(std::to_string(p1.yourHand.size()));
    text.setCharacterSize(52);
    text.setFillColor(sf::Color::White);
    text.setPosition(window.getSize().x / 2 - text.getCharacterSize() / 2, 360);

    sf::Text pnum;
    pnum.setFont(font);
    pnum.setString("P" + std::to_string(p1.number));
    pnum.setCharacterSize(52);
    pnum.setFillColor(sf::Color::White);
    pnum.setPosition(30.f, 525.f);

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

            //constantly updating mouse position
            auto mouse_pos = sf::Mouse::getPosition(window); // Mouse position relative to the window
            auto translated_pos = window.mapPixelToCoords(mouse_pos); // Mouse position translated into world coordinates
            if (event.type == sf::Event::MouseButtonPressed) {
                std::cout << "(" << translated_pos.x << ", " << translated_pos.y << " )";
            }
            if (play.getGlobalBounds().contains(translated_pos)) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    std::cout << "You pressed play" << std::endl;
                    for (int i = p1.yourHand.size() - 1; i >= 0; i--) {
                        if (p1.yourHand[i].getIsSelected()) {
                            p1.playCard(i + 1);
                        }
                    }
                }
                
            }
            if (pass.getGlobalBounds().contains(translated_pos)) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    std::cout << "You pressed pass" << std::endl;
                }
            }
            // while the turn player has cards in hand
            if (!p1.yourHand.empty()) {
                //check for a mouseclick
                if (event.type == sf::Event::MouseButtonPressed) {
                    // for each card in your hand, it checks if the mouseclick was on them
                    // this checks from high cards to low
                    for (int i = p1.yourHand.size() - 1; i >= 0; i--) {
                        if (p1.yourHand[i].sprite.getGlobalBounds().contains(translated_pos)) {
                            // moves the card dwon
                            if (p1.yourHand[i].getIsSelected()) {
                                std::cout << "You deselected this card" << std::endl;
                                p1.yourHand[i].setIsSelected(false);
                                p1.yourHand[i].sprite.move(0.f, 40.f);
                                break;
                            }
                            else {
                                std::cout << "You selected this card" << std::endl;
                                p1.yourHand[i].printCard();
                                std::cout << std::endl;
                                p1.yourHand[i].setIsSelected(true);
                                p1.yourHand[i].sprite.move(0.f, -40.f);
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
        for (int i = 0; i < p1.yourHand.size(); i++) {
            window.draw(p1.yourHand[i].sprite);
        }
        window.draw(play);
        window.draw(pass);

        text.setString(std::to_string(p1.yourHand.size()));
        window.draw(text);
        window.draw(pnum);
        p1.updateHand();

        window.display();
    }

    return 0;
}