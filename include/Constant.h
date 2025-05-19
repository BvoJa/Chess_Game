/// Constant.h
#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include<string>
#include<iostream>
#include<vector>
#include<cmath>
#include<array>
#include<stack>
#include<queue>
#include <random>
#include <chrono>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;
using namespace sf;
using namespace std; 

const int Initial_main = -1; 
const int GamePlay_main = 1;
const int Settings_main = 2;
const int TwoPlayers_main = 3;
const int OnePlayer_main = 4;
const int SaveGame_main = 5; 
const int Playasblack_main = 6; 
const int Playaswhite_main = 7; 

const int Backgroundmusic_main = 8; 
const int Soundeffect_main = 9; 
const int repetition_main = 10; 
const int stalemate_main = 11; 
const int fifty_main = 12; 
const int insufficient_main = 13; 
const int checkmate_main = 1; 

const int king_main = 0;
const int queen_main = 1;
const int bishop_main = 2;
const int knight_main = 3;
const int rook_main = 4;
const int pawn_main = 5;

const int SIZE = 75; 

const int Kx[] = { -1, 1, -1, 1, 0, 0, -1, 1 };
const int Ky[] = { -1, 1, 1, -1, -1, 1, 0, 0 };

const int Knx[] = { -1, -2, 1, 2, 2, 1, -1, -2 };
const int Kny[] = { -2, -1, -2, -1, 1, 2, 2, 1 };

extern std::mt19937 rd;
int random(int l, int r);   

pair<int, int> Square_to_cord(int square);

void ScalePic(Sprite &shape, int sz);

bool in_Board(Vector2f pos); 

void Convert(Sprite& A, Vector2i pos, const int adjust = 215); 

class Figure {
public:
    Figure();
    Figure(int t, Texture& path);
    void SetPosition(int x, int y);
    void Setposition(Vector2i pos);
    void reSize(Texture& path);
    Sprite shape;
    int type;
    int played;
    bool ismove; 
};

struct custom_hash {
    static uint64_t splitmix64(uint64_t x) {
        // http://xorshift.di.unimi.it/splitmix64.c
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }
};

struct FileItem {
    sf::RectangleShape button;
    sf::Text text;
};

void listSaveFiles(const std::string& saveDir, std::vector<std::string>& fileList);