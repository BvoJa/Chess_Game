#pragma once
#include "../include/Constant.h"

std::mt19937 rd(time(0));

int random(int l, int r) {
	return l + rd() % (r - l + 1);
}

pair<int, int> Square_to_cord(int square) {
	return make_pair(square % 8, square / 8);
}

void ScalePic(Sprite& shape, int sz) {
	float scaleX = sz / shape.getGlobalBounds().width;
	float scaleY = sz / shape.getLocalBounds().height;
	float scale = min(scaleX, scaleY);
	shape.setScale(scale, scale);
}

void Convert(Sprite& A, Vector2i pos, int adjust) {
	if (A.getGlobalBounds().contains(pos.x, pos.y))
		A.setColor(sf::Color(255, 255, 255, adjust));
	else
		A.setColor(sf::Color(255, 255, 255, 255));
}

bool in_Board(Vector2f pos) {
	int x = pos.x / SIZE; 
	int y = pos.y / SIZE; 

	return !(x < 0 || y < 0); 
}

Figure::Figure(int t, Texture& path) {
	shape.setTexture(path);
	float scaleX = SIZE / shape.getGlobalBounds().width;
	float scaleY = SIZE / shape.getLocalBounds().height;
	float scale = min(scaleX, scaleY);
	shape.setScale(scale, scale);
	type = t; played = ismove = 0; 
}

void Figure::reSize(Texture& path) {
	Vector2f pos = shape.getPosition();
	shape.setTexture(path);
	float scaleX = SIZE / shape.getGlobalBounds().width;
	float scaleY = SIZE / shape.getLocalBounds().height;
	float scale = min(scaleX, scaleY);
	shape.setPosition(pos);
	shape.setScale(scale, scale);
}

Figure::Figure() {
	ismove = played = 0; 
}

void Figure::SetPosition(int x, int y) {
	shape.setPosition(x, y);
}

void Figure::Setposition(Vector2i pos) {
	shape.setPosition(pos.x, pos.y);
}

void listSaveFiles(const std::string& saveDir, std::vector<std::string>& fileList) {
    for (const auto& entry : std::filesystem::directory_iterator(saveDir)) {
        if (entry.is_regular_file()) {
            fileList.push_back(entry.path().filename().string());
        }
    }
}