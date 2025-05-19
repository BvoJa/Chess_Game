
#include "../include/OnePlayerMode.h"
#include "../include/Constant.h"

OnePlayerMode::OnePlayerMode() {
    BoardTexture.loadFromFile("Assets/images/oneplayer.jpg");
    Board.setTexture(BoardTexture);

    easyButton.setSize(sf::Vector2f(200, 50));
    easyButton.setPosition(300, 100);
    easyButton.setFillColor(sf::Color::Blue);

    mediumButton.setSize(sf::Vector2f(200, 50));
    mediumButton.setPosition(300, 200);
    mediumButton.setFillColor(sf::Color::Blue);

    hardButton.setSize(sf::Vector2f(200, 50));
    hardButton.setPosition(300, 300);
    hardButton.setFillColor(sf::Color::Blue);

    backButton.setSize(sf::Vector2f(200, 50));
    backButton.setPosition(300, 400);
    backButton.setFillColor(sf::Color::Red);

    WhiteButton.setSize(sf::Vector2f(200, 50));
    BlackButton.setSize(sf::Vector2f(200, 50));
    BackTo.setSize(sf::Vector2f(200, 50));

    WhiteButton.setPosition(300, 150);
    BlackButton.setPosition(300, 250);
    BackTo.setPosition(300, 350);

    WhiteButton.setFillColor(sf::Color::Blue);
    BlackButton.setFillColor(sf::Color::Blue);
    BackTo.setFillColor(sf::Color::Red);

    selectMode = PlayasBlack = Hard = Easy = Medium = 0;

    blackText.loadFromFile("Assets/images/black.png");
    blackButton.setTexture(blackText); 
    blackButton.setPosition(300, 250);

    whiteText.loadFromFile("Assets/images/white.png");
    whiteButton.setTexture(whiteText);
    whiteButton.setPosition(300, 150);
}

void OnePlayerMode::draw(RenderWindow& window) {
    Text  White, Black, Backto; 

    Texture easyT, mediumT, hardT, backT; 
    
    easyT.loadFromFile("Assets/images/easy.png"); 
    mediumT.loadFromFile("Assets/images/medium.png"); 
    hardT.loadFromFile("Assets/images/hard.png"); 
    backT.loadFromFile("Assets/images/back.png"); 

    Sprite easy, medium, hard, back; 
    easy.setTexture(easyT);
    medium.setTexture(mediumT);
    hard.setTexture(hardT);
    back.setTexture(backT); 

    easy.setPosition(300, 100);
    medium.setPosition(300, 200);
    hard.setPosition(300, 300);
    back.setPosition(300, 400);

    Backto.setFont(font);
    Backto.setString("Back");
    Backto.setCharacterSize(24);

    Backto.setPosition(
        BackTo.getPosition().x + (BackTo.getSize().x - Backto.getLocalBounds().width) / 2,
        BackTo.getPosition().y + (BackTo.getSize().y - Backto.getLocalBounds().height) / 2 - 5
    );

    if (selectMode) {
        BoardTexture.loadFromFile("Assets/images/playas.jpg");
        Board.setTexture(BoardTexture);
    }
    else {
        BoardTexture.loadFromFile("Assets/images/oneplayer.jpg");
        Board.setTexture(BoardTexture);
    }

    window.draw(Board);

    if (selectMode == 0) {
        Vector2i pos = Mouse::getPosition(window);

        Convert(easy, pos); 
        Convert(medium, pos); 
        Convert(hard, pos); 
        Convert(back, pos);

        window.draw(easy);
        window.draw(medium);
        window.draw(hard);
        window.draw(back); 
    }
    else {
        back.setPosition(300, 350);
        Vector2i pos = Mouse::getPosition(window);

        Convert(blackButton, pos); 
        Convert(whiteButton, pos);
        Convert(back, pos);

        window.draw(blackButton); 
        window.draw(whiteButton); 
        window.draw(back); 
    }
}

bool OnePlayerMode::EASY() {
    return Easy; 
}

bool OnePlayerMode::MEDIUM() {
    return Medium;
}

bool OnePlayerMode::HARD() {
    return Hard;
}

int OnePlayerMode::handleEvent(RenderWindow& window) {
    Event event;

    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) window.close();

        Vector2i pos = Mouse::getPosition(window);

        if (event.type == sf::Event::KeyPressed && event.key.code == Keyboard::Left) {

            if (selectMode) {
                selectMode = 0; 
                return OnePlayer_main;
            }

            return GamePlay_main; 
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (selectMode) {
                if (WhiteButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    PlayasBlack = 0; selectMode = 0; 
                    return Playaswhite_main; 
                }

                if (BlackButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    PlayasBlack = 1; selectMode = 0;    
                    return Playasblack_main;
                }

                if (BackTo.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    selectMode = 0; 
                    return OnePlayer_main;
                }
            }
            else {
                if (backButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    return GamePlay_main;
                }

                if (easyButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    selectMode = 1; Easy = 1; Hard = Medium = 0; 
                    return OnePlayer_main;
                }

                if (mediumButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    selectMode = 1; Medium = 1; Hard = Easy = 0; 
                    return OnePlayer_main;
                }

                if (hardButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    selectMode = 1; Hard = 1, Medium = Easy = 0; 
                    return OnePlayer_main;
                }
            }
        }
    }

    return OnePlayer_main;
}
