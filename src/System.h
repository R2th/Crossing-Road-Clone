#pragma once
#pragma comment(lib, "winmm.lib")
#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "Game.h"
#include <cstring>
#include <string>
#include <conio.h>
#include <mmsystem.h>
class System {
private:
	Game currentGame;
	bool IsSound;
	int DifficultLevel;
public:
	System();
	~System();
	void StartGame();
	void HandleInputWhilePlay();
	void ShowMenuBegin();
	bool ShowMenuWhilePlay();
	bool LoadGame();
	bool SaveGame();
	void Setting(bool IsPlay);
	void PlayGameSound();
	void StopGameSound();
	void MakeColor();
};
#endif // !SYSTEM_H_

