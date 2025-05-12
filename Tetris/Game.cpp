#include "Game.h"
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

Game::Game()
    : field(new Field(FIELD_WIDTH, FIELD_HEIGHT)), screen(new wchar_t[SCREEN_WIDTH * SCREEN_HEIGHT]), gameOver(false),
    speed(INITIAL_SPEED), speedCounter(0), pieceCount(0), score(0), rotateHold(false), dwBytesWritten(0) {

    currentPiece = rand() % 7;
    rotation = 0;
    posX = FIELD_WIDTH / 2;
    posY = 0;
    dwBytesWritten = 0;

    for (int i = 0; i < TETROMINO_MAX_DIM; i++) keys[i] = false;
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) screen[i] = EMPTY_SQUARE;
    hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
}

void Game::Run() {
    while (!gameOver) {
        this_thread::sleep_for(50ms);
        speedCounter++;
        bool forceDown = (speedCounter == speed);

        ProcessInput();

        // Input Handler
        if (keys[0] && field->PieceFits(tetromino, currentPiece, rotation, posX + 1, posY)) posX++;
        if (keys[1] && field->PieceFits(tetromino, currentPiece, rotation, posX - 1, posY)) posX--;
        if (keys[2] && field->PieceFits(tetromino, currentPiece, rotation, posX, posY + 1)) posY++;
        if (keys[3]) {
            if (!rotateHold && field->PieceFits(tetromino, currentPiece, rotation + 1, posX, posY))
                rotation++;
            rotateHold = true;
        }
        else rotateHold = false;

        if (forceDown) {
            if (field->PieceFits(tetromino, currentPiece, rotation, posX, posY + 1)) {
                posY++;
            }
            else {
                field->LockPiece(tetromino, currentPiece, rotation, posX, posY);

                pieceCount++;
                if (pieceCount % 10 == 0 && speed >= SPEED_MIN) speed--;
                speed++;

                for (int y = 0; y < TETROMINO_MAX_DIM; y++) {
                    if (posY + y < FIELD_HEIGHT - 1) {
                        if (field->CheckLine(posY + y)) {
                            for (int x = 1; x < 11; x++)
                                field->GetField()[(posY + y) * FIELD_WIDTH + x] = LINE_CLEAR;
                            lines.push_back(posY + y);
                        }
                    }
                }

                // Increment score
                score += BASE_SCORE;
                if (!lines.empty()) {
                    score += (1 << lines.size()) * LINE_CLEAR_MULTIPLIER;
                }

				// New piece
                posX = FIELD_WIDTH / 2;
                posY = 0;
                rotation = 0;
                currentPiece = rand() % 7;
                gameOver = !field->PieceFits(tetromino, currentPiece, rotation, posX, posY);
            }
            speedCounter = 0;
        }

        Render();
    }

    CloseHandle(hConsole);
    wcout << L"Game Over!\nScore: " << score << endl;
    system("pause");
}

void Game::ProcessInput() {
    for (int k = 0; k < 4; k++)
        keys[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;
}

void Game::Render() {
    field->Draw(screen, SCREEN_WIDTH, SCREEN_HEIGHT);

    for (int x = 0; x < TETROMINO_MAX_DIM; x++)
        for (int y = 0; y < TETROMINO_MAX_DIM; y++)
            if (tetromino.GetShape(currentPiece)[Tetromino::Rotate(x, y, rotation)] != TETROMINO_FILLER)
                screen[(posY + y + 2) * SCREEN_WIDTH + (posX + x + 2)] = currentPiece + 65;

    swprintf_s(&screen[2 * SCREEN_WIDTH + 18], 11, L"CLI TETRIS");
    swprintf_s(&screen[3 * SCREEN_WIDTH + 18], 16, L"SCORE: %8d", score);

    if (!lines.empty()) {
        WriteConsoleOutputCharacter(hConsole, screen, SCREEN_WIDTH * SCREEN_HEIGHT, { 0, 0 }, &dwBytesWritten);
        this_thread::sleep_for(400ms);
        for (auto& v : lines)
            field->RemoveLine(v);
        lines.clear();
    }

    WriteConsoleOutputCharacter(hConsole, screen, SCREEN_WIDTH * SCREEN_HEIGHT, { 0, 0 }, &dwBytesWritten);
}
