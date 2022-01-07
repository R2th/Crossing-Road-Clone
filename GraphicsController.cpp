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