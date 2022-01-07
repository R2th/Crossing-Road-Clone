#pragma once
#include <vector>
#include <string>
#include "GameObject.h"
using namespace std;

class Player : public GameObject
{
protected:
public:
	Player() : GameObject(0, 0, 0, 0, "player") {}
	Player(GraphicsController *&graphic) : GameObject(0, 0, graphic->getBuffer("player")[0].size(), graphic->getBuffer("player").size(), "player") {}
	Player(int x, int y, GraphicsController *&graphic) : GameObject(x, y, graphic->getBuffer("player")[0].size(), graphic->getBuffer("player").size(), "player") {}
	void update()
	{
		this->oldX = this->x;
		this->oldY = this->y;
	}

	void move(int x, int y)
	{
		this->x += 2 * x;
		this->y += y;
	}

	void render(GraphicsController *&graphic, int offset)
	{
		// clearOldPos(graphic);
		graphic->setBufferWhite(graphic->getBuffer(bufferKey), oldX, oldY + offset, BG, 1);
		graphic->setBufferWhite(graphic->getBuffer(bufferKey), x, y + offset, BG, 7);
	}
	void render(GraphicsController *&graphic, int offset, int bg, int ch)
	{
		// clearOldPos(graphic);
		graphic->setBufferWhite(graphic->getBuffer(bufferKey), oldX, oldY + offset, bg, blueLight);
		graphic->setBufferWhite(graphic->getBuffer(bufferKey), x, y + offset, bg, ch);
	}

	void clearOldPos(GraphicsController *&graphic)
	{
		GameObject::clearOldPos(graphic, BG, 7);
	}

	void setPos(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
};