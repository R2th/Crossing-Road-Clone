#include <iostream>
#include <fstream>
#include <Windows.h>
#include <mmsystem.h>
#include <cstdlib>
#include <windows.h>
#include <iomanip>
#include <vector>
#include <thread>
#include <chrono>
#include <ctime> // Time
#include "Utils.h"
#include "GameObject.h"
#include "Player.h"
#include "Obstacles.h"
#include "GameLane.h"
#include "Items.h"
#include "Menu.h"
#include "GameMenu.h"
#include "GameLevel.h"
#include "GraphicsController.h"
using namespace std;

const vector<char> key = { 'W', 'A', 'S', 'D', 'P', 'R' };
const vector<int> threshold = { 8, 13, 21, 34, 55 };

// For sound
LPCWSTR song_intro{ L"play song_intro.wav" };
LPCWSTR song_game_1{ L"play song_game_1.wav" };
LPCWSTR song_game_2{ L"play song_game_2.wav" };
LPCWSTR song_game_3{ L"play song_game_3.wav" };
LPCWSTR song_game_4{ L"play song_game_4.wav" };
LPCWSTR song_game_5{ L"play song_game_5.wav" };
LPCWSTR click_menu{ L"play click_menu.wav" };
LPCWSTR enter{ L"play enter.wav" };
LPCWSTR start_level{ L"play start_level.wav" };
LPCWSTR pass_lane{ L"play pass_lane.wav" };
LPCWSTR pass_level{ L"play pass_level.wav" };
LPCWSTR game_over{ L"play game_over.wav" };
LPCWSTR silence{ L"play silence.wav" };
LPCWSTR green_light{ L"play green_light.wav" };
LPCWSTR red_light{ L"play red_light.wav" };
LPCWSTR stage_clear{ L"play stage_clear.wav" };

vector<int> songDuration = { 60, 60, 60, 60, 60 };

// For display time
auto startTime = chrono::system_clock::now();class GameCore
{
protected:
	Player *player;
	GraphicsController *graphic;
	ILevel *levelController;
	bool soundOn;
	vector<bool> saved;

public:
	GameCore()
	{
		graphic = new GraphicsController;
		player = new Player(70, 37, graphic);
		levelController = nullptr;
		soundOn = true;
		for (int i = 0; i < 5; ++i)
			saved.emplace_back(0);
		for (int i = 0; i < 5; ++i)
		{
			string tmp = "SaveFile";
			tmp += char(i + '1');
			tmp += ".txt";
			ifstream saveFile;
			saveFile.open(tmp);
			if (saveFile)
				saved[i] = 1;
			saveFile.close();
		}
		srand(time(0));
	}
	~GameCore()
	{
		delete graphic;
		delete player;
		delete levelController;
	}

	void start()
	{
		introScreen();
		titleScreen();
	}
	void introScreen()
	{
		graphic->createFrame(0, 0, 145, 40);

		// loading bar, cuz why not
		graphic->createFrame(30, 23, 85, 3);
		for (int i = 1; i < 82; ++i)
		{
			vector<wstring> tmp = {L"█"};
			graphic->setBuffer(tmp, 31 + i, 24, BG, whiteDark);
			graphic->render();
			delay(1);
		}
		vector<wstring> tmp = {L"PRESS ENTER TO CONTINUE"};
		graphic->setBuffer(tmp, 58, 27, BG, whiteDark);
		graphic->render();

		int count = 0;
		while (!GetAsyncKeyState(VK_RETURN))
		{
			delay(1000 / FRAMERATE);
			count++;
			if (count == 480)
			{
				GameMenu *easterEgg1 = new Button("easterEgg1");
				graphic->setBuffer(graphic->getBuffer(easterEgg1->getBufferKey()), 28, 29, BG, 7);
				graphic->render();
			}
		}
		graphic->clearBuffer();
	}