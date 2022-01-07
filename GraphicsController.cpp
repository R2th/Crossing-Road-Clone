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

void GraphicsController::openFrame(int x, int y, int w, int h) {
	buffer[y * screenWidth + x] = bufferStorage["frame"][0][0];
	buffer[y * screenWidth + x + w - 1] = bufferStorage["frame"][0][2];
	color[y * screenWidth + x] = 7;
	color[y * screenWidth + x + w - 1] = 7;
	for (int i = 1; i < w - 1; ++i) {
		buffer[y * screenWidth + x + i] = bufferStorage["frame"][0][1];
		color[y * screenWidth + x + i] = 7;
	}
	int temp = 2;
	while (1) {
		buffer[(y + temp - 1) * screenWidth + x] = bufferStorage["frame"][2][0];
		buffer[(y + temp - 1) * screenWidth + x + w - 1] = bufferStorage["frame"][2][2];
		color[(y + temp - 1) * screenWidth + x] = 7;
		color[(y + temp - 1) * screenWidth + x + w - 1] = 7;
		for (int i = 1; i < w - 1; ++i) {
			buffer[(y + temp - 1) * screenWidth + x + i] = bufferStorage["frame"][2][1];
			color[(y + temp - 1) * screenWidth + x + i] = 7;
		}
		if (temp > 2) {
			buffer[(y + temp - 2) * screenWidth + x] = bufferStorage["frame"][1][0];
			buffer[(y + temp - 2) * screenWidth + x + w - 1] = bufferStorage["frame"][1][2];
			color[(y + temp - 2) * screenWidth + x] = 7;
			color[(y + temp - 2) * screenWidth + x + w - 1] = 7;
			for (int j = 1; j < w - 1; ++j) {
				buffer[(y + temp - 2) * screenWidth + x + j] = L' ';
				color[(y + temp - 2) * screenWidth + x + j] = 7;
			}
		}
		temp++;
		if (temp == h + 1) {
			render();
			break;
		}
		render();
		delay(1000 / (FRAMERATE));
	}
}

void GraphicsController::createFrame(int x, int y, int w, int h, bool transparentBG = true, bool isBlackBG = true) {
	int Bg = isBlackBG ? black : blueDark;
	buffer[y * screenWidth + x] = bufferStorage["frame"][0][0];
	buffer[y * screenWidth + x + w - 1] = bufferStorage["frame"][0][2];
	buffer[(y + h - 1) * screenWidth + x] = bufferStorage["frame"][2][0];
	buffer[(y + h - 1) * screenWidth + x + w - 1] = bufferStorage["frame"][2][2];
	color[y * screenWidth + x] = Bg * 16 + white;
	color[y * screenWidth + x + w - 1] = Bg * 16 + white;
	color[(y + h - 1) * screenWidth + x] = Bg * 16 + white;
	color[(y + h - 1) * screenWidth + x + w - 1] = Bg * 16 + white;
	for (int i = 1; i < w - 1; ++i) {
		buffer[y * screenWidth + x + i] = bufferStorage["frame"][0][1];
		buffer[(y + h - 1) * screenWidth + x + i] = bufferStorage["frame"][2][1];
		color[y * screenWidth + x + i] = Bg * 16 + white;
		color[(y + h - 1) * screenWidth + x + i] = Bg * 16 + white;
	}
	for (int i = 1; i < h - 1; ++i) {
		buffer[(y + i) * screenWidth + x] = bufferStorage["frame"][1][0];
		buffer[(y + i) * screenWidth + x + w - 1] = bufferStorage["frame"][1][2];
		color[(y + i) * screenWidth + x] = Bg * 16 + white;
		color[(y + i) * screenWidth + x + w - 1] = Bg * 16 + white;
	}
	if (!transparentBG) {
		for (int i = 1; i < h - 1; ++i) {
			for (int j = 1; j < w - 1; ++j) {
				buffer[(y + i) * screenWidth + x + j] = L' ';
				color[(y + i) * screenWidth + x + j] = Bg * 16 + white;
			}
		}
	}
}

vector<wstring>& GraphicsController::getBuffer(string key)
{
	return bufferStorage[key];
}

bool GraphicsController::isInScreen(int y, int x) {
	return 0 <= y && y < screenHeight - 1 && 0 <= x && x <= screenWidth - 1;
}

void GraphicsController::setBuffer(vector<wstring>& content, int x, int y, int bgColor, int fgColor) {
	for (int i = 0; i < content.size(); ++i) {
		for (int j = 0; j < content[i].length(); ++j) {
			if (!isInScreen(y + i, x + j))
				continue;
			buffer[(y + i) * screenWidth + x + j] = content[i].at(j);
			color[(y + i) * screenWidth + x + j] = bgColor * 16 + fgColor;
		}
		if (isInScreen(y + i, x - 2))
			color[(y + i) * screenWidth + x - 2] = bgColor * 16 + fgColor;
		if (isInScreen(y + i, x - 1))
			color[(y + i) * screenWidth + x - 1] = bgColor * 16 + fgColor;
		if (isInScreen(y + i, x + content[i].length()))
			color[(y + i) * screenWidth + x + content[i].length()] = bgColor * 16 + fgColor;
		if (isInScreen(y + i, x + content[i].length() + 1))
			color[(y + i) * screenWidth + x + content[i].length() + 1] = bgColor * 16 + fgColor;
	}
}
void GraphicsController::setBufferObject(vector<wstring>& content, int x, int y, int bgColor, int fgColor) {
	for (int i = 0; i < content.size(); ++i) {
		for (int j = 0; j < content[i].length(); ++j) {
			if (!isInScreen(y + i, x + j))
				continue;
			buffer[(y + i) * screenWidth + x + j] = content[i].at(j);
			color[(y + i) * screenWidth + x + j] = bgColor * 16 + fgColor;
		}
		if (isInScreen(y + i, x - 2))
			color[(y + i) * screenWidth + x - 2] = bgColor * 16 + fgColor;
		if (isInScreen(y + i, x - 1))
			color[(y + i) * screenWidth + x - 1] = bgColor * 16 + fgColor;
		if (isInScreen(y + i, x + content[i].length()))
			color[(y + i) * screenWidth + x + content[i].length()] = bgColor * 16 + fgColor;
		if (isInScreen(y + i, x + content[i].length() + 1))
			color[(y + i) * screenWidth + x + content[i].length() + 1] = bgColor * 16 + fgColor;
	}
	for (int j = 0; j < content[0].length(); ++j) {
		if (isInScreen(y - 1, x + j))
			color[(y - 1) * screenWidth + x + j] = bgColor * 16 + fgColor;
		if (isInScreen(y + content.size(), x + j))
			color[(y + content.size()) * screenWidth + x + j] = bgColor * 16 + fgColor;
	}
	if (isInScreen(y - 1, x - 1))
		color[(y - 1) * screenWidth + x - 1] = bgColor * 16 + fgColor;
	if (isInScreen(y - 1, x + content[0].length()))
		color[(y - 1) * screenWidth + x + content[0].length()] = bgColor * 16 + fgColor;
	if (isInScreen(y + content.size(), x - 1))
		color[(y + content.size()) * screenWidth + x - 1] = bgColor * 16 + fgColor;
	if (isInScreen(y + content.size(), x + content[0].length()))
		color[(y + content.size()) * screenWidth + x + content[0].length()] = bgColor * 16 + fgColor;
}
void GraphicsController::setBuffer(wstring content, int x, int y, int bgColor, int fgColor) {
	for (int i = 0; i < content.length(); ++i) {
		buffer[y * screenWidth + x + i] = content[i];
		color[y * screenWidth + x + i] = bgColor * 16 + fgColor;
	}
}

void GraphicsController::setBufferWhite(vector<wstring>& content, int x, int y, int bgColor, int fgColor) {
	for (int i = 0; i < content.size(); ++i) {
		for (int j = 0; j < content[i].length(); ++j) {
			buffer[(y + i) * screenWidth + x + j] = content[i].at(j);
			color[(y + i) * screenWidth + x + j] = bgColor * 16 + fgColor;
		}
	}
}

void GraphicsController::clearBuffer() {
	for (int i = 0; i < screenWidth * screenHeight; ++i) {
		color[i] = black * 16 + whiteDark;
		buffer[i] = L' ';
	}
}
void GraphicsController::clearBuffer(int bg, int ch) {
	for (int i = 0; i < screenWidth * screenHeight; ++i) {
		color[i] = bg * 16 + ch;
		buffer[i] = L' ';
	}
}

void GraphicsController::render() {
	WriteConsoleOutputAttribute(hConsole, color, screenWidth * screenHeight, { 0,0 }, &dwBytesWritten);
	WriteConsoleOutputCharacter(hConsole, buffer, screenWidth * screenHeight, { 0,0 }, &dwBytesWritten);
}
void GraphicsController::renderAt(int x, int y, string key) {
	vector<wstring> tmp = bufferStorage[key];
	for (int i = 0; i < tmp.size(); i++)
	{
		for (int j = 0; j < tmp[i].length(); j++)
		{
			COORD t;
			t.X = j;
			t.Y = i;
			WriteConsoleOutputAttribute(hConsole, &color[i * screenWidth + j], 1, t, &dwBytesWritten);
			WriteConsoleOutputCharacter(hConsole, &buffer[i * screenWidth + j], 1, t, &dwBytesWritten);
		}
	}
}

void GraphicsController::randomStars() {
	if (randomInterval != 10) {
		randomInterval++;
		copyStarBuffer();
		return;
	}

	randomInterval -= 10;
	for (int i = 0; i < 70; ++i) {
		int a = rand() % (screenWidth * screenHeight);
		starBuffer[a] = L'.';
	}
	copyStarBuffer();
}
void GraphicsController::clearStars() {
	if (randomInterval == 10)
		for (int i = 0; i < screenWidth * screenHeight; ++i) {
			starBuffer[i] = L' ';
		}
}
void GraphicsController::copyStarBuffer() {
	for (int i = 0; i < screenWidth * screenHeight; ++i) {
		if (starBuffer[i] == L'.') {
			buffer[i] = L'.';
			color[i] = black * 16 + white;
		}
	}
}
// custom star
void GraphicsController::randomStars(int bg, int ch) {
	if (randomInterval != 10) {
		randomInterval++;
		copyStarBuffer(bg, ch);
		return;
	}

	randomInterval -= 10;
	for (int i = 0; i < 70; ++i) {
		int a = rand() % (screenWidth * screenHeight);
		starBuffer[a] = L'.';
	}
	copyStarBuffer(bg, ch);
}
void GraphicsController::clearStars(int bg, int ch) {
	if (randomInterval == 10)
		for (int i = 0; i < screenWidth * screenHeight; ++i) {
			starBuffer[i] = L' ';
		}
}
void GraphicsController::copyStarBuffer(int bg, int ch) {
	for (int i = 0; i < screenWidth * screenHeight; ++i) {
		if (starBuffer[i] == L'.') {
			buffer[i] = L'.';
			color[i] = bg * 16 + ch;
		}
	}
}

void GraphicsController::glitch() {
	for (int i = 0; i < screenWidth * screenHeight; ++i) {
		color[i] = whiteDark * 16 + black;
	}
	render();
	delay(250);
	for (int i = 0; i < screenWidth * screenHeight; ++i) {
		color[i] = black * 16 + whiteDark;
	}
	render();
	delay(125);
	for (int i = 0; i < screenWidth * screenHeight; ++i) {
		color[i] = whiteDark * 16 + black;
	}
	render();
	delay(125);
}