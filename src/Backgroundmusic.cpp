#include "../include/Backgroundmusic.h"
#include "../include/Constant.h"

vector<std::string> musicFiles = { "Assets/Sound/chill.mp3", "Assets/Sound/edm.mp3", "Assets/Sound/mtp.mp3"};

vector<string> musicPng = {
	"Assets/images/MusicBackground/lofi.png", "Assets/images/MusicBackground/edm.png", "Assets/images/MusicBackground/mtp.jpg"
};

Backgroundmusic::Backgroundmusic() {
	BoardTexture.loadFromFile("Assets/images/music.png");
	Board.setTexture(BoardTexture);

	VolumeTexture.loadFromFile("Assets/images/volume.png");
	VolumeSprite.setTexture(VolumeTexture);

	VolumeSprite.setPosition(150, 75);

	MuteTexture.loadFromFile("Assets/images/volume-mute.png");
	MuteSprite.setTexture(MuteTexture);

	MuteSprite.setPosition(150, 75);

	if (!font.loadFromFile("Assets/Fonts/Arial.ttf")) {
		std::cerr << "Failed to load font\n";
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

	sliderBar.setSize(sf::Vector2f(400, 10)); 
	sliderBar.setPosition(230, 100);
	sliderBar.setFillColor(sf::Color(180, 180, 180)); 
	sliderBar.setOutlineThickness(2);
	sliderBar.setOutlineColor(sf::Color::Black);

	sliderKnob.setRadius(15); 
	sliderKnob.setFillColor(sf::Color(50, 150, 250)); 
	sliderKnob.setOutlineThickness(3);
	sliderKnob.setOutlineColor(sf::Color::White);
	sliderKnob.setPosition(230 + (volume * 4) - sliderKnob.getRadius(), 92); 

	musicIndex = 0; isMove = 0; 

	music.openFromFile(musicFiles[musicIndex]); 

	for (int i = 0; i < 3; i++) {
		if (musicTextures[i].loadFromFile(musicPng[i])) {
			musicOptions[i].setTexture(musicTextures[i]);
			ScalePic(musicOptions[i], 160);
			musicOptions[i].setPosition(150 + i * 200, 200);
		}
		else {
			std::cout << "Failed to load " << musicPng[i] << "\n";
		}
	}

	music.setVolume(volume);
	music.setLoop(true);
	music.play();
}

void Backgroundmusic::Play() {
	music.setVolume(volume);
	music.play(); 
}

void Backgroundmusic::SetMusic(short id) {
	music.stop();

	if (!music.openFromFile(musicFiles[id])) {
		cerr << "Music can not load \n"; exit(0);
	}

	music.setVolume(volume);
	music.setLoop(1);
	music.play();
}

int Backgroundmusic::handleEvent(RenderWindow& window) {
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

		if (event.type == sf::Event::MouseButtonPressed) {
			for (int i = 0; i < 3; i++) {
				if (musicOptions[i].getGlobalBounds().contains(pos.x, pos.y)) {
					musicIndex = i;
					SetMusic(i); 
				}
			}

			if (VolumeSprite.getGlobalBounds().contains(pos.x, pos.y)) {
				if (volume < 1) volume = 30;
				else volume = 0; 
				
				float sliderStartX = sliderBar.getPosition().x;
				float sliderEndX = sliderStartX + sliderBar.getSize().x;

				sliderKnob.setPosition(sliderStartX + (volume / 100) * sliderBar.getSize().x - sliderKnob.getRadius(), 92);
				music.setVolume(volume);
			}
		}	

		if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased) {
			if (event.mouseButton.button == sf::Mouse::Left) {
				float sliderStartX = sliderBar.getPosition().x;
				float sliderEndX = sliderStartX + sliderBar.getSize().x;

				if (sliderKnob.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) ||
					(pos.x >= sliderStartX && pos.x <= sliderEndX &&
						pos.y >= sliderBar.getPosition().y &&
						pos.y <= sliderBar.getPosition().y + sliderBar.getSize().y)) {
					
					volume = max(0.01, ((pos.x - sliderStartX) / sliderBar.getSize().x) * 100.0);
					sliderKnob.setPosition(sliderStartX + (volume / 100.0) * sliderBar.getSize().x - sliderKnob.getRadius(), 92);

					music.setVolume(volume);
				}
			}
		}

		if (event.type == sf::Event::MouseButtonPressed) {
			if (event.mouseButton.button == sf::Mouse::Left) {
				if (sliderKnob.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
					isMove = true;
				}
			}
		}

		if (event.type == sf::Event::MouseButtonReleased) {
			if (event.mouseButton.button == sf::Mouse::Left) {
				if (sliderKnob.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
					isMove = false;
				}
			}
		}

		if (event.type == sf::Event::MouseMoved) {
			if (isMove) {
				float mouseX = event.mouseMove.x;
				float sliderStartX = sliderBar.getPosition().x;
				float sliderEndX = sliderStartX + sliderBar.getSize().x;
				float newKnobX = std::max(sliderStartX, std::min(mouseX, sliderEndX));

				volume = max(0.01, ((pos.x - sliderStartX) / sliderBar.getSize().x) * 100.0);
				sliderKnob.setPosition(newKnobX - sliderKnob.getRadius(), sliderKnob.getPosition().y);
				music.setVolume(volume);
			}
		}
	}


	return Backgroundmusic_main;
}

void Backgroundmusic::draw(RenderWindow& window) {
	window.draw(Board);

	Texture backLeftT;
	Sprite backLeft;

	backLeftT.loadFromFile("Assets/images/left.png");
	backLeft.setTexture(backLeftT);
	backLeft.setPosition(15, 10);

	Vector2i pos = Mouse::getPosition(window);
	Convert(backLeft, pos);

	window.draw(backLeft); 
	window.draw(sliderBar);
	window.draw(sliderKnob);

	if (volume >= 1)
		window.draw(VolumeSprite);
	else
		window.draw(MuteSprite); 

	for (int i = 0; i < 3; i++) {
		if (i == musicIndex) {
			sf::RectangleShape highlight(sf::Vector2f(musicOptions[i].getGlobalBounds().width, musicOptions[i].getGlobalBounds().height));
			highlight.setPosition(musicOptions[i].getPosition());
			highlight.setFillColor(sf::Color::Transparent);
			highlight.setOutlineColor(sf::Color::Red);
			highlight.setOutlineThickness(3);
			window.draw(highlight);
		}

		window.draw(musicOptions[i]);
	}
}
