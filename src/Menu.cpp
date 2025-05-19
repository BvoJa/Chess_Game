/// Menu.cpp
#include "../include/Menu.h"
#include "../include/Constant.h"

Menu::Menu() {
    if (!font.loadFromFile("Assets/Fonts/arial.ttf")) {
        std::exit(-1);
    }

    BoardTexture.loadFromFile("Assets/images/Background.jpg");
    Board.setTexture(BoardTexture);

    playButton.setSize(sf::Vector2f(200, 50));
    settingsButton.setSize(sf::Vector2f(200, 50));
    exitButton.setSize(sf::Vector2f(200, 50));

    playButton.setPosition(300, 150);
    settingsButton.setPosition(300, 250);
    exitButton.setPosition(300, 350);

    playButton.setFillColor(sf::Color::Blue);    
    settingsButton.setFillColor(sf::Color::Blue);
    exitButton.setFillColor(sf::Color::Red);

    playText.setString("Play");
    settingsText.setString("Settings");
    exitText.setString("Exit");

    playText.setFont(font);
    settingsText.setFont(font);
    exitText.setFont(font);

    playText.setCharacterSize(24);
    settingsText.setCharacterSize(24);
    exitText.setCharacterSize(24);

    playText.setPosition(
        playButton.getPosition().x + (playButton.getSize().x - playText.getLocalBounds().width) / 2,
        playButton.getPosition().y + (playButton.getSize().y - playText.getLocalBounds().height) / 2 - 5
    );

    settingsText.setPosition(
        settingsButton.getPosition().x + (settingsButton.getSize().x - settingsText.getLocalBounds().width) / 2,
        settingsButton.getPosition().y + (settingsButton.getSize().y - settingsText.getLocalBounds().height) / 2 - 5
    );

    exitText.setPosition(
        exitButton.getPosition().x + (exitButton.getSize().x - exitText.getLocalBounds().width) / 2,
        exitButton.getPosition().y + (exitButton.getSize().y - exitText.getLocalBounds().height) / 2 - 5
    );
}

bool Menu::isMouseOver(const sf::RectangleShape& button, sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::FloatRect buttonBounds = button.getGlobalBounds();
    return buttonBounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
}

int Menu::handleEvents(sf::RenderWindow& window) {
    sf::Event event;

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (isMouseOver(playButton, window)) {
                return GamePlay_main; 
            }
            else if (isMouseOver(settingsButton, window)) {
                return Settings_main;
            }
            else if (isMouseOver(exitButton, window)) {
                window.close();
            }
        }

    }
    
    return -1; 
}

void Menu::draw(sf::RenderWindow& window) {
    window.draw(Board);
    Texture playText, exitText, settingText; 
    
    exitText.loadFromFile("Assets/images/exit.png");
    exitSprite.setTexture(exitText);
    exitSprite.setPosition(300, 350);

    playText.loadFromFile("Assets/images/play.png");
    playSprite.setTexture(playText);
    playSprite.setPosition(300, 150); 
    
    settingText.loadFromFile("Assets/images/settings.png");
    settingSprite.setTexture(settingText);
    settingSprite.setPosition(300, 250);

    Vector2i pos = Mouse::getPosition(window);

    Convert(playSprite, pos); 
    Convert(exitSprite, pos); 
    Convert(settingSprite, pos); 

    window.draw(playSprite);
    window.draw(exitSprite);
    window.draw(settingSprite); 
}
