/// Menu.h
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

class Menu {
public:
    Menu();
    void draw(sf::RenderWindow& window);
    int handleEvents(sf::RenderWindow& window);

private:
    sf::Font font;
    sf::RectangleShape playButton;
    sf::RectangleShape settingsButton;
    sf::RectangleShape exitButton;
    sf::Texture BoardTexture;
    sf::Sprite Board, playSprite, exitSprite, settingSprite; 
    sf::Text playText;
    sf::Text settingsText;
    sf::Text exitText;

    bool isMouseOver(const sf::RectangleShape& button, sf::RenderWindow& window);
};