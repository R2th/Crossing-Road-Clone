#include "GraphicsController.h"

GraphicsController::GraphicsController()
{
	buffer = new wchar_t[screenHeight * screenWidth]{ L" " };
	starBuffer = new wchar_t[screenHeight * screenWidth]{ L' ' };
	color = new WORD[screenHeight * screenWidth];
	for (int i = 0; i < screenHeight * screenWidth; ++i) {
		color[i] = black * 16 + whiteDark; //will add crescendo effect later
	}
	HANDLE hConsole1 = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole1);
	hConsole = hConsole1;
	dwBytesWritten = 0;
	bufferStorage = BUFFER_MAP;
	initCharToBlock();
	initClearBuffer();
}

GraphicsController::~GraphicsController()
{
	delete[] buffer;
	delete[] starBuffer;
	delete[] color;
}

void GraphicsController::initClearBuffer() {
	for (string c : CLEAR_BUFFER) {
		wstring tmp = L"";
		for (int i = 0; i < bufferStorage[c][0].size() /* width */; i++)
			tmp += L" ";
		bufferStorage[c + "_clear"] = vector <wstring>{};
		for (int i = 0; i < bufferStorage[c].size() /* height */; i++)
			bufferStorage[c + "_clear"].push_back(tmp);
	}
}
void GraphicsController::initCharToBlock() {
	for (string c : CHAR_TO_BLOCK)
		charToBlock(bufferStorage[c]);
}

void GraphicsController::charToBlock(vector<wstring>& graphics) {
	for (int i = 0; i < graphics.size(); ++i) {
		for (int j = 0; j < graphics[i].length(); ++j) {
			if (graphics[i][j] == L' ')
				continue;
			else if (graphics[i][j] == L'.')
				graphics[i][j] = L'▄';
			else if (graphics[i][j] == L'\'')
				graphics[i][j] = L'▀';
			else if (graphics[i][j] == L'l')
				graphics[i][j] = L'█';
		}
	}
}