// Settings.cpp
#include "../include/Settings.h"
#include "../include/Constant.h"
#include <iostream>

vector<string>boardDesignPaths = {
    "Assets/images/BoardGame/background0.png","Assets/images/BoardGame/background5.png", "Assets/images/BoardGame/background3.png"
};

vector<string>pieceDesignPaths = { "Assets/images/FigureTheme/Class/background.png",
"Assets/images/FigureTheme/Classic/background.png", "Assets/images/FigureTheme/Modern/background.png" };

Settings::Settings() {

    soundEffectsOn = backgroundMusicOn = pieceTheme = musicAdjust = boardTheme = musicSoundThemes = 0;
    lst_access = Initial_main; 

    BoardTexture.loadFromFile("Assets/images/setting.jpg");
    Board.setTexture(BoardTexture);

    selectedPieceThemeIndex = selectedBoardThemeIndex = 1; 

    for (int i = 0; i < 3; i++) {
        if (pieceDesignTextures[i].loadFromFile(pieceDesignPaths[i])) {
            pieceDesignOptions[i].setTexture(pieceDesignTextures[i]);
            pieceDesignOptions[i].setPosition(100 + i * 200, 200);
        }
    }


    for (int i = 0; i < 3; i++) {
        if (boardDesignTextures[i].loadFromFile(boardDesignPaths[i])) {
            boardDesignOptions[i].setTexture(boardDesignTextures[i]);
            boardDesignOptions[i].setPosition(150 + i * 200, 200);
        }
    }

    boardColorButton.setSize(sf::Vector2f(200, 50));
    boardColorButton.setPosition(300, 100);
    boardColorButton.setFillColor(sf::Color::Blue);

    pieceDesignButton.setSize(sf::Vector2f(200, 50));
    pieceDesignButton.setPosition(300, 200);
    pieceDesignButton.setFillColor(sf::Color::Blue);

    soundEffectsButton.setSize(sf::Vector2f(200, 50));
    soundEffectsButton.setPosition(300, 300);
    soundEffectsButton.setFillColor(sf::Color::Blue);

    backgroundMusicButton.setSize(sf::Vector2f(200, 50));
    backgroundMusicButton.setPosition(300, 400);
    backgroundMusicButton.setFillColor(sf::Color::Blue);

    backButton.setSize(sf::Vector2f(100, 65));
    backButton.setPosition(20, 10);
    backButton.setFillColor(sf::Color::Red);
}

void Settings::draw(RenderWindow& window) {
    window.draw(Board);

    Texture backLeftT; 
    Sprite backLeft; 

    backLeftT.loadFromFile("Assets/images/left.png");
    backLeft.setTexture(backLeftT);
    backLeft.setPosition(15, 10);

    Vector2i pos = Mouse::getPosition(window);
    Convert(backLeft, pos); 


    window.draw(backLeft);


    if (pieceTheme) {
        for (int i = 0; i < 3; i++) {
            if (i == selectedPieceThemeIndex) {
                sf::RectangleShape highlight(sf::Vector2f(pieceDesignOptions[i].getGlobalBounds().width, pieceDesignOptions[i].getGlobalBounds().height));
                highlight.setPosition(pieceDesignOptions[i].getPosition());
                highlight.setFillColor(sf::Color::Transparent);
                highlight.setOutlineColor(sf::Color::Red);
                highlight.setOutlineThickness(3);
                window.draw(highlight);
            }

            window.draw(pieceDesignOptions[i]);
        }
        return;
    }

    if (boardTheme) {
        for (int i = 0; i < 3; i++) {
            if (i == selectedBoardThemeIndex) {
                sf::RectangleShape highlight(sf::Vector2f(boardDesignOptions[i].getGlobalBounds().width, boardDesignOptions[i].getGlobalBounds().height));
                highlight.setPosition(boardDesignOptions[i].getPosition());
                highlight.setFillColor(sf::Color::Transparent);
                highlight.setOutlineColor(sf::Color::Red);
                highlight.setOutlineThickness(3);
                window.draw(highlight);
            }

            window.draw(boardDesignOptions[i]);
        }
        return;
    }

    Texture boardthemeT, piecethemeT, soundeffectT, backgroundmusicT; 
    Sprite boardtheme, piecetheme, soundeffect, backgroundmusic;

    boardthemeT.loadFromFile("Assets/images/boardtheme.png");
    piecethemeT.loadFromFile("Assets/images/piecetheme.png");
    soundeffectT.loadFromFile("Assets/images/soundeffect.png");
    backgroundmusicT.loadFromFile("Assets/images/backgroundmusic.png");

    boardtheme.setTexture(boardthemeT); 
    piecetheme.setTexture(piecethemeT); 
    soundeffect.setTexture(soundeffectT); 
    backgroundmusic.setTexture(backgroundmusicT); 

    boardtheme.setPosition(300, 100);
    piecetheme.setPosition(300, 200); 
    soundeffect.setPosition(300, 300); 
    backgroundmusic.setPosition(300, 400);

    Convert(boardtheme, pos); 
    Convert(piecetheme, pos); 
    Convert(soundeffect, pos); 
    Convert(backgroundmusic, pos); 

    window.draw(boardtheme); 
    window.draw(piecetheme); 
    window.draw(soundeffect); 
    window.draw(backgroundmusic); 
}

int Settings::handleEvent(RenderWindow& window) {
    Event event;

    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) window.close();

        Vector2i pos = Mouse::getPosition(window);

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left) {
            if (pieceTheme) {
                pieceTheme = 0;
                return Settings_main;
            }

            if (boardTheme) {
                boardTheme = 0;
                return Settings_main;
            }

            return lst_access;
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (backButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                if (pieceTheme) {
                    pieceTheme = 0;
                    return Settings_main;
                }

                if (boardTheme) {
                    boardTheme = 0;
                    return Settings_main;
                }

                return lst_access; 
            }

            bool comb = pieceTheme | boardTheme;

            if (pieceTheme) {
                for (int i = 0; i < 3; i++) {
                    if (pieceDesignOptions[i].getGlobalBounds().contains(pos.x, pos.y)) {
                        selectedPieceThemeIndex = i; 
                    }
                }
            }

            if (boardTheme) {
                for (int i = 0; i < 3; i++) {
                    if (boardDesignOptions[i].getGlobalBounds().contains(pos.x, pos.y)) {
                        selectedBoardThemeIndex = i;
                    }
                }
            }

            if (comb == 0 && pieceDesignButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) pieceTheme = 1;

            if (comb == 0 && boardColorButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) boardTheme = 1;

            if (comb == 0) {
                if (soundEffectsButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    return Soundeffect_main;
                }

                if (backgroundMusicButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    return Backgroundmusic_main;
                }
            }
        }
    }

    return Settings_main;
}
string Settings::getPiecePath() {
    string path = ""; 
    for (char c : pieceDesignPaths[selectedPieceThemeIndex]) {
        if (c == 'b') return path; 
        path += c; 
    }

    return path; 
}

char Settings::getBoard() {
    for (char c : boardDesignPaths[selectedBoardThemeIndex]) {
        if (isdigit(c)) return c;
    }

    return '0';
}