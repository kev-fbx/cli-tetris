#include "Field.h"

Field::Field(int w, int h) : width(w), height(h) {
    field = new unsigned char[width * height];
    for (int x = 0; x < width; ++x)
        for (int y = 0; y < height; ++y)
            field[y * width + x] = (x == 0 || x == width - 1 || y == height - 1) ? FIELD_BORDER : EMPTY_SPACE;
}

Field::~Field() {
    delete[] field;
}

bool Field::PieceFits(const Tetromino& piece, int index, int r, int posX, int posY) const {
    for (int x = 0; x < TETROMINO_MAX_DIM; x++)
        for (int y = 0; y < TETROMINO_MAX_DIM; y++) {
            int pi = Tetromino::Rotate(x, y, r);
            if (piece.GetShape(index)[pi] != TETROMINO_FILLER) {
                int delta_x = posX + x;
                int delta_y = posY + y;
                if (delta_x < 0 || delta_x >= width || delta_y < 0 || delta_y >= height) {
                    return false;
                }
                if (field[delta_y * width + delta_x] != 0) {
                    return false;
                }
            }
        }
    return true;
}

void Field::LockPiece(const Tetromino& piece, int index, int r, int posX, int posY) {
    for (int x = 0; x < TETROMINO_MAX_DIM; x++)
        for (int y = 0; y < TETROMINO_MAX_DIM; y++) {
            if (piece.GetShape(index)[Tetromino::Rotate(x, y, r)] != TETROMINO_FILLER) {
                field[(posY + y) * width + (posX + x)] = index + 1;
            }
        }
}

unsigned char* Field::GetField() {
    return field;
}

void Field::Draw(wchar_t* screen, int screenWidth, int screenHeight) const {
    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++) {
            int screenIndex = (y + 2) * screenWidth + (x + 2);
            screen[screenIndex] = SYMBOLS[field[y * width + x]];
        }
}

bool Field::CheckLine(int y) {
    for (int x = 1; x < width - 1; ++x)
        if (field[y * width + x] == 0)
            return false;
    return true;
}

void Field::RemoveLine(int y) {
    for (int x = 1; x < width - 1; x++)
        for (int i = y; i > 0; i--)
            field[i * width + x] = field[(i - 1) * width + x];
}
