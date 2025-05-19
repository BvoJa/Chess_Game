/// TwoPlayerMode.h
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include<cstring>
#include "Constant.h"
#include <array>
#include <vector>
#include <unordered_map>

using namespace sf;
using namespace std;


class TwoPlayerMode {
public: 
    TwoPlayerMode();
    void BoardSetUp(string path, char numberBoard); 
    void draw(sf::RenderWindow& window);
    Vector2i getCord(Figure A); 
    int handleEvents(sf::RenderWindow& window);

    void drawValidMoves(RenderWindow& window, const std::vector<Vector2i>& validmoves);
    bool KingSafe(Vector2i temp); 
    void getValidMove(Figure A, vector<Vector2i>& move); 
    void UpdBoardState(); 
    void PawnLogic(Figure A, vector<Vector2i>& move); 
    bool isValidMove(Figure A, Vector2i pos);
    void UpdHistoryWhite(Figure A, Vector2i pos);
    void UpdHistoryBlack(Figure A, Vector2i pos);
    void KingLogic(Figure A, vector <Vector2i>& move);
    void KnightLogic(Figure A, vector<Vector2i>& move);
    void Horizontal(Figure A, vector<Vector2i>& move);
    void Vertical(Figure A, vector<Vector2i>& move);
    void Diagonal(Figure A, vector<Vector2i>& move);
    bool TakePiece(Vector2i pos);
    void Castling(vector <Vector2i>& move);

    void drawPawnPromote();
    int PawnPromotion(Event event);
    void drawUndoRedo(RenderWindow& window);
    void UndoRedo(Event event, RenderWindow& window); 
    void ResetRedo(); 
    void ResetAll();
    bool check(); 
    int EndGame(); 
    void UseRedo(); 
    void UseUndo(); 
    bool makemove(int x, int y);
    void SaveGame(const string& filename = "");
    void loadGame(const string& filename = "");
    long long getState(); 
    Sound sound[5];
    bool _settings = 0; 
    void UpdChanges(string path, char numberBoard);
    void PrintSaveFile();
    void PrintLoadFile();
private:
    RenderWindow promotion, GameSave, GameLoad; 
    bool isWhiteTurn, pawnPromote, pieceSelected; 
    int isGameEnd; 
    int index;
    bool isAMoveDone; 
    vector <Vector2i> validMoves; 
    Font font;
    RectangleShape Back;
    RectangleShape Save_Game, Load_Game;
    RectangleShape playAgain, Quit;
    
    Vector2i square_from;
    Vector2i square_to;

    Texture BoardTexture, Test, settingText;
    Sprite Board, t, settingbutton;
    Text cancelText; 
    Text Indicator;

    Texture black[6]; 
    Texture white[6]; 
    
    bool isDragging = false;
    Vector2f originalPosition;

    Figure Black[16];
    Figure White[16];

    Figure Blackdrag[16]; 
    Figure Whitedrag[16];

    Vector2i target; 
    Sprite sample[5], Undo, Redo, Reset;

    array<int, 4> SaveBoard[601][33];
    array<int, 4> store[601][33];

    vector <array<int, 4>> Black_history, White_history; 
    int criticalMove; 

    int BoardState[10][10]; 
    int Occur[10][10]; 
    int NumberOfMove;
    int NumberOfUndo;

    unordered_map<int, long long, custom_hash> countState;
    SoundBuffer sBuffer[5];

};