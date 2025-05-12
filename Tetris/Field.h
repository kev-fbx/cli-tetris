#pragma once

#ifndef FIELD_H
#define FIELD_H

#include "Tetromino.h"

class Field {
public:
    Field(int width, int height);
    ~Field();

    bool PieceFits(const Tetromino& piece, int index, int rotation, int posX, int posY) const;
    void LockPiece(const Tetromino& piece, int index, int rotation, int posX, int posY);
    void Draw(wchar_t* screen, int screenWidth, int screenHeight) const;
    bool CheckLine(int y);
    void RemoveLine(int y);
    unsigned char* GetField();

private:
    int width, height;
    unsigned char* field;
};


#endif