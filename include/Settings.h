// Settings.h
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include<string>

using namespace sf; 
using namespace std; 


class Settings {
public:
    Settings();
    string getPiecePath();
    char getBoard();

    void draw(RenderWindow& window);
    int handleEvent(RenderWindow& window);
    int lst_access = -1;

private:

    sf::Font font;
    sf::RectangleShape boardColorButton;
    sf::RectangleShape pieceDesignButton;
    sf::RectangleShape soundEffectsButton;
    sf::RectangleShape backgroundMusicButton;

    sf::Texture BoardTexture;
    sf::Sprite Board;
    sf::RectangleShape backButton;

    bool soundEffectsOn;
    bool backgroundMusicOn;
    bool pieceTheme;
    bool musicAdjust;
    bool boardTheme;
    bool musicSoundThemes;

    Sprite pieceDesignOptions[3];
    Texture pieceDesignTextures[3];

    Sprite boardDesignOptions[3];
    Texture boardDesignTextures[3];

    int selectedPieceThemeIndex;
    int selectedBoardThemeIndex;
};
