// Settings.h
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include<string>

using namespace sf;
using namespace std;


class OnePlayerMode {
public:
    OnePlayerMode();
    void draw(RenderWindow& window);
    int handleEvent(RenderWindow& window);
    bool EASY(); 
    bool MEDIUM(); 
    bool HARD(); 
private:
    bool selectMode, Hard, Easy, Medium, PlayasBlack;
    sf::Font font;
    
    sf::RectangleShape easyButton;
    sf::RectangleShape mediumButton;
    sf::RectangleShape hardButton;
    sf::RectangleShape backButton;
    sf::RectangleShape WhiteButton;
    sf::RectangleShape BlackButton;
    sf::RectangleShape BackTo;

    sf::Texture BoardTexture, blackText, whiteText;
    sf::Sprite Board, blackButton, whiteButton;
};
