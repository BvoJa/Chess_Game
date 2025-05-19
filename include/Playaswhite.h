#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include<cstring>
#include <array>
#include <vector>
#include "Constant.h"
#include <unordered_map>

using namespace sf;
using namespace std;


class Playaswhite {
public:
    Playaswhite();
    void BoardSetUp(string path, char numberBoard);
    void UpdChanges(string path, char numberBoard);
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
    int EndGame();
    array<int, 3> getAImove();
    void GetDiffculty(bool _easy, bool _medium, bool _hard); 
    bool check(); 
    void UseRedo(); 
    void UseUndo(); 
    bool MakeMove(int x, int y, int promoted); 
    long long getState();
    Sound sound[5];
    bool _settings = 0; 
    
    void SaveGame(const string& filename = "");
    void loadGame(const string& filename = "");
    void PrintSaveFile();
    void PrintLoadFile();
private:
    bool isWhiteTurn, pawnPromote, pieceSelected, Easy, Medium, Hard; 
    RenderWindow promotion, GameSave, GameLoad;
    int index;
    int isGameEnd; 
    int criticalMove; 
    vector <Vector2i> validMoves;
    Font font;
    RectangleShape Back, Load_Game;
    RectangleShape Save_Game;
    RectangleShape playAgain, Quit;
   
    Vector2i square_from;
    Vector2i square_to;
    
    bool isDragging = false;
    Vector2f originalPosition;

    Texture BoardTexture, Test, settingText;
    Sprite Board, t, settingbutton;
    Text cancelText;

    Texture black[6];
    Texture white[6];

    Figure Black[16];
    Figure White[16];

    Figure Blackdrag[16];
    Figure Whitedrag[16];

    Vector2i target;
    Sprite sample[5], Undo, Redo, Reset;

    array<int, 4> SaveBoard[551][33];
    array<int, 4> store[551][33];

    vector <array<int, 4>> Black_history, White_history;

    int BoardState[10][10];
    int Occur[10][10];
    int NumberOfMove;
    int NumberOfUndo;
    unordered_map<int, long long, custom_hash> countState;

    SoundBuffer sBuffer[5];
};