#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <vector>
#include<string>

using namespace sf;
using namespace std;

struct SoundControl {
    sf::Text label;               
    sf::RectangleShape sliderBar; 
    sf::CircleShape knob;
    sf::RectangleShape testButton, box; 
    sf::Text boxlabel;         
    bool isChecked = 0;  
    float volume = 50; 
    bool ismove = 0; 

    SoundControl(); 
    SoundControl(const Font &font, const string &name, float y);
};

class SoundEffect {
public:
    SoundEffect();
    void draw(RenderWindow& window);
    int handleEvent(RenderWindow& window);
    SoundControl control[5];
private:
    sf::Texture BoardTexture;
    sf::Sprite Board;
    sf::Font font;
    sf::Text backButtonText;
    sf::RectangleShape backButton;
    SoundBuffer Buffer[5];  
    Sound sound[5]; 
};
