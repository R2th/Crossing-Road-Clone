#pragma once
#include <vector>
#include <string>
#include "GameObject.h"
#include "Utils.h"
using namespace std;

class Obstacles : public GameObject {
private:
	int speed = 0, tick = 0, bgColor, fgColor;
protected:
	int autoX = 1, autoY = 0;
public:
	Obstacles() : GameObject(0, 0, 0, 0, "unknownObstacles") {}
	Obstacles(int x, int y, int _speed, int _bgColor, int _fgColor, string bufferKey, GraphicsController*& graphic) : 
		GameObject(x, y, graphic->getBuffer(bufferKey)[0].size(), graphic->getBuffer(bufferKey).size(), bufferKey), 
		speed(_speed),
		bgColor(_bgColor),
		fgColor(_fgColor)
	{}

	void move(int x, int y)
	{
		tick++;

		// Check if enough ticks yet
		if (tick < speed)
			return; //Nah, not enough
		
		tick %= speed;
		this->oldX = this->x;
		this->oldY = this->y;
		this->x += x;
		this->y += y;

		if (isOutOfBound())
			resetPos(x > 0);
	}
	virtual void resetPos(bool left = true) {
		this->x = left? -MAX_ENEMY_WIDTH + 1 : screenWidth;
	}

	void render(GraphicsController*& graphic, int offset) {
		//clearOldPos(graphic);
		graphic->setBufferObject(graphic->getBuffer(bufferKey), this->x, this->y + offset, bgColor, fgColor);
	}
	void render(GraphicsController*& graphic, int offset, int bg) {
		//clearOldPos(graphic);
		graphic->setBufferObject(graphic->getBuffer(bufferKey), this->x, this->y + offset, bg, fgColor);
	}
	void render(GraphicsController*& graphic, int offset, int bg, int ch) {
		//clearOldPos(graphic);
		graphic->setBufferObject(graphic->getBuffer(bufferKey), this->x, this->y + offset, bg, ch);
	}
	void clearOldPos(GraphicsController*& graphic) {
		GameObject::clearOldPos(graphic, bgColor, fgColor);
	}

	void setPos(int x, int y) {
		this->x = x;
		this->oldX = x;
		this->y = y;
		this->oldY = y;
	}

	bool isOutOfBound() {
		return x + MAX_ENEMY_WIDTH < 1 || x > screenWidth;
	}

	int getTick() {
		return tick;
	}
};

//fell free to declare more class using the above template
//feel*