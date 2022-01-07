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
	}void loadingScreen()
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
	}void clearLevelScreen(int Level, int Score)
	{
		int top = 15;
		int left = 57;
		graphic->openFrame(left, top, 34, 15);

		GameMenu* clearLevelTitle = new Button("clearLevelTitle");
		graphic->setBufferWhite(graphic->getBuffer(clearLevelTitle->getBufferKey()), left + 1, top + 2, 0, 7);
		GameMenu* continueButton = new Button("continueButton");

		int choice = 0;
		bool* bKeyGame = new bool[key.size()]{ 0 }; // Check ingame input
		while (1)
		{
			delay(1000 / (FRAMERATE / 8));

			// default color
			graphic->setBuffer(L"YOU PASSED LEVEL " + to_wstring(Level) + L" WITH:", left + 4, top + 4, 0, 7);
			graphic->setBuffer(to_wstring(Score), left + 15, top + 6, 0, 7);
			graphic->setBuffer(L"SCORE TO PASS NEXT LEVEL: ", left + 4, top + 8, 0, 7);
			graphic->setBuffer(to_wstring(threshold[Level]), left + 15, top + 10, 0, 7);
			graphic->setBuffer(graphic->getBuffer(continueButton->getBufferKey()), left + 10, top + 12, 7, 0);

			// input
			for (int i = 0; i < key.size(); i++)
				bKeyGame[i] = (GetAsyncKeyState(key.at(i))) != 0;
			if (GetAsyncKeyState(VK_RETURN))
			{ // Enter - select
				if (soundOn)
				{
					mciSendString(enter, NULL, 0, NULL);
					return;
				}
				Sleep(1000);
			}

			graphic->render();
		}
	}void firstLevelScreen()
	{
		int top = 15;
		int left = 57;
		graphic->clearBuffer(blueDark, white);
		graphic->openFrame(left, top, 33, 9);

		GameMenu* clearLevelTitle = new Button("clearLevelTitle");
		// graphic->setBufferWhite(graphic->getBuffer(clearLevelTitle->getBufferKey()), left + 1, top + 2, 0, 7);
		GameMenu* continueButton = new Button("continueButton");

		int choice = 0;
		bool* bKeyGame = new bool[key.size()]{ 0 }; // Check ingame input
		while (1)
		{
			delay(1000 / (FRAMERATE / 8));

			// default color
			graphic->setBuffer(L"SCORE TO PASS LEVEL 1:", left + 6, top + 2, 0, 7);
			graphic->setBuffer(to_wstring(threshold[0]), left + 15, top + 4, 0, 7);
			graphic->setBuffer(graphic->getBuffer(continueButton->getBufferKey()), left + 10, top + 6, 7, 0);

			// input
			for (int i = 0; i < key.size(); i++)
				bKeyGame[i] = (GetAsyncKeyState(key.at(i))) != 0;
			if (GetAsyncKeyState(VK_RETURN))
			{ // Enter - select
				if (soundOn)
				{
					mciSendString(enter, NULL, 0, NULL);
					return;
				}
				Sleep(1000);
			}

			graphic->render();
		}
	}int playScreen1(int Level, int baseScore)
	{
		// Stage clear
		if (Level == threshold.size() + 1)
			return 0;
		if (Level == 1)
			firstLevelScreen();

		// Music option
		switch (Level)
		{
		case 1:
		{
			mciSendString(song_game_1, NULL, 0, NULL);
			if (!soundOn)
				mciSendString(L"pause song_game_1.wav", NULL, 0, NULL);
			break;
		}
		case 2:
		{
			mciSendString(L"stop song_game_1.wav", NULL, 0, NULL);
			mciSendString(song_game_2, NULL, 0, NULL);
			if (!soundOn)
				mciSendString(L"pause song_game_2.wav", NULL, 0, NULL);
			break;
		}
		case 3:
		{
			mciSendString(L"stop song_game_2.wav", NULL, 0, NULL);
			mciSendString(song_game_3, NULL, 0, NULL);
			if (!soundOn)
				mciSendString(L"pause song_game_3.wav", NULL, 0, NULL);
			break;
		}
		case 4:
		{
			mciSendString(L"stop song_game_3.wav", NULL, 0, NULL);
			mciSendString(song_game_4, NULL, 0, NULL);
			if (!soundOn)
				mciSendString(L"pause song_game_4.wav", NULL, 0, NULL);
			break;
		}
		case 5:
		{
			mciSendString(L"stop song_game_4.wav", NULL, 0, NULL);
			mciSendString(song_game_5, NULL, 0, NULL);
			if (!soundOn)
				mciSendString(L"pause song_game_5.wav", NULL, 0, NULL);
			break;
		}
		default:
			break;
		}

		// Background color
		int bg = blueDark, ch = white;
		graphic->clearBuffer(bg, ch);

		// Player
		player->setPos(72, 2);

		// Lanes
		bool order = true;
		vector<GameLane*> lanes;
		levelController = new Level_1;
		levelController->getMap(lanes, graphic, bg, order, Level);

		// Score
		int Score; // In the scope of this level
		GameMenu* score = new Button("score");
		GameMenu* level = new Button("level");
		vector<wstring> scoreCounter, levelCounter;
		graphic->render();
		// Input
		bool* bKeyGame = new bool[key.size()]{ 0 };

		// scroll
		int offset = 0;
		int nLane = 7;

		// Time
		startTime = chrono::system_clock::now();

		while (1)
		{
			// Initialize
			delay(1000 / (FRAMERATE - 20));
			graphic->clearBuffer(bg, ch);
			graphic->clearStars(bg, ch);
			player->render(graphic, offset, bg, ch);
			player->update();

			// Stars
			graphic->randomStars(bg, ch);

			// Lanes shananigans
			for (auto l : lanes)
				l->logic(); // Lights
			for (auto l : lanes)
				l->render(graphic, offset, bg);
			if (this->checkCollision(lanes))
			{
				switch (Level)
				{
				case 1:
				{
					mciSendString(L"pause song_game_1.wav", NULL, 0, NULL);
					break;
				}
				case 2:
				{
					mciSendString(L"pause song_game_2.wav", NULL, 0, NULL);
					break;
				}
				case 3:
				{
					mciSendString(L"pause song_game_3.wav", NULL, 0, NULL);
					break;
				}
				case 4:
				{
					mciSendString(L"pause song_game_4.wav", NULL, 0, NULL);
					break;
				}
				case 5:
				{
					mciSendString(L"pause song_game_5.wav", NULL, 0, NULL);
					break;
				}
				default:
					break;
				}
				mciSendString(L"play crash.wav", NULL, 0, NULL);
				graphic->glitch();

				delay(400);
				gameoverScreen(Score);
				graphic->clearBuffer();
				return 0;
			}

			// player render
			player->render(graphic, offset, bg, ch);

			// score and level
			Score = player->getBoundingBox().y / LANE_HEIGHT;
			toVwstring(Score + baseScore, scoreCounter);
			toVwstring(Level, levelCounter);
			graphic->setBufferWhite(graphic->getBuffer(score->getBufferKey()), 2, 1, bg, ch);
			graphic->setBufferWhite(scoreCounter, 9, 1, bg, ch);
			graphic->setBufferWhite(graphic->getBuffer(level->getBufferKey()), 2, 2, bg, ch);
			graphic->setBufferWhite(levelCounter, 9, 2, bg, ch);

			// Time
			auto endTime = chrono::system_clock::now();
			chrono::duration<double> elapsed_seconds = endTime - startTime;
			int elapsed = elapsed_seconds.count();
			graphic->progressBar(elapsed, songDuration[Level - 1], 20, 1);

			// Check score and level pass
			if (elapsed > songDuration[Level - 1])
			{
				if (baseScore + Score >= threshold[Level - 1])
				{
					clearLevelScreen(Level, Score);
					playScreen1(Level + 1, 0);
					return 0;
				}
				else
				{
					// if (elapsed > songDuration[Level - 1]) {
					if (soundOn)
						switch (Level)
						{
						case 1:
							mciSendString(L"stop song_game_1", NULL, 0, NULL);
							break;
						case 2:
							mciSendString(L"stop song_game_2", NULL, 0, NULL);
							break;
						case 3:
							mciSendString(L"stop song_game_3", NULL, 0, NULL);
							break;
						case 4:
							mciSendString(L"stop song_game_4", NULL, 0, NULL);
							break;
						case 5:
							mciSendString(L"stop song_game_5", NULL, 0, NULL);
							break;
						default:
							break;
						}
					gameoverScreen(Score);
					graphic->clearBuffer();
					return 0;
					//}
				}
			}

			// Controls
			for (int i = 0; i < key.size(); i++)
			{ // Read input
				bKeyGame[i] = (GetAsyncKeyState(key.at(i))) != 0;
			}
			if (bKeyGame[4] == 1 || GetAsyncKeyState(VK_ESCAPE))
			{
				if (soundOn)
					mciSendString(enter, NULL, 0, NULL);
				if (!pauseScreen(player->getBoundingBox().y / LANE_HEIGHT + baseScore, 0, Level))
					return Level;
				if (soundOn)
					switch (Level)
					{
					case 1:
						mciSendString(L"resume song_game_1", NULL, 0, NULL);
						break;
					case 2:
						mciSendString(L"resume song_game_2", NULL, 0, NULL);
						break;
					case 3:
						mciSendString(L"resume song_game_3", NULL, 0, NULL);
						break;
					case 4:
						mciSendString(L"resume song_game_4", NULL, 0, NULL);
						break;
					case 5:
						mciSendString(L"resume song_game_5", NULL, 0, NULL);
						break;
					default:
						break;
					}
			}
			if (bKeyGame[0] == 1 && player->getPos().y > 1)
			{
				if (player->getPos().y + offset <= 5 && player->getPos().y > 5)
					offset++;
				player->move(0, -1);
			}
			if (bKeyGame[1] == 1 && player->getPos().x > 1)
			{
				player->move(-1, 0);
			}
			if (bKeyGame[2] == 1 && player->getPos().y < LANE_HEIGHT * nLane)
			{
				if (player->getPos().y + offset >= screenHeight - 30 && player->getPos().y + 5 <= 18 + nLane * LANE_HEIGHT)
					offset--;
				//	int addLanes(vector<GameLane*>& lanes, GraphicsController* graphic, int laneIndex) return number of new lanes added
				if (player->getPos().y > (nLane - 3) * LANE_HEIGHT)
				{
					nLane += levelController->addLanes(lanes, graphic, nLane + 1, bg, order, Level);
				}
				player->move(0, 1);
			}
			if (bKeyGame[3] == 1 && player->getPos().x < screenWidth - 1 - graphic->getBuffer(player->getBufferKey())[0].length())
			{
				player->move(1, 0);
			}

			graphic->render();
		}
	}int playScreen(int Level, int baseScore = 0)
	{
		mciSendString(song_game_1, NULL, 0, NULL);
		if (!soundOn)
			mciSendString(L"pause song_game_1.wav", NULL, 0, NULL);
		// theses lines are for the case when user turn off the sound -> game over -> start a new one -> turn on sound
		// or turn off sound -> play game -> turn on sound
		graphic->clearBuffer();
		player->setPos(72, 2);
		vector<GameLane*> lanes;
		if (Level == 1)
		{
			levelController = new Level_1;
			levelController->getMap(lanes, graphic);
		}
		GameMenu* score = new Button("score");
		GameMenu* level = new Button("level");
		vector<wstring> scoreCounter;
		bool* bKeyGame = new bool[key.size()]{ 0 };

		// scroll
		int offset = 0;
		int nLane = 7;

		while (1)
		{
			// Initialize
			delay(1000 / (FRAMERATE - 20));
			graphic->clearBuffer();
			graphic->clearStars();
			player->render(graphic, offset);
			player->update();

			// Stars
			graphic->randomStars();

			// lanes shananigans
			for (auto l : lanes)
				l->logic();
			for (auto l : lanes)
				l->render(graphic, offset);
			if (this->checkCollision(lanes))
			{
				mciSendString(L"stop song_game_1.wav", NULL, 0, NULL);
				mciSendString(L"play crash.wav", NULL, 0, NULL);
				graphic->glitch();

				delay(400);
				gameoverScreen(player->getBoundingBox().y / LANE_HEIGHT + baseScore);
				graphic->clearBuffer();
				for (auto l : lanes)
					delete l;
				return 0;
			}

			// player render
			player->render(graphic, offset);

			// Score
			toVwstring(player->getBoundingBox().y / LANE_HEIGHT + baseScore, scoreCounter);
			graphic->setBufferWhite(graphic->getBuffer(score->getBufferKey()), 2, 1, BG, 7);
			graphic->setBufferWhite(scoreCounter, 9, 1, BG, 7);

			// frame around screen
			graphic->createFrame(0, 0, 145, 40);

			// Controls
			for (int i = 0; i < key.size(); i++)
			{ // Read input
				bKeyGame[i] = (GetAsyncKeyState(key.at(i))) != 0;
			}
			if (bKeyGame[4] == 1 || GetAsyncKeyState(VK_ESCAPE))
			{
				if (soundOn)
					mciSendString(enter, NULL, 0, NULL);
				if (!pauseScreen(player->getBoundingBox().y / LANE_HEIGHT + baseScore, 1))
					return Level;
				if (soundOn)
					mciSendString(L"resume song_game_1.wav", NULL, 0, NULL);
			}
			if (bKeyGame[0] == 1 && player->getPos().y > 1)
			{
				if (player->getPos().y + offset <= 5 && player->getPos().y > 5)
					offset++;
				player->move(0, -1);
			}
			if (bKeyGame[1] == 1 && player->getPos().x > 1)
			{
				player->move(-1, 0);
			}
			if (bKeyGame[2] == 1 && player->getPos().y < LANE_HEIGHT * nLane)
			{
				if (player->getPos().y + offset >= screenHeight - 30 && player->getPos().y + 5 <= 18 + nLane * LANE_HEIGHT)
					offset--;
				if (player->getPos().y > (nLane - 3) * LANE_HEIGHT)
				{
					nLane += levelController->addLanes(lanes, graphic, nLane + 1);
				}
				player->move(0, 1);
			}
			if (bKeyGame[3] == 1 && player->getPos().x < screenWidth - 1 - graphic->getBuffer(player->getBufferKey())[0].length())
			{
				player->move(1, 0);
			}

			graphic->render();
		}
	}
	void saveScreen(int score, int mode = 0, int level = 0)
	{
		int top = 8;
		int left = 58;
		vector<wstring> blank = { L"███████████████████████" };
		vector<wstring> save{ L"" };
		vector<wstring> gameScore;
		graphic->openFrame(left, top - 1, 29, 24);
		GameMenu* saveTitle = new Button("saveTitle");
		graphic->setBufferWhite(graphic->getBuffer(saveTitle->getBufferKey()), left + 1, top + 1, 0, 7);
		GameMenu* slots = new Button("slots");
		graphic->createFrame(left + 2, top + 4, blank[0].length() + 2, 3);
		graphic->createFrame(left + 2, top + 7, blank[0].length() + 2, 3);
		graphic->createFrame(left + 2, top + 10, blank[0].length() + 2, 3);
		graphic->createFrame(left + 2, top + 13, blank[0].length() + 2, 3);
		graphic->createFrame(left + 2, top + 16, blank[0].length() + 2, 3);
		GameMenu* backButton = new Button("back");
		int choice = 0;
		bool* bKeyGame = new bool[key.size()]{ 0 };

		while (1)
		{
			delay(1000 / (FRAMERATE / 8));

			// default color
			graphic->setBuffer(graphic->getBuffer(slots->getBufferKey()), left + 7, top + 2, 0, 7);
			for (int i = 0; i < 5; ++i)
			{
				if (saved[i])
				{
					string tmp = "SaveFile";
					tmp += char(i + '1');
					tmp += ".txt";
					ifstream saveFile{ tmp };
					int tmpScore;
					saveFile >> tmpScore;

					string mode;
					saveFile >> mode;
					wstring tmpppp;
					if (mode[0] == 'i')
					{
						save[0] = L"Endless. Score: ";
						tmpppp = toWstring(tmpScore);
					}
					else
					{
						save[0] = L"Timed. Level: ";
						tmpppp = toWstring(level);
					}
					save[0] += tmpppp;

					int saveLength = save[0].length();

					int n = blank[0].length() - save[0].length();
					for (int j = 0; j < n; ++j)
					{
						save[0] += L' ';
					}
					graphic->setBufferWhite(save, left + 3, top + 5 + 3 * i, BG, whiteDark);

					save[0].erase(saveLength, save[0].length() - tmpppp.length());
					saveFile.close();
				}
				else
					graphic->setBufferWhite(blank, left + 3, top + 5 + 3 * i, whiteDark, BG);
			}
			graphic->setBuffer(graphic->getBuffer(backButton->getBufferKey()), left + 9, top + 20, 0, 7);

			// input
			for (int i = 0; i < key.size(); i++)
				bKeyGame[i] = (GetAsyncKeyState(key.at(i))) != 0;
			if (GetAsyncKeyState(VK_RETURN))
			{
				if (soundOn)
					mciSendString(enter, NULL, 0, NULL);
				if (choice == 5)
					return;
				else
				{
					choice = (choice + 6) % 6;
					saved[choice] = 1;
					string tmp = "SaveFile";
					tmp += char(choice + '1');
					tmp += ".txt";
					ofstream saveFile{ tmp };
					saveFile << score;
					saveFile << (mode ? " inf " : " lvl ");
					if (!mode)
						saveFile << level;
					saveFile.close();
					return;
				}
			}
			else if (bKeyGame[0] == 1)
			{
				if (soundOn)
					PlaySound(TEXT("menuClick.wav"), NULL, SND_FILENAME | SND_ASYNC);
				choice = (choice + 6 - 1) % 6;
			}
			else if (bKeyGame[2] == 1)
			{
				if (soundOn)
					PlaySound(TEXT("menuClick.wav"), NULL, SND_FILENAME | SND_ASYNC);
				choice = (choice + 1) % 6;
			}

			// change color depends on choice
			switch (choice)
			{
			case 5:
				graphic->setBuffer(graphic->getBuffer(backButton->getBufferKey()), left + 9, top + 20, 7, 0);
				break;
			default:
				if (saved[choice])
				{
					string tmp = "SaveFile";
					tmp += char(choice + '1');
					tmp += ".txt";
					ifstream saveFile{ tmp };
					int tmpScore;
					saveFile >> tmpScore;

					string mode;
					saveFile >> mode;
					wstring tmpppp;
					if (mode[0] == 'i')
					{
						save[0] = L"Endless. Score: ";
						tmpppp = toWstring(tmpScore);
					}
					else
					{
						save[0] = L"Timed. Level: ";
						tmpppp = toWstring(level);
					}
					save[0] += tmpppp;

					int saveLength = save[0].length();

					int n = blank[0].length() - save[0].length();
					for (int j = 0; j < n; ++j)
					{
						save[0] += L' ';
					}
					graphic->setBufferWhite(save, left + 3, top + 5 + 3 * choice, whiteDark, BG);

					save[0].erase(saveLength, save[0].length() - tmpppp.length());
					saveFile.close();
				}
				else
					graphic->setBufferWhite(blank, left + 3, top + 5 + 3 * choice, BG, whiteDark);
				break;
			}

			graphic->render();
		}
	};
	void loadScreen()
	{
		int top = 8;
		int left = 58;
		vector<wstring> blank = { L"███████████████████████" };
		vector<wstring> save{ L"" };
		vector<wstring> gameScore;
		graphic->openFrame(left, top - 1, 29, 24);
		GameMenu* saveTitle = new Button("loadTitle");
		graphic->setBufferWhite(graphic->getBuffer(saveTitle->getBufferKey()), left + 1, top + 1, 0, 7);
		GameMenu* slots = new Button("slots");
		graphic->createFrame(left + 2, top + 4, blank[0].length() + 2, 3);
		graphic->createFrame(left + 2, top + 7, blank[0].length() + 2, 3);
		graphic->createFrame(left + 2, top + 10, blank[0].length() + 2, 3);
		graphic->createFrame(left + 2, top + 13, blank[0].length() + 2, 3);
		graphic->createFrame(left + 2, top + 16, blank[0].length() + 2, 3);
		GameMenu* backButton = new Button("back");
		int choice = 0;
		bool* bKeyGame = new bool[key.size()]{ 0 };

		while (1)
		{
			delay(1000 / (FRAMERATE / 8));

			// default color
			graphic->setBuffer(graphic->getBuffer(slots->getBufferKey()), left + 7, top + 2, 0, 7);
			for (int i = 0; i < 5; ++i)
			{
				if (saved[i])
				{
					string tmp = "SaveFile";
					tmp += char(i + '1');
					tmp += ".txt";
					ifstream saveFile{ tmp };
					int tmpScore;
					saveFile >> tmpScore;

					string mode;
					saveFile >> mode;
					wstring tmpppp;
					if (mode[0] == 'i')
					{
						save[0] = L"Endless. Score: ";
						tmpppp = toWstring(tmpScore);
					}
					else
					{
						int Level;
						saveFile >> Level;
						save[0] = L"Timed. Level: ";
						tmpppp = toWstring(Level);
					}
					save[0] += tmpppp;

					int saveLength = save[0].length();

					int n = blank[0].length() - save[0].length();
					for (int j = 0; j < n; ++j)
					{
						save[0] += L' ';
					}
					graphic->setBufferWhite(save, left + 3, top + 5 + 3 * i, BG, whiteDark);

					save[0].erase(saveLength, save[0].length() - tmpppp.length());
					saveFile.close();
				}
				else
					graphic->setBufferWhite(blank, left + 3, top + 5 + 3 * i, whiteDark, BG);
			}
			graphic->setBuffer(graphic->getBuffer(backButton->getBufferKey()), left + 9, top + 20, 0, 7);

			// input
			for (int i = 0; i < key.size(); i++)
				bKeyGame[i] = (GetAsyncKeyState(key.at(i))) != 0;
			if (GetAsyncKeyState(VK_RETURN))
			{
				if (soundOn)
					mciSendString(enter, NULL, 0, NULL);
				if (choice == 5)
					return;
				else
				{
					string tmp = "SaveFile";
					tmp += char(choice + '1');
					tmp += ".txt";
					ifstream saveFile{ tmp };
					int score;
					saveFile >> score;
					string mode;
					saveFile >> mode;

					if (mode[0] == 'i')
					{
						mciSendString(L"pause song_intro.wav", NULL, 0, NULL);
						loadingScreen();
						int level = 1;
						while (level)
						{
							level = playScreen(level, score);
						}
						if (soundOn)
							mciSendString(L"resume song_intro.wav", NULL, 0, NULL);
					}
					else
					{
						int level;
						saveFile >> level;
						mciSendString(L"pause song_intro.wav", NULL, 0, NULL);
						loadingScreen();
						while (level)
						{
							level = playScreen1(level, 0);
						}
						if (soundOn)
							mciSendString(L"resume song_intro.wav", NULL, 0, NULL);
					}

					saveFile.close();
					return;
				}
			}
			else if (bKeyGame[0] == 1)
			{
				if (soundOn)
					PlaySound(TEXT("menuClick.wav"), NULL, SND_FILENAME | SND_ASYNC);
				choice = (choice + 6 - 1) % 6;
			}
			else if (bKeyGame[2] == 1)
			{
				if (soundOn)
					PlaySound(TEXT("menuClick.wav"), NULL, SND_FILENAME | SND_ASYNC);
				choice = (choice + 1) % 6;
			}

			// change color depends on choice
			switch (choice)
			{
			case 5:
				graphic->setBuffer(graphic->getBuffer(backButton->getBufferKey()), left + 9, top + 20, 7, 0);
				break;
			default:
				if (saved[choice])
				{
					string tmp = "SaveFile";
					tmp += char(choice + '1');
					tmp += ".txt";
					ifstream saveFile{ tmp };
					int tmpScore;
					saveFile >> tmpScore;

					string mode;
					saveFile >> mode;
					wstring tmpppp;
					if (mode[0] == 'i')
					{
						save[0] = L"Endless. Score: ";
						tmpppp = toWstring(tmpScore);
					}
					else
					{
						int Level;
						saveFile >> Level;
						save[0] = L"Timed. Level: ";
						tmpppp = toWstring(Level);
					}
					save[0] += tmpppp;

					int saveLength = save[0].length();

					int n = blank[0].length() - save[0].length();
					for (int j = 0; j < n; ++j)
					{
						save[0] += L' ';
					}
					graphic->setBufferWhite(save, left + 3, top + 5 + 3 * choice, whiteDark, BG);

					save[0].erase(saveLength, save[0].length() - tmpppp.length());
					saveFile.close();
				}
				else
					graphic->setBufferWhite(blank, left + 3, top + 5 + 3 * choice, BG, whiteDark);
				break;
			}

			graphic->render();
		}
	};

	void settingsScreen()
	{
		int top = 19;
		int left = 58;
		graphic->openFrame(left, top, 28, 8);
		GameMenu* settingsTitle = new Button("settingsTitle");
		graphic->setBufferWhite(graphic->getBuffer(settingsTitle->getBufferKey()), left + 1, top + 2, 0, 7);

		GameMenu* backButton = new Button("back");
		GameMenu* soundButton = new Button("sound");
		GameMenu* on = new Button("on");
		GameMenu* off = new Button("off");

		int choice = 0;
		bool* bKeyGame = new bool[key.size()]{ 0 }; // Check ingame input
		while (1)
		{
			delay(1000 / (FRAMERATE / 8));

			// default color
			graphic->setBuffer(graphic->getBuffer(soundButton->getBufferKey()), left + 9, top + 4, 0, 7);
			if (soundOn)
				graphic->setBuffer(graphic->getBuffer(on->getBufferKey()), left + 9 + 6, top + 4, 0, 7);
			else
				graphic->setBuffer(graphic->getBuffer(off->getBufferKey()), left + 9 + 6, top + 4, 0, 7);
			graphic->setBuffer(graphic->getBuffer(backButton->getBufferKey()), left + 9, top + 5, 0, 7);

			// input
			for (int i = 0; i < key.size(); i++)
				bKeyGame[i] = (GetAsyncKeyState(key.at(i))) != 0;
			if (GetAsyncKeyState(VK_RETURN))
			{
				if (soundOn)
					mciSendString(enter, NULL, 0, NULL);
				if (choice == 0)
				{
					soundOn = !soundOn;
					if (soundOn)
						mciSendString(L"resume song_intro.wav", NULL, 0, NULL);
					else
						mciSendString(L"pause song_intro.wav", NULL, 0, NULL);
				}
				else
				{
					graphic->clearBuffer();
					return;
				}
			}
			else if (bKeyGame[0] == 1)
			{
				if (soundOn)
					PlaySound(TEXT("menuClick.wav"), NULL, SND_FILENAME | SND_ASYNC);
				choice = (choice + 2 - 1) % 2;
			}
			else if (bKeyGame[2] == 1)
			{
				if (soundOn)
					PlaySound(TEXT("menuClick.wav"), NULL, SND_FILENAME | SND_ASYNC);
				choice = (choice + 1) % 2;
			}

			// change color depends on choice
			switch (choice)
			{
			case 0:
				graphic->setBuffer(graphic->getBuffer(soundButton->getBufferKey()), left + 9, top + 4, 7, 0);
				if (soundOn)
					graphic->setBuffer(graphic->getBuffer(on->getBufferKey()), left + 9 + 6, top + 4, 7, 0);
				else
					graphic->setBuffer(graphic->getBuffer(off->getBufferKey()), left + 9 + 6, top + 4, 7, 0);
				break;
			case 1:
				graphic->setBuffer(graphic->getBuffer(backButton->getBufferKey()), left + 9, top + 5, 7, 0);
				break;
			default:
				break;
			}

			graphic->render();
		}
	};
	bool pauseScreen(int score = 0, int mode = 0, int level = 0)
	{
		int top = 15;
		int left = 60;
		graphic->openFrame(left, top, 25, 10);
		GameMenu* pauseTitle = new Button("pauseTitle");
		graphic->setBufferWhite(graphic->getBuffer(pauseTitle->getBufferKey()), left + 1, top + 2, 0, 7);
		GameMenu* resumeButton = new Button("resumeButton");
		GameMenu* restartButton = new Button("restartButton");
		GameMenu* saveButton = new Button("saveButton");
		GameMenu* soundButton = new Button("sound");
		GameMenu* on = new Button("on");
		GameMenu* off = new Button("off");
		int choice = 0;
		bool* bKeyGame = new bool[key.size()]{ 0 };

		while (1)
		{
			delay(1000 / (FRAMERATE / 8));
			// default color
			graphic->setBuffer(graphic->getBuffer(resumeButton->getBufferKey()), left + 7, top + 4, 0, 7);
			graphic->setBuffer(graphic->getBuffer(restartButton->getBufferKey()), left + 7, top + 5, 0, 7);
			graphic->setBuffer(graphic->getBuffer(saveButton->getBufferKey()), left + 7, top + 6, 0, 7);
			graphic->setBuffer(graphic->getBuffer(soundButton->getBufferKey()), left + 7, top + 7, 0, 7);
			if (soundOn)
				graphic->setBuffer(graphic->getBuffer(on->getBufferKey()), left + 13, top + 7, 0, 7);
			else
				graphic->setBuffer(graphic->getBuffer(off->getBufferKey()), left + 13, top + 7, 0, 7);

			// input
			for (int i = 0; i < key.size(); i++)
				bKeyGame[i] = (GetAsyncKeyState(key.at(i))) != 0;
			if (GetAsyncKeyState(VK_RETURN))
			{
				if (soundOn)
					mciSendString(enter, NULL, 0, NULL);
				if (choice == 3)
				{ // sound
					soundOn = !soundOn;
					if (soundOn)
						mciSendString(L"resume song_game_1.wav", NULL, 0, NULL);
					else
						mciSendString(L"pause song_game_1.wav", NULL, 0, NULL);
				}
				else if (choice == 0)
				{ // resume
					graphic->clearBuffer();
					return 1;
				}
				else if (choice == 2)
				{ // save
					saveScreen(score, mode, level);
					return 1;
				}
				else
				{ // restart
					mciSendString(L"stop song_game_1.wav", NULL, 0, NULL);
					return 0;
				}
			}
			else if (bKeyGame[0] == 1)
			{
				if (soundOn)
					PlaySound(TEXT("menuClick.wav"), NULL, SND_FILENAME | SND_ASYNC);
				choice = (choice + 4 - 1) % 4;
			}
			else if (bKeyGame[2] == 1)
			{
				if (soundOn)
					PlaySound(TEXT("menuClick.wav"), NULL, SND_FILENAME | SND_ASYNC);
				choice = (choice + 1) % 4;
			}

			// change color depends on choice
			switch (choice)
			{
			case 0:
				graphic->setBuffer(graphic->getBuffer(resumeButton->getBufferKey()), left + 7, top + 4, 7, 0);
				break;
			case 1:
				graphic->setBuffer(graphic->getBuffer(restartButton->getBufferKey()), left + 7, top + 5, 7, 0);
				break;
			case 2:
				graphic->setBuffer(graphic->getBuffer(saveButton->getBufferKey()), left + 7, top + 6, 7, 0);
				break;
			case 3:
				graphic->setBuffer(graphic->getBuffer(soundButton->getBufferKey()), left + 7, top + 7, 7, 0);
				if (soundOn)
					graphic->setBuffer(graphic->getBuffer(on->getBufferKey()), left + 13, top + 7, 7, 0);
				else
					graphic->setBuffer(graphic->getBuffer(off->getBufferKey()), left + 13, top + 7, 7, 0);
				break;
			default:
				break;
			}

			graphic->render();
		}
	};

	void exitScreen()
	{
		graphic->clearBuffer();
		GameMenu* exitTitle = new Button("exitTitle");
		graphic->setBuffer(graphic->getBuffer(exitTitle->getBufferKey()), 46, 19, BG, 7);
		graphic->createFrame(0, 0, 145, 40);
		graphic->render();
		while (1)
		{
			delay(1000 / (FRAMERATE / 10));
			if (GetAsyncKeyState(VK_RETURN))
				exit(0);
		}
	}
	void gameoverScreen(int score = 0)
	{
		int top = 8;
		int left = 46;
		graphic->clearBuffer();
		if (soundOn)
			PlaySound(TEXT("gameOver.wav"), NULL, SND_FILENAME | SND_ASYNC);
		GameMenu* gameoverTitle = new Button("gameoverTitle");
		GameMenu* inspirationalText = new Button("inspirationalText");
		GameMenu* scoreThingi = new Button("score");
		GameMenu* backButton = new Button("back");
		vector<wstring> scoreCounter;
		toVwstring(score, scoreCounter);
		int length = (17 - (6 + scoreCounter[0].length())) / 2;
		graphic->setBuffer(graphic->getBuffer(gameoverTitle->getBufferKey()), left, top, 7, BG);
		graphic->setBufferWhite(graphic->getBuffer(scoreThingi->getBufferKey()), left + 34 + length, top + 6, 7, BG);
		graphic->setBufferWhite(scoreCounter, left + 34 + length + 6, top + 6, 7, BG);
		graphic->setBuffer(graphic->getBuffer(inspirationalText->getBufferKey()), left + 12, top + 20, BG, 7);
		graphic->setBuffer(graphic->getBuffer(backButton->getBufferKey()), left + 21, top + 22, 7, BG);

		graphic->createFrame(0, 0, 145, 40);
		graphic->render();
		int count = 0;
		while (!GetAsyncKeyState(VK_RETURN))
		{
			delay(1000 / (FRAMERATE / 8));
			if (GetAsyncKeyState('W') || GetAsyncKeyState('S'))
				count++;
			if (count == 10)
			{
				GameMenu* easterEgg2 = new Button("easterEgg2");
				graphic->setBuffer(graphic->getBuffer(easterEgg2->getBufferKey()), left + 11, top + 24, BG, 7);
				graphic->render();
			}
		}
		if (soundOn)
			mciSendString(enter, NULL, 0, NULL);
	}

	bool checkCollision(vector<GameLane*> lanes)
	{
		BOUNDINGBOX pla = player->getBoundingBox();
		int lane = pla.y / LANE_HEIGHT;
		if (lane - 1 < 0 || lane > lanes.size())
			return false;
		GameLane* tmp = lanes[lane - 1];
		if (tmp->checkCollision(pla))
		{
			return true;
		}
		return false;
	}
};