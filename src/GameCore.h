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
