#pragma once
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

const vector<char> key = {'W', 'A', 'S', 'D', 'P', 'R'};
const vector<int> threshold = {8, 13, 21, 34, 55};

// For sound
LPCWSTR song_intro{L"play song_intro.wav"};
LPCWSTR song_game_1{L"play song_game_1.wav"};
LPCWSTR song_game_2{L"play song_game_2.wav"};
LPCWSTR song_game_3{L"play song_game_3.wav"};
LPCWSTR song_game_4{L"play song_game_4.wav"};
LPCWSTR song_game_5{L"play song_game_5.wav"};
LPCWSTR click_menu{L"play click_menu.wav"};
LPCWSTR enter{L"play enter.wav"};
LPCWSTR start_level{L"play start_level.wav"};
LPCWSTR pass_lane{L"play pass_lane.wav"};
LPCWSTR pass_level{L"play pass_level.wav"};
LPCWSTR game_over{L"play game_over.wav"};
LPCWSTR silence{L"play silence.wav"};
LPCWSTR green_light{L"play green_light.wav"};
LPCWSTR red_light{L"play red_light.wav"};
LPCWSTR stage_clear{L"play stage_clear.wav"};

vector<int> songDuration = {60, 60, 60, 60, 60};

// For display time
auto startTime = chrono::system_clock::now();

class GameCore
{
protected:
	Player* player;
	GraphicsController* graphic;
	ILevel* levelController;
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
			vector<wstring> tmp = { L"█" };
			graphic->setBuffer(tmp, 31 + i, 24, BG, whiteDark);
			graphic->render();
			delay(1);
		}
		vector<wstring> tmp = { L"PRESS ENTER TO CONTINUE" };
		graphic->setBuffer(tmp, 58, 27, BG, whiteDark);
		graphic->render();

		int count = 0;
		while (!GetAsyncKeyState(VK_RETURN))
		{
			delay(1000 / FRAMERATE);
			count++;
			if (count == 480)
			{
				GameMenu* easterEgg1 = new Button("easterEgg1");
				graphic->setBuffer(graphic->getBuffer(easterEgg1->getBufferKey()), 28, 29, BG, 7);
				graphic->render();
			}
		}
		graphic->clearBuffer();
	}

	void titleScreen()
	{
		GameMenu* title = new Button(46, 8, "title");
		GameMenu* startButton = new Button("start");
		GameMenu* loadButton = new Button("load");
		GameMenu* settingButton = new Button("settings");
		GameMenu* exitButton = new Button("exit");
		Obstacles* enemy1 = new Obstacles(20, 5, 1, BG, 1, "enemy1", graphic);
		Obstacles* enemy2 = new Obstacles(135, 25, 2, BG, 3, "enemy2", graphic);
		Obstacles* enemy3 = new Obstacles(30, 30, 2, BG, 4, "enemy3", graphic);
		Obstacles* enemy4 = new Obstacles(115, 35, 3, BG, 6, "enemy4", graphic);

		int choice = 0;
		bool* bKeyGame = new bool[key.size()]{ 0 };
		if (soundOn)
			mciSendString(song_intro, NULL, 0, NULL);
		while (1)
		{
			// slow down the speed for "sensible" input
			delay(1000 / (FRAMERATE / 8));
			graphic->clearBuffer();
			graphic->clearStars();

			// enemies and stars
			graphic->randomStars();

			enemy1->render(graphic, 0);
			enemy2->render(graphic, 0);
			enemy3->render(graphic, 0);
			enemy4->render(graphic, 0);

			enemy1->move(2, 0);
			enemy2->move(-2, 0);
			enemy3->move(2, 0);
			enemy4->move(-2, 0);

			// default color
			graphic->setBufferWhite(graphic->getBuffer(title->getBufferKey()), 46, 11, BG, whiteDark);
			graphic->setBuffer(graphic->getBuffer(startButton->getBufferKey()), 68, 20, BG, whiteDark);
			graphic->setBuffer(graphic->getBuffer(loadButton->getBufferKey()), 68, 21, BG, whiteDark);
			graphic->setBuffer(graphic->getBuffer(settingButton->getBufferKey()), 68, 22, BG, whiteDark);
			graphic->setBuffer(graphic->getBuffer(exitButton->getBufferKey()), 68, 23, BG, whiteDark);

			// change color depends on choice
			switch (choice)
			{
			case 0:
				graphic->setBuffer(graphic->getBuffer(startButton->getBufferKey()), 68, 20, whiteDark, BG);
				break;
			case 1:
				graphic->setBuffer(graphic->getBuffer(loadButton->getBufferKey()), 68, 21, whiteDark, BG);
				break;
			case 2:
				graphic->setBuffer(graphic->getBuffer(settingButton->getBufferKey()), 68, 22, whiteDark, BG);
				break;
			case 3:
				graphic->setBuffer(graphic->getBuffer(exitButton->getBufferKey()), 68, 23, whiteDark, BG);
				break;
			default:
				break;
			}

			// input
			for (int i = 0; i < key.size(); i++)
				bKeyGame[i] = (GetAsyncKeyState(key.at(i))) != 0;
			if (GetAsyncKeyState(VK_RETURN))
			{
				if (soundOn)
					mciSendString(enter, NULL, 0, NULL);
				if (choice == 0)
				{
					int play = selectModeScreen();
					if (play)
					{
						mciSendString(L"pause song_intro.wav", NULL, 0, NULL);
						loadingScreen();
						int level = 1;
						while (level)
						{
							if (play == 1)
								level = playScreen1(level, 0); // aaaaaaaaaaaaaaaaaaaaaaaaa
							else
								level = playScreen(level);
						}
						if (soundOn)
							mciSendString(L"resume song_intro.wav", NULL, 0, NULL);
					}
				}
				else if (choice == 1)
				{
					loadScreen();
				}
				else if (choice == 2)
				{
					settingsScreen();
				}
				else
				{
					mciSendString(L"stop song_intro", NULL, 0, NULL);
					exitScreen();
				}
			}
			else if (bKeyGame[0] == 1)
			{
				if (soundOn)
					PlaySound(TEXT("menuClick.wav"), NULL, SND_FILENAME | SND_ASYNC);
				// mciSendStrings needs to finish the previous instance of the same sound first before playing it again
				// However, PlaySound will stop the previous PlaySound command and play the current one
				// so PlaySound should be used here
				choice = (choice + 4 - 1) % 4;
			}
			else if (bKeyGame[2] == 1)
			{
				if (soundOn)
					PlaySound(TEXT("menuClick.wav"), NULL, SND_FILENAME | SND_ASYNC);
				choice = (choice + 1) % 4;
			}

			// graphic->createFrame(0, 0, 145, 40);
			graphic->render();
		}
	}
	void loadingScreen()
	{ // cuz why not
		graphic->clearBuffer();
		graphic->createFrame(0, 0, 145, 40);
		graphic->createFrame(31, 27, 84, 3);
		for (int i = 1; i < 81; i += 5)
		{
			vector<wstring> tmp = { L"█████" };
			graphic->setBuffer(tmp, 32 + i, 28, BG, whiteDark);
			graphic->render();
			delay(83 - i);
		}
	}
	int selectModeScreen()
	{
		int top = 15;
		int left = 58;
		graphic->openFrame(left, top, 31, 9);
		GameMenu* chooseTitle = new Button("chooseTitle");
		graphic->setBufferWhite(graphic->getBuffer(chooseTitle->getBufferKey()), left + 1, top + 2, 0, 7);

		GameMenu* backButton = new Button("back");
		GameMenu* levelMode = new Button("levelMode");
		GameMenu* infinityMode = new Button("infinityMode");

		int choice = 0;
		bool* bKeyGame = new bool[key.size()]{ 0 }; // Check ingame input
		while (1)
		{
			delay(1000 / (FRAMERATE / 8));

			// default color
			graphic->setBuffer(graphic->getBuffer(levelMode->getBufferKey()), left + 8, top + 4, 0, 7);
			graphic->setBuffer(graphic->getBuffer(infinityMode->getBufferKey()), left + 8, top + 5, 0, 7);
			graphic->setBuffer(graphic->getBuffer(backButton->getBufferKey()), left + 10, top + 6, 0, 7);

			// input
			for (int i = 0; i < key.size(); i++)
				bKeyGame[i] = (GetAsyncKeyState(key.at(i))) != 0;
			if (GetAsyncKeyState(VK_RETURN))
			{ // Enter - select
				if (soundOn)
					mciSendString(enter, NULL, 0, NULL);
				if (choice == 0)
				{
					return 1;
				}
				else if (choice == 1)
				{
					return 2;
				}
				else
				{
					return 0;
				}
			}
			else if (bKeyGame[0] == 1)
			{ // W - Move up
				if (soundOn)
					PlaySound(TEXT("menuClick.wav"), NULL, SND_FILENAME | SND_ASYNC);
				choice = (choice - 1 + 3) % 3;
			}
			else if (bKeyGame[2] == 1)
			{ // S - Move down
				if (soundOn)
					PlaySound(TEXT("menuClick.wav"), NULL, SND_FILENAME | SND_ASYNC);
				choice = (choice + 1) % 3;
			}

			// change color depends on choice
			switch (choice)
			{
			case 0: // Level mode
				graphic->setBuffer(graphic->getBuffer(levelMode->getBufferKey()), left + 8, top + 4, 7, 0);
				break;
			case 1: // Ininity mode
				graphic->setBuffer(graphic->getBuffer(infinityMode->getBufferKey()), left + 8, top + 5, 7, 0);
				break;
			case 2: // Back
				graphic->setBuffer(graphic->getBuffer(backButton->getBufferKey()), left + 10, top + 6, 7, 0);
				break;
			default:
				break;
			}

			graphic->render();
		}
	}