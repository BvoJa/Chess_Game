/// Mode.h
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
using namespace sf; 
using namespace std; 

class Mode {
public:
    Mode();
    void draw(sf::RenderWindow& window);
    int handleEvents(sf::RenderWindow& window);

private:
    sf::Font font;
    sf::RectangleShape Two_Players;
    sf::RectangleShape One_Player;
    sf::RectangleShape Back;
    sf::Texture BoardTexture;
    sf::Sprite Board;
    sf::Text playText;
    sf::Text settingsText;
    sf::Text exitText;

    bool isMouseOver(const sf::RectangleShape& button, sf::RenderWindow& window);
};