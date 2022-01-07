#pragma once

#include "BufferStorage.h"

class GraphicsController {
protected:
	wchar_t* buffer, * starBuffer;
	WORD* color;

	HANDLE hConsole;
	DWORD dwBytesWritten;

	unordered_map<string, vector<wstring>> bufferStorage;

	int randomInterval = 10;
public:
	GraphicsController();
	~GraphicsController();
	void initClearBuffer();
	void initCharToBlock();
	void charToBlock(vector<wstring>& graphics);
	void openFrame(int x, int y, int w, int h);
	void createFrame(int x, int y, int w, int h, bool transparentBG = true, bool isBlackBG = true);
	vector<wstring>& getBuffer(string key);
	bool isInScreen(int y, int x);
	void setBuffer(vector<wstring>& content, int x, int y, int bgColor, int fgColor);
	void setBufferObject(vector<wstring>& content, int x, int y, int bgColor, int fgColor);
	void setBuffer(wstring content, int x, int y, int bgColor, int fgColor);
	void setBufferWhite(vector<wstring>& content, int x, int y, int bgColor, int fgColor);
	void clearBuffer();
	void clearBuffer(int bg, int ch);
	void render();
	void renderAt(int x, int y, string key);
	void randomStars();
	void clearStars();
	void copyStarBuffer();
	void randomStars(int bg, int ch);
	void clearStars(int bg, int ch);
	void copyStarBuffer(int bg, int ch);
	void glitch();
	wstring line(wstring pattern, int length);
	wstring time_to_wstring(int t);
	void progressBar(int elapsed, int duration, int x, int y);
};