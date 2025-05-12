#include "Tetromino.h"

Tetromino::Tetromino() {}

const std::wstring& Tetromino::GetShape(int index) const {
    return pieces[index];
}

int Tetromino::Rotate(int x, int y, int r) {
    switch (r % 4) {
    case ZERO_DEG: return y * 4 + x;
    case NINETY_DEG: return 12 + y - (x * 4);
    case ONE_EIGHTY_DEG: return 15 - (y * 4) - x;
    case TWO_SEVENTY_DEG: return 3 - y + (x * 4);
    }
    return 0;
}
