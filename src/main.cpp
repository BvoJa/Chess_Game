// main.cpp

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<iostream>
#include<fstream>
#include "../include/Menu.h"
#include "../include/Constant.h"
#include "../include/Mode.h"
#include "../include/Settings.h"
#include "../include/TwoPlayerMode.h"
#include "../include/OnePlayerMode.h"
#include "../include/Playaswhite.h"
#include "../include/Playasblack.h"
#include "../include/SoundEffect.h"
#include "../include/Backgroundmusic.h"
#include <memory>

using namespace sf;
int GameState = Initial_main;


int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Chess Game");

    sf::Image icon;
    icon.loadFromFile("Assets/images/thechess.png");

    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());


    auto menu = std::make_unique<Menu>();
    auto mode = std::make_unique<Mode>();
    auto modify = std::make_unique<Settings>();
    auto twoPlayers = std::make_unique<TwoPlayerMode>();
    auto onePlayer = std::make_unique<OnePlayerMode>();
    auto playaswhite = std::make_unique<Playaswhite>();
    auto playasblack = std::make_unique<Playasblack>();
    auto soundeffect = std::make_unique<SoundEffect>();
    auto backgroundmusic = std::make_unique<Backgroundmusic>();

    while (window.isOpen()) {

        if (GameState == -1) {
            GameState = menu->handleEvents(window);
        }

        if (GameState == GamePlay_main) {
            GameState = mode->handleEvents(window);
            if (GameState == TwoPlayers_main) {
                twoPlayers->BoardSetUp(modify->getPiecePath(), modify->getBoard());
                for (int i = 0; i <= 4; i++)
                    twoPlayers->sound[i].setVolume(soundeffect->control->volume);
            }
        }

        if (GameState == Settings_main) {
            GameState = modify->handleEvent(window);
            if (GameState == TwoPlayers_main) {
                twoPlayers->_settings = 0;
                modify->lst_access = Initial_main;
                for (int i = 0; i <= 4; i++)
                    twoPlayers->sound[i].setVolume(soundeffect->control->volume);
                twoPlayers->UpdChanges(modify->getPiecePath(), modify->getBoard());
            }
            
            if (GameState == Playasblack_main) {
                playasblack->_settings = 0;
                modify->lst_access = Initial_main;
                for (int i = 0; i <= 4; i++)
                    playasblack->sound[i].setVolume(soundeffect->control->volume);
                playasblack->UpdChanges(modify->getPiecePath(), modify->getBoard());
            }

            if (GameState == Playaswhite_main) {
                playaswhite->_settings = 0;
                modify->lst_access = Initial_main;
                for (int i = 0; i <= 4; i++)
                    playaswhite->sound[i].setVolume(soundeffect->control->volume);
                playaswhite->UpdChanges(modify->getPiecePath(), modify->getBoard());
            }
        }

        if (GameState == OnePlayer_main) {
            GameState = onePlayer->handleEvent(window);

            if (GameState == Playaswhite_main) {
                playaswhite->GetDiffculty(onePlayer->EASY(), onePlayer->MEDIUM(), onePlayer->HARD());
                playaswhite->BoardSetUp(modify->getPiecePath(), modify->getBoard());
                for (int i = 0; i <= 4; i++)
                    playaswhite->sound[i].setVolume(soundeffect->control->volume);

            }

            if (GameState == Playasblack_main) {
                playasblack->GetDiffculty(onePlayer->EASY(), onePlayer->MEDIUM(), onePlayer->HARD());
                playasblack->BoardSetUp(modify->getPiecePath(), modify->getBoard());

                for (int i = 0; i <= 4; i++)
                    playasblack->sound[i].setVolume(soundeffect->control->volume);
            }
        }

        if (GameState == TwoPlayers_main) {
            GameState = twoPlayers->handleEvents(window);
            if (twoPlayers->_settings) {
                GameState = Settings_main;
                modify->lst_access = TwoPlayers_main;
            }
        }

        if (GameState == Playasblack_main) {
            GameState = playasblack->handleEvents(window);
            if (playasblack->_settings) {
                GameState = Settings_main;
                modify->lst_access = Playasblack_main;
            }
        }

        if (GameState == Playaswhite_main) {
            GameState = playaswhite->handleEvents(window);
            if (playaswhite->_settings) {
                GameState = Settings_main;
                modify->lst_access = Playaswhite_main;
            }
        }

        if (GameState == Backgroundmusic_main) {
            GameState = backgroundmusic->handleEvent(window);
        }

        if (GameState == Soundeffect_main) {
            GameState = soundeffect->handleEvent(window);
        }

        window.clear(sf::Color(50, 50, 50));

        if (GameState == Initial_main) menu->draw(window);
        if (GameState == GamePlay_main) mode->draw(window);
        if (GameState == Settings_main) modify->draw(window);
        if (GameState == OnePlayer_main) onePlayer->draw(window);
        if (GameState == TwoPlayers_main) twoPlayers->draw(window);
        if (GameState == Playaswhite_main)  playaswhite->draw(window);
        if (GameState == Playasblack_main)  playasblack->draw(window);
        if (GameState == Soundeffect_main) soundeffect->draw(window);
        if (GameState == Backgroundmusic_main) backgroundmusic->draw(window);

        window.display();
    }

    return 0;
}

//#include <SFML/Graphics.hpp>
//#include <iostream>
//#include <vector>
//
//int main() {
//    // Tạo cửa sổ
//    sf::RenderWindow window(sf::VideoMode(800, 600), "Move Style Dropdown");
//
//    // Font chữ
//    sf::Font font;
//    if (!font.loadFromFile("Assets/Fonts/arial.ttf")) {
//        std::cout << "Error loading font!\n";
//        return -1;
//    }
//
//    // Trạng thái hiện tại
//    std::string currentStyle = "Drag pieces";
//
//    // Danh sách lựa chọn
//    std::vector<std::string> options = { "Drag pieces", "Click pieces" };
//    bool dropdownOpen = false; // Trạng thái menu (mở/đóng)
//
//    // Nút chính
//    sf::RectangleShape mainButton(sf::Vector2f(150, 40));
//    mainButton.setPosition(300, 250);
//    mainButton.setFillColor(sf::Color::White);
//    mainButton.setOutlineColor(sf::Color(139, 69, 19));
//    mainButton.setOutlineThickness(2);
//
//    sf::Text mainText(currentStyle, font, 20);
//    mainText.setFillColor(sf::Color::Black);
//    mainText.setPosition(310, 255);
//
//    // Menu thả xuống
//    std::vector<sf::RectangleShape> dropdownBoxes;
//    std::vector<sf::Text> dropdownTexts;
//
//    for (size_t i = 0; i < options.size(); ++i) {
//        sf::RectangleShape box(sf::Vector2f(150, 40));
//        box.setPosition(300, 250 + (i + 1) * 41); // Vị trí lệch xuống
//        box.setFillColor(sf::Color::White);
//        box.setOutlineColor(sf::Color(139, 69, 19));
//        box.setOutlineThickness(1);
//
//        sf::Text text(options[i], font, 20);
//        text.setFillColor(sf::Color::Black);
//        text.setPosition(310, 255 + (i + 1) * 41);
//
//        dropdownBoxes.push_back(box);
//        dropdownTexts.push_back(text);
//    }
//
//    // Vòng lặp chính
//    while (window.isOpen()) {
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed)
//                window.close();
//
//            // Xử lý click chuột
//            if (event.type == sf::Event::MouseButtonPressed) {
//                if (event.mouseButton.button == sf::Mouse::Left) {
//                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
//
//                    // Kiểm tra nút chính
//                    if (mainButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
//                        dropdownOpen = !dropdownOpen; // Mở hoặc đóng menu
//                    }
//
//                    // Kiểm tra các lựa chọn trong menu
//                    if (dropdownOpen) {
//                        for (size_t i = 0; i < dropdownBoxes.size(); ++i) {
//                            if (dropdownBoxes[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
//                                currentStyle = options[i]; // Cập nhật lựa chọn
//                                mainText.setString(currentStyle);
//                                dropdownOpen = false; // Đóng menu
//                            }
//                        }
//                    }
//                }
//            }
//        }
//
//        // Vẽ các thành phần
//        window.clear(sf::Color(50, 50, 50)); // Nền xám đậm
//        window.draw(mainButton);
//        window.draw(mainText);
//
//        if (dropdownOpen) {
//            for (size_t i = 0; i < dropdownBoxes.size(); ++i) {
//                window.draw(dropdownBoxes[i]);
//                window.draw(dropdownTexts[i]);
//            }
//        }
//
//        window.display();
//    }
//
//    return 0;
//}



//#include <SFML/Graphics.hpp>
//#include <fstream>
//#include <string>
//#include <iostream>
//#include <filesystem>
//
//using namespace std; 
//namespace fs = std::filesystem;
//
//void listSaveFiles(const std::string& saveDir, std::vector<std::string>& fileList) {
//    for (const auto& entry : std::filesystem::directory_iterator(saveDir)) {
//        if (entry.is_regular_file()) {
//            fileList.push_back(entry.path().filename().string());
//        }
//    }
//}
//
//int main() {
//    sf::RenderWindow window(sf::VideoMode(800, 600), "Save Game");
//
//    // Hộp thoại nhập tên file
//    sf::RectangleShape inputBox(sf::Vector2f(400, 50));
//    inputBox.setFillColor(sf::Color(200, 200, 200));
//    inputBox.setOutlineThickness(2);
//    inputBox.setOutlineColor(sf::Color(255, 150, 0));
//    inputBox.setPosition(200, 300);
//
//
//    // Text hướng dẫn
//    sf::Font font;
//    if (!font.loadFromFile("Assets/Fonts/Arial.ttf")) {
//        std::cerr << "Error loading font\n";
//        return -1;
//    }
//
//    sf::Text instruction("Enter filename and press Enter to save:", font, 24);
//    instruction.setFillColor(sf::Color::White);
//    instruction.setPosition(150, 250);
//
//    // Text hiển thị tên file đang nhập
//    sf::Text filenameText("", font, 24);
//    filenameText.setFillColor(sf::Color::Black);
//    filenameText.setPosition(210, 310);
//
//    std::string filename;
//    bool isFileSaved = false;
//
//    // Con trỏ nhấp nháy
//    bool showCursor = true; // Biến để bật/tắt con trỏ
//    sf::Clock cursorClock;  // Đếm thời gian để nhấp nháy
//    const float cursorBlinkTime = 0.5f; // Thời gian nhấp nháy (0.5 giây)
//
//    while (window.isOpen()) {
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed)
//                window.close();
//
//            // Xử lý nhập ký tự
//            if (event.type == sf::Event::TextEntered) {
//                if (event.text.unicode == '\b') { // Xóa ký tự
//                    if (!filename.empty())
//                        filename.pop_back();
//                }
//                else if (event.text.unicode == '\r') { // Nhấn Enter
//                    if (!filename.empty()) {
//                        std::ofstream outFile(filename + ".txt");
//                        if (outFile) {
//                            outFile << "Saved game data goes here\n";
//                            outFile.close();
//                            isFileSaved = true;
//                        }
//                    }
//                }
//                else if (event.text.unicode < 128) { // Nhập ký tự
//                    filename += static_cast<char>(event.text.unicode);
//                }
//            }
//        }
//
//        // Cập nhật trạng thái nhấp nháy
//        if (cursorClock.getElapsedTime().asSeconds() >= cursorBlinkTime) {
//            showCursor = !showCursor; // Đảo trạng thái
//            cursorClock.restart();   // Reset thời gian
//        }
//
//        // Cập nhật text hiển thị (thêm con trỏ nếu cần)
//        if (showCursor)
//            filenameText.setString(filename + "|");
//        else
//            filenameText.setString(filename);
//
//        // Vẽ màn hình
//        window.clear(sf::Color(50, 50, 50));
//        window.draw(inputBox);
//        window.draw(instruction);
//        window.draw(filenameText);
//
//        if (isFileSaved) {
//            cout << filename << '\n';
//            sf::Text success("File saved successfully!", font, 20);
//            success.setFillColor(sf::Color::Green);
//            success.setPosition(200, 400);
//            window.draw(success);
//        }
//
//        window.display();
//    }
//
//    return 0;
//}


//#include <SFML/Graphics.hpp>
//#include <vector>
//#include <string>
//#include <iostream>
//#include <filesystem>
//
//namespace fs = std::filesystem;
//
//struct FileItem {
//    sf::RectangleShape button;
//    sf::Text text;
//};
//
//int main() {
//    sf::RenderWindow window(sf::VideoMode(800, 600), "Load Game");
//
//    // Font
//    sf::Font font;
//    if (!font.loadFromFile("Assets/Fonts/Arial.ttf")) {
//        std::cerr << "Error loading font\n";
//        return -1;
//    }
//
//    // Lấy danh sách file
//    std::vector<std::string> files;
//    for (const auto& entry : fs::directory_iterator("Assets/GameHistory")) {
//        if (entry.is_regular_file())
//            files.push_back(entry.path().filename().string());
//    }
//
//    // Tạo danh sách các nút file
//    std::vector<FileItem> fileButtons;
//    const float buttonWidth = 400;
//    const float buttonHeight = 50;
//    const float buttonSpacing = 10;
//
//    for (size_t i = 0; i < files.size(); ++i) {
//        FileItem item;
//
//        item.button.setSize(sf::Vector2f(buttonWidth, buttonHeight));
//        item.button.setFillColor(sf::Color(200, 200, 200));
//        item.button.setOutlineThickness(2);
//        item.button.setOutlineColor(sf::Color::Black);
//        item.button.setPosition(200, i * (buttonHeight + buttonSpacing));
//
//        item.text.setFont(font);
//        item.text.setString(files[i]);
//        item.text.setCharacterSize(24);
//        item.text.setFillColor(sf::Color::Black);
//        item.text.setPosition(item.button.getPosition().x + 10, item.button.getPosition().y + 10);
//
//        fileButtons.push_back(item);
//    }
//
//    // Thanh trượt (scrollbar)
//    sf::RectangleShape scrollbar;
//    scrollbar.setSize(sf::Vector2f(20, 400));
//    scrollbar.setFillColor(sf::Color(150, 150, 150));
//    scrollbar.setPosition(650, 100);
//
//    sf::RectangleShape slider;
//    slider.setSize(sf::Vector2f(20, 50)); // Kích thước ban đầu
//    slider.setFillColor(sf::Color(100, 100, 100));
//    slider.setPosition(650, 100);
//
//    // Tính toán tỷ lệ thanh trượt
//    const float visibleHeight = 400; // Chiều cao vùng hiển thị
//    const float totalHeight = files.size() * (buttonHeight + buttonSpacing);
//    float sliderRatio = visibleHeight / totalHeight;
//    slider.setSize(sf::Vector2f(20, sliderRatio * scrollbar.getSize().y));
//
//    float scrollOffset = 0; // Độ lệch dọc để cuộn
//    float currentScroll = 0; // Vị trí cuộn hiện tại
//
//    // Vòng lặp chính
//    while (window.isOpen()) {
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed)
//                window.close();
//
//            // Xử lý con lăn chuột
//            if (event.type == sf::Event::MouseWheelScrolled) {
//                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
//                    float delta = event.mouseWheelScroll.delta * 20; // Lượng cuộn (20 pixel mỗi lần xoay)
//                    currentScroll -= delta;
//
//                    // Giới hạn cuộn trong vùng danh sách
//                    if (currentScroll < 0) currentScroll = 0;
//                    if (currentScroll > totalHeight - visibleHeight)
//                        currentScroll = totalHeight - visibleHeight;
//
//                    // Cập nhật vị trí slider
//                    float sliderY = scrollbar.getPosition().y +
//                        (currentScroll / (totalHeight - visibleHeight)) *
//                        (scrollbar.getSize().y - slider.getSize().y);
//                    slider.setPosition(slider.getPosition().x, sliderY);
//
//                    // Cập nhật vị trí các nút file
//                    for (size_t i = 0; i < fileButtons.size(); ++i) {
//                        fileButtons[i].button.setPosition(
//                            200, i * (buttonHeight + buttonSpacing) - currentScroll);
//                        fileButtons[i].text.setPosition(
//                            fileButtons[i].button.getPosition().x + 10,
//                            fileButtons[i].button.getPosition().y + 10);
//                    }
//                }
//            }
//
//            // Hiệu ứng hover
//            sf::Vector2f mousePos(sf::Mouse::getPosition(window));
//            for (auto& item : fileButtons) {
//                if (item.button.getGlobalBounds().contains(mousePos)) {
//                    item.button.setFillColor(sf::Color(180, 180, 180)); // Hover
//                }
//                else {
//                    item.button.setFillColor(sf::Color(200, 200, 200)); // Mặc định
//                }
//            }
//        }
//
//        // Vẽ giao diện
//        window.clear(sf::Color(50, 50, 50));
//
//        for (const auto& item : fileButtons) {
//            if (item.button.getPosition().y + buttonHeight > 100 && item.button.getPosition().y < 500) {
//                window.draw(item.button);
//                window.draw(item.text);
//            }
//        }
//
//        window.draw(scrollbar);
//        window.draw(slider);
//
//        window.display();
//    }
//
//    return 0;
//}
