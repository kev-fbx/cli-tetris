#pragma once
#ifndef CONSTANTS_H
#define CONSTANTS_H

// Field dimensions
#define FIELD_WIDTH                  12
#define FIELD_HEIGHT                 18

// Screen dimensions
#define SCREEN_WIDTH                 120
#define SCREEN_HEIGHT                43

// Tetromino info
#define TETROMINO_VARIATIONS         7
#define TETROMINO_MAX_DIM            4

// Tetromino rotation states
#define ZERO_DEG                     0
#define NINETY_DEG                   1
#define ONE_EIGHTY_DEG               2
#define TWO_SEVENTY_DEG              3

// Tetromino shape definitions
#define I_TETROMINO_SHAPE            L"..X...X...X...X."
#define O_TETROMINO_SHAPE            L".....XX..XX....."
#define T_TETROMINO_SHAPE            L"..X..XX...X....."
#define J_TETROMINO_SHAPE            L"..X...X..XX....."
#define L_TETROMINO_SHAPE            L".X...X...XX....."
#define S_TETROMINO_SHAPE            L".X...XX...X....."
#define Z_TETROMINO_SHAPE            L"..X..XX..X......"
#define TETROMINO_FILLER             L'.'

// Tetromino indexing
#define I_TETROMINO                  0
#define O_TETROMINO                  1
#define T_TETROMINO                  2
#define J_TETROMINO                  3
#define L_TETROMINO                  4
#define S_TETROMINO                  5
#define Z_TETROMINO                  6

// Game speed
#define INITIAL_SPEED                20
#define SPEED_MIN                    10

// Scoring
#define BASE_SCORE                   25
#define LINE_CLEAR_MULTIPLIER        100

// Misc rendering constants
#define SYMBOLS                      L" ABCDEFG=#"
#define EMPTY_SQUARE                 L' '
#define EMPTY_SPACE                  0
#define FIELD_BORDER                 9
#define LINE_CLEAR                   8

#endif // CONSTANTS_H