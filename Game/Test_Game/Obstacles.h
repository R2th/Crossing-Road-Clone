#pragma once
#include <vector>
#include <string>
#include "GameObject.h"
#include "Utils.h"
using namespace std;

class Obstacles : public GameObject
{
private:
	int speed = 0, tick = 0, bgColor, fgColor;

protected:
	int autoX = 1, autoY = 0;

public:
	Obstacles() : GameObject(0, 0, 0, 0, "unknownObstacles") {}
	Obstacles(int x, int y, int _speed, int _bgColor, int _fgColor, string bufferKey, GraphicsController *&graphic) : GameObject(x, y, graphic->getBuffer(bufferKey)[0].size(), graphic->getBuffer(bufferKey).size(), bufferKey),
																													  speed(_speed),
																													  bgColor(_bgColor),
																													  fgColor(_fgColor)
	{
	}
};

// fell free to declare more class using the above template
// feel*