#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include<string>

using namespace sf;
using namespace std;


class Backgroundmusic {
public:
    Backgroundmusic();
    void draw(RenderWindow& window);
    void SetMusic(short id); 
    int handleEvent(RenderWindow& window);
    void Play(); 
private:
    bool isMove; 
    sf::Text backButtonText;
    sf::RectangleShape backButton;
    sf::Texture BoardTexture, VolumeTexture, MuteTexture;
    sf::Sprite Board, VolumeSprite, MuteSprite;
    Font font; 
    Music music;
    float volume = 20;
    
    sf::RectangleShape sliderBar;
    sf::CircleShape sliderKnob;
    short musicIndex;

    Sprite musicOptions[3];
    Texture musicTextures[3];

};
