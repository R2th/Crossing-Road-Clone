#include "System.h"
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ENTER 13
#define KEY_W 119
#define KEY_A 97
#define KEY_S 115
#define KEY_D 100
#define KEY_M 109
#define KEY_P 112
#define KEY_L 108
#define KEY_T 116
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
void FixConsoleWindow() {
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}
void SetWindow(int Width, int Height)
{
	_COORD coord;
	coord.X = Width;
	coord.Y = Height;

	_SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Bottom = Height - 1;
	Rect.Right = Width - 1;

	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);      // Get Handle 
	SetConsoleScreenBufferSize(Handle, coord);            // Set Buffer Size 
	SetConsoleWindowInfo(Handle, TRUE, &Rect);            // Set Window Size 
}
void setcursor(bool visible, DWORD size) // set bool visible = 0 - invisible, bool visible = 1 - visible
{
	if (size == 0)
	{
		size = 20;	// default cursor size Changing to numbers from 1 to 20, decreases cursor width
	}
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console, &lpCursor);
}
System::System() {
	mciSendString("open \"sound/BG_music.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
	mciSendString("play mp3", NULL, 0, NULL);
	mciSendString("stop mp3", NULL, 0, NULL);
	setcursor(0, 0);
	SetWindow(63, 23);
	FixConsoleWindow();
	IsSound = false;
	DifficultLevel = 1;
}
System::~System() {
	mciSendString("close mp3", NULL, 0, NULL);
}
void System::ShowMenuBegin() {
	cin.sync();
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	system("cls");
	int defaultBeginX = 24, defaultBeginY = 7;
	gotoxy(defaultBeginX, defaultBeginY);
	int pos = 0;
	int x = 0;
	string option[4] = {
		"New Game",
		"Load Game",
		"Setting",
		"Exit"
	};
	cout << " Menu";
	do {
		for (int i = 0; i < 4; i++) {
			gotoxy(defaultBeginX, defaultBeginY + 2 + i);
			if (i == pos) {
				SetConsoleTextAttribute(hConsole, 10);
				cout << option[i] << "\n";
				SetConsoleTextAttribute(hConsole, 15);
			}
			else cout << option[i] << "\n";
		}
		x = _getch();
		if (x == KEY_DOWN)
			pos++;
		if (x == KEY_UP)
			pos--;
		pos = (pos + 4) % 4;
	} while (x != KEY_ENTER);
	if (pos == 0) {
		this->StartGame();
	}
	if (pos == 1) {
		currentGame.Paused();
		this->LoadGame();
	}
	if (pos == 2)
		this->Setting(false);
}
void System::StartGame() {
	currentGame.ResetGame();
	this->MakeColor();
	currentGame.StartGame(this->DifficultLevel);
	this->HandleInputWhilePlay();
}
void System::MakeColor()
{
	system("cls");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	gotoxy(18, 8);
	cout << "Loading";

	SetConsoleTextAttribute(hConsole, 8);
	gotoxy(10, 11);  cout << "[";
	gotoxy(40, 11);  cout << "]";
	gotoxy(11, 11);
	for (int i = 11; i <= 39; i++) {
		cout << "#";
		Sleep(50);
	}
	SetConsoleTextAttribute(hConsole, 15);
}
bool System::LoadGame() {
	cin.sync();
	system("cls");
	string s;
	gotoxy(17, 10); cout << "Please Input File Name to Load: ";
	gotoxy(20, 11);  cin >> s;
	ifstream is;
	is.open(s.c_str(), ios::in | ios::binary);
	if (!is.is_open()) {
		gotoxy(23, 13);
		cout << "Cannot open File";
		Sleep(1500);
		currentGame.Resume();
		return true;
	}
	is.read((char *)&DifficultLevel, 4);
	currentGame.LoadGame(is);
	is.close();
	this->MakeColor();
	gotoxy(20, 13);
	cout << "Game has been loaded";
	Sleep(1000);
	currentGame.Resume();
	this->HandleInputWhilePlay();
	return false;
}
void System::Setting(bool IsPlay) {
	cin.sync();
	system("cls");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int defaultBeginX = 24, defaultBeginY = 7;
	gotoxy(defaultBeginX, defaultBeginY);
	cout << "  Setting";
	int pos = 0;
	int x = 0;
	string option[3] = {
		"Audio: ",
		"Difficult: ",
		"Back"
	};
	do {
		for (int i = 0; i < 3; i++) {
			gotoxy(defaultBeginX, defaultBeginY + 2 + i);
			if (i == pos) {
				if (IsPlay == false || i != 1) {
					SetConsoleTextAttribute(hConsole, 10);
					cout << option[i];
					SetConsoleTextAttribute(hConsole, 15);
				}
				else {
					SetConsoleTextAttribute(hConsole, 12);
					cout << option[i];
					SetConsoleTextAttribute(hConsole, 15);
				}
			}
			else cout << option[i];
			if (i == 0)
				if (this->IsSound) {
					cout << "On ";
				}
				else {
					cout << "Off";
				}
				if (i == 1) {
					if (this->DifficultLevel == 1)
						cout << "Easy   ";
					if (this->DifficultLevel == 2)
						cout << "Medium ";
					if (this->DifficultLevel == 3)
						cout << "Hard   ";
				}
		}
		x = _getch();
		if (x == KEY_ENTER) {
			if (pos == 2)
				break;
			if (pos == 0) {
				this->IsSound = !(this->IsSound);
				if (this->IsSound) PlayGameSound();
				else StopGameSound();
			}
			if (pos == 1 && IsPlay == false)
				this->DifficultLevel = (this->DifficultLevel) % 3 + 1;
		}
		if (x == KEY_DOWN)
			pos++;
		if (x == KEY_UP)
			pos--;
		pos = (pos + 3) % 3;
	} while (1);
	if (IsPlay == false)
		this->ShowMenuBegin();
}
void System::PlayGameSound() {
	mciSendString("play mp3 repeat", NULL, 0, NULL);
	//Sleep(2000);
	//mciSendString("play sound/Dino.wav", NULL, 0, NULL);
}
void System::StopGameSound() {
	mciSendString("stop mp3", NULL, 0, NULL);
}
void System::HandleInputWhilePlay() {
	bool IsStillPlay = true;
	int x = 0;
	while (IsStillPlay && currentGame.IsRunning()) {
		x = _getch();
		switch (x) {
		case KEY_W:
			if (!currentGame.IsPaused())
				IsStillPlay = currentGame.UpdatePosPlayer('w', true); break;
		case KEY_A:
			if (!currentGame.IsPaused())
				IsStillPlay = currentGame.UpdatePosPlayer('a', true); break;
		case KEY_S:
			if (!currentGame.IsPaused())
				IsStillPlay = currentGame.UpdatePosPlayer('s', true); break;
		case KEY_D:
			if (!currentGame.IsPaused())
				IsStillPlay = currentGame.UpdatePosPlayer('d', true); break;
		case KEY_UP:
			if (!currentGame.IsPaused())
				IsStillPlay = currentGame.UpdatePosPlayer('w', true); break;
		case KEY_LEFT:
			if (!currentGame.IsPaused())
				IsStillPlay = currentGame.UpdatePosPlayer('a', true); break;
		case KEY_DOWN:
			if (!currentGame.IsPaused())
				IsStillPlay = currentGame.UpdatePosPlayer('s', true); break;
		case KEY_RIGHT:
			if (!currentGame.IsPaused())
				IsStillPlay = currentGame.UpdatePosPlayer('d', true); break;
		case KEY_P:
			if (currentGame.IsPaused())
				currentGame.Resume();
			else currentGame.Paused();
			break;
		case KEY_M:
			currentGame.Paused();
			IsStillPlay = ShowMenuWhilePlay(); break;
		case KEY_L:
			currentGame.Paused();
			IsStillPlay = SaveGame(); break;
		case KEY_T:
			currentGame.Paused();
			IsStillPlay = LoadGame(); break;
		}
		cin.sync();
	}
	if (IsStillPlay) {
		this->ShowMenuBegin();
	}
}
bool System::ShowMenuWhilePlay() {
	cin.sync();
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	system("cls");
	int defaultBeginX = 24, defaultBeginY = 7;
	gotoxy(defaultBeginX, defaultBeginY);
	int pos = 0;
	int x = 0;
	string option[5] = {
		"Save Game",
		"Load Game",
		"Setting",
		"Resume",
		"Exit"
	};
	cout << " Menu";
	do {
		for (int i = 0; i < 5; i++) {
			gotoxy(defaultBeginX, defaultBeginY + 2 + i);
			if (i == pos) {
				SetConsoleTextAttribute(hConsole, 10);
				cout << option[i] << "\n";
				SetConsoleTextAttribute(hConsole, 15);
			}
			else cout << option[i] << "\n";
		}
		x = _getch();
		if (x == KEY_DOWN)
			pos++;
		if (x == KEY_UP)
			pos--;
		pos = (pos + 5) % 5;
	} while (x != KEY_ENTER);
	if (pos == 0) {
		return this->SaveGame();
	}
	if (pos == 1) {
		return this->LoadGame();
	}
	if (pos == 2) {
		this->Setting(true);
		currentGame.Resume();
		return true;
	}
	if (pos == 3) {
		currentGame.Resume();
		return true;
	}
	return false;
}
bool System::SaveGame() {
	system("cls");
	string s;
	gotoxy(17, 10); cout << "Please Input File Name to Save: ";
	gotoxy(20, 11);  cin >> s;
	ofstream os;
	os.open(s.c_str(), ios::out | ios::binary);
	if (!os.is_open()) {
		gotoxy(23, 13);
		cout << "Cannot open File";
		Sleep(1500);
		currentGame.Resume();
		return true;
	}
	os.write((char *)&DifficultLevel, 4);
	currentGame.SaveGame(os);
	os.close();
	gotoxy(20, 13);
	cout << "Game has been saved";
	Sleep(1000);
	currentGame.Resume();
	return true;
}