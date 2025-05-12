#include <iostream>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <vector>

using namespace std;

wstring tetromino[7];
int nFieldWidth = 12;
int nFieldHeight = 18;
unsigned char* pField = nullptr;

int nScreenWidth = 120;
int nScreenHeight = 43;

int RotatePiece(int x, int y, int r) {
	switch (r % 4) {
	case 0: return y * 4 + x;        //   0 degress
	case 1: return 12 + y - (x * 4); //  90 degrees
	case 2: return 15 - (y * 4) - x; // 180 degrees
	case 3: return 3 - y + (x * 4);  // 270 degrees
	}
	return 0;
}

static bool PieceFits(int nTetromino, int nRotation, int nPosX, int nPosY) {
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			// Get index of piece
			int pieceIndex = RotatePiece(x, y, nRotation);
			// Get index of field
			int fieldIndex = (nPosY + y) * nFieldWidth + (nPosX + x); 
			// Collision check
			if (nPosX+ x >= 0 && nPosX+ x < nFieldWidth) {
				if (nPosY + y >= 0 && nPosY + y < nFieldHeight) {
					if (tetromino[nTetromino][pieceIndex] != L'.' && pField[fieldIndex] != 0) {
						return false;
					}
				}
			}
		}
	}
	return true;
}

int main() {

	// Initialise shapes
	tetromino[0].append(L"..X...X...X...X.");
	tetromino[1].append(L"..X..XX...X.....");
	tetromino[2].append(L".....XX..XX.....");
	tetromino[3].append(L"..X..XX..X......");
	tetromino[4].append(L".X...XX...X.....");
	tetromino[5].append(L".X...X...XX.....");
	tetromino[6].append(L"..X...X..XX.....");

	// Create play field buffer
	pField = new unsigned char[nFieldWidth * nFieldHeight];

	// Define board boundary: 9 is border, 0 is empty space
	for (int x = 0; x < nFieldWidth; x++) {
		for (int y = 0; y < nFieldHeight; y++) {
			pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;
		}
	}

	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	// Primary game loop and logic
	bool gameOver = false;

	// Current piece state
	int nCurrentPiece = rand() % 7;
	int nCurrentRotation = 0;

	// Initial position of a piece
	int nCurrentX = nFieldWidth / 2;
	int nCurrentY = 0;

	bool bKey[4] = { false, false, false, false };
	bool bRotateHold = false;

	int nSpeed = 20;
	int nSpeedCounter = 0;
	bool bForceDown = false;
	int nPieceCount = 0;
	int score = 0;

	vector<int> vLines;

	while (!gameOver) {

		// Game Timing
		this_thread::sleep_for(50ms);
		nSpeedCounter++;
		bForceDown = (nSpeedCounter == nSpeed);

		// Input Handling
		for (int k = 0; k < 4; k++) {
			bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;
		}

		// Game Logic
		nCurrentX += (bKey[0] && PieceFits(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) ? 1 : 0;
		nCurrentX -= (bKey[1] && PieceFits(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) ? 1 : 0;
		nCurrentY += (bKey[2] && PieceFits(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) ? 1 : 0;

		if (bKey[3]) {
			nCurrentRotation += (!bRotateHold && PieceFits(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) ? 1 : 0;
			bRotateHold = true;
		}
		else {
			bRotateHold = false;
		}

		if (bForceDown) {
			// First case: piece can still move down
			if (PieceFits(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) {
				nCurrentY++;
			// Second case: piece can no longer move down
			} else {
				// Lock current piece
				for (int x = 0; x < 4; x++) {
					for (int y = 0; y < 4; y++) {
						if (tetromino[nCurrentPiece][RotatePiece(x, y, nCurrentRotation)] != L'.') {
							pField[(nCurrentY + y) * nFieldWidth + (nCurrentX + x)] = nCurrentPiece + 1;
						}
					}
				}
				nPieceCount++;
				if (nPieceCount % 10 == 0) {
					if (nSpeed >= 10) nSpeed--;
					nSpeed++;
				}
				// Check for complete lines
				for (int y = 0; y < 4; y++) {
					if (nCurrentY + y < nFieldHeight - 1) {
						bool bLine = true;
						for (int x = 1; x < nFieldWidth - 1; x++) {
							bLine &= (pField[(nCurrentY + y) * nFieldWidth + x]) != 0;
						}
						if (bLine) {
							for (int x = 1; x < nFieldWidth - 1; x++) {
								pField[(nCurrentY + y) * nFieldWidth + x] = 8;
							}
							vLines.push_back(nCurrentY + y);
						}
					}
				}
				score += 25;
				if (!vLines.empty()) {
					score += (1 << vLines.size()) * 100;
				}
				// Get next piece
				nCurrentX = nFieldWidth / 2;
				nCurrentY = 0;
				nCurrentRotation = 0;
				nCurrentPiece = rand() % 7;
				// Losing state
				gameOver = !PieceFits(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
			}

			nSpeedCounter = 0;
		}

		// Render Output

		// Draw field
		for (int x = 0; x < nFieldWidth; x++) {  
			for (int y = 0; y < nFieldHeight; y++) {  
				int screenIndex = (y + 2) * nScreenWidth + (x + 2);  
				if (screenIndex >= 0 && screenIndex < nScreenWidth * nScreenHeight) {  
					screen[screenIndex] = L" ABCDEFG=#"[pField[y * nFieldWidth + x]];  
				}  
			}  
		}

		// Draw current piece
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                if (tetromino[nCurrentPiece][RotatePiece(x, y, nCurrentRotation)] != L'.') {
                    int screenIndex = (nCurrentY + y + 2) * nScreenWidth + (nCurrentX + x + 2);
                    if (screenIndex >= 0 && screenIndex < nScreenWidth * nScreenHeight) {
                        screen[screenIndex] = nCurrentPiece + 65;
                    }
                }
            }
        }

		// Draw title and score
		swprintf_s(&screen[2 * nScreenWidth + nFieldWidth + 6], 11, L"CLI TETRIS");
		swprintf_s(&screen[3 * nScreenWidth + nFieldWidth + 6], 16, L"SCORE: %8d", score);

		if (!vLines.empty()) {
			WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);
			this_thread::sleep_for(400ms);

			for (auto& v : vLines) {
				for (int x = 1; x < nFieldWidth - 1; x++) {
					for (int y = v; y > 0; y--) {
						pField[y * nFieldWidth + x] = pField[(y - 1) * nFieldWidth + x];
					}
					pField[x] = 0;
				}
			}
			vLines.clear();
		}

		// Display frame
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);
	}

	CloseHandle(hConsole);
	cout << "Game Over!\nScore:" << score << endl;
	system("pause");

	return 0;
}