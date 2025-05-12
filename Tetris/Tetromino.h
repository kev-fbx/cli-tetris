#pragma once
#ifndef TETROMINO_H
#define TETROMINO_H

#include <iostream>
#include <string>
#include <vector>

#include "Constants.h"



class Tetromino {
public:
	Tetromino();
	const std::wstring& GetShape(int index) const;
	static int Rotate(int x, int y, int rotation);

private:
	std::wstring pieces[TETROMINO_VARIATIONS]= {
		I_TETROMINO_SHAPE,
		O_TETROMINO_SHAPE,
		T_TETROMINO_SHAPE,
		J_TETROMINO_SHAPE,
		L_TETROMINO_SHAPE,
		S_TETROMINO_SHAPE,
		Z_TETROMINO_SHAPE
	};
};

#endif // TETROMINO_H