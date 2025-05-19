#include "../include/SoundEffect.h"
#include "../include/Constant.h"

vector<string> Soundssss = {
	"Assets/Sound/move-self.mp3",
	"Assets/Sound/capture.mp3",
	"Assets/Sound/move-check.mp3",
	"Assets/Sound/castle.mp3",
	"Assets/Sound/promote.mp3"
};

SoundControl::SoundControl() {

}

SoundControl::SoundControl(const Font &font, const string& name, float y) {
	// Label
	volume = 50; 
	ismove = 0; 
	label.setFont(font);
	label.setString(name);
	label.setCharacterSize(32);
	label.setFillColor(sf::Color::Yellow);
	label.setPosition(70, y);

	// Slider
	sliderBar.setSize(sf::Vector2f(200, 10));
	sliderBar.setFillColor(sf::Color(192, 192, 192));
	sliderBar.setOutlineThickness(2);
	sliderBar.setOutlineColor(sf::Color::Black);
	sliderBar.setPosition(200, y + 15);

	// Knob
	knob.setRadius(9);
	knob.setFillColor(sf::Color(100, 200, 255)); 
	knob.setOutlineThickness(2);
	knob.setOutlineColor(sf::Color::White);
	
	knob.setPosition(200 + (volume * 2) - knob.getRadius(), y + 10);

	// Test Button
	testButton.setSize(sf::Vector2f(60, 30));
	testButton.setFillColor(sf::Color::Blue);
	testButton.setOutlineThickness(2);
	testButton.setOutlineColor(sf::Color::Black);
	testButton.setPosition(450, y);

	box.setSize(sf::Vector2f(20, 20));
	box.setOutlineThickness(2);
	box.setOutlineColor(sf::Color::White);
	box.setFillColor(sf::Color::Green);
	box.setPosition(550, y);

	isChecked = 1; 

	string tmp = "Enable ";
	tmp += name;
	tmp += " Sound";
	boxlabel.setFont(font);
	boxlabel.setString(tmp);
	boxlabel.setCharacterSize(20);
	boxlabel.setFillColor(sf::Color::White);
	boxlabel.setPosition(580, y - 5);
}

SoundEffect::SoundEffect() {
	BoardTexture.loadFromFile("Assets/images/sound.jpg");
	Board.setTexture(BoardTexture);
	if (!font.loadFromFile("Assets/Fonts/Arial.ttf")) {
		std::exit(-1);
	}

	control[0] = SoundControl(font, "Move", 150);
	control[1] = SoundControl(font, "Capture", 150 + 70);
	control[2] = SoundControl(font, "Check", 150 + 70 * 2);
	control[3] = SoundControl(font, "Castle", 150 + 70 * 3);
	control[4] = SoundControl(font, "Promote", 150 + 70 * 4);

	for (int i = 0; i <= 4; i++) {
		Buffer[i].loadFromFile(Soundssss[i]);
		sound[i].setBuffer(Buffer[i]);
		sound[i].setVolume(50);
	}

	backButton.setSize(sf::Vector2f(100, 40));
	backButton.setPosition(20, 20);
	backButton.setFillColor(sf::Color::Red);

	backButtonText.setFont(font);
	backButtonText.setString("Back");
	backButtonText.setCharacterSize(20);
	backButtonText.setFillColor(sf::Color::White);

	backButtonText.setPosition(
		backButton.getPosition().x + (backButton.getSize().x - backButtonText.getGlobalBounds().width) / 2,
		backButton.getPosition().y + (backButton.getSize().y - backButtonText.getGlobalBounds().height) / 4
	);
}

int SoundEffect::handleEvent(RenderWindow &window) {
	Event event;

	while (window.pollEvent(event)) {
		if (event.type == Event::Closed) window.close();

		Vector2i pos = Mouse::getPosition(window);
		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
			if (backButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
				return Settings_main;
			}
		}


		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left) {
			return Settings_main;
		}

		for (int i = 0; i <= 4; i++) {
			if (control[i].sliderBar.getGlobalBounds().contains(pos.x, pos.y))
				control[i].sliderBar.setFillColor(sf::Color::Yellow); 
			else 
				control[i].sliderBar.setFillColor(sf::Color(192, 192, 192)); 

			if (control[i].isChecked == 0) control[i].volume = 0; 
			else if (control[i].volume < 1) control[i].volume = 50;

			if (control[i].testButton.getGlobalBounds().contains(pos.x, pos.y) && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				control[i].testButton.setFillColor(sf::Color::Green);
				sound[i].setVolume(control[i].volume); 
				sound[i].play(); 
			}else control[i].testButton.setFillColor(sf::Color::Blue);


			if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					float sliderStartX = control[i].sliderBar.getPosition().x;
					float sliderEndX = sliderStartX + control[i].sliderBar.getSize().x;

					if (control[i].knob.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) ||
						(event.mouseButton.x >= sliderStartX && event.mouseButton.x <= sliderEndX &&
							event.mouseButton.y >= control[i].sliderBar.getPosition().y &&
							event.mouseButton.y <= control[i].sliderBar.getPosition().y + control[i].sliderBar.getSize().y)) {

						control[i].volume = max(0.01, ((event.mouseButton.x - sliderStartX) / control[i].sliderBar.getSize().x) * 100.0);
						control[i].knob.setPosition(sliderStartX + (control[i].volume / 100) * control[i].sliderBar.getSize().x - control[i].knob.getRadius(), control[i].knob.getPosition().y);
					}
				}
			}

			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					if (control[i].knob.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
						control[i].ismove = true;
					}
				}
			}

			if (event.type == sf::Event::MouseButtonReleased) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					if (control[i].knob.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
						control[i].ismove = false;
					}
				}
			}

			if (event.type == sf::Event::MouseMoved) {
				if (control[i].ismove) {
					float mouseX = event.mouseMove.x;
					float sliderStartX = control[i].sliderBar.getPosition().x;
					float sliderEndX = sliderStartX + control[i].sliderBar.getSize().x;
					float newKnobX = std::max(sliderStartX, std::min(mouseX, sliderEndX));

					control[i].volume = max(0.01, ((pos.x - sliderStartX) / control[i].sliderBar.getSize().x) * 100.0);
					control[i].knob.setPosition(newKnobX - control[i].knob.getRadius(), control[i].knob.getPosition().y);
				}
			}

			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					if (control[i].box.getGlobalBounds().contains(pos.x, pos.y)) {
						control[i].isChecked = !control[i].isChecked;

						if (control[i].isChecked)
							control[i].box.setFillColor(sf::Color::Green);
						else
							control[i].box.setFillColor(sf::Color::Transparent);

						return Soundeffect_main;
					}
				}
			}
		}
	}

	return Soundeffect_main; 

}


void SoundEffect::draw(RenderWindow& window) {
	window.draw(Board);
	Texture backLeftT;
	Sprite backLeft;

	backLeftT.loadFromFile("Assets/images/left.png");
	backLeft.setTexture(backLeftT);
	backLeft.setPosition(15, 10);

	Vector2i pos = Mouse::getPosition(window);
	Convert(backLeft, pos);

	window.draw(backLeft);



	for (int i = 0; i <= 4; i ++) {
		window.draw(control[i].label);
		window.draw(control[i].sliderBar);
		window.draw(control[i].knob);
		window.draw(control[i].testButton);
		window.draw(control[i].box); 
		window.draw(control[i].boxlabel); 
	}
}