#pragma once
#include "Tetromino.h"
#include "Field.h"
#include <Windows.h>
#include <vector>

class Game {
public:
    Game();
    void Run();

private:
    void ProcessInput();
    void Render();

    Tetromino tetromino;
    Field* field;
    wchar_t* screen;
    HANDLE hConsole;
    DWORD dwBytesWritten;

    bool gameOver;
    int currentPiece;
    int rotation;;
    int posX;
    int posY;
    bool keys[4];
    bool rotateHold;
    int speed;
    int speedCounter;
    int pieceCount;
    int score;
    std::vector<int> lines;
};
