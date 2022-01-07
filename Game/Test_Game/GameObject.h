#pragma once
#include "Items.h"
#include <string>
#include "Utils.h"
#include "GraphicsController.h"

using namespace std;

class GameObject : public Items {
protected:
	int oldX, oldY;
	int x, y; // coordinate of top-left corner
	int w, h; // box size
public:
	GameObject() : oldX(0), oldY(0), x(0), y(0), w(0), h(0), Items() {}
	GameObject(int x, int y, int w, int h, string bufferKey) : Items(bufferKey), oldX(x), oldY(y), x(x), y(y), w(w), h(h) {}

	coord getPos() {
		return { x, y };
	}
	coord getOldPos() {
		return { oldX, oldY };
	}
	BOUNDINGBOX getBoundingBox() {
		return { x,y,w,h };
	}

	virtual void move(int x, int y) = 0;

	void render(GraphicsController*& graphic, int bgColor, int fgColor) {
		graphic->setBufferObject(graphic->getBuffer(bufferKey), x, y, bgColor, fgColor);
	}

	void clearOldPos(GraphicsController*& graphic, int bgColor, int fgColor) {
		graphic->setBufferObject(graphic->getBuffer(bufferKey + "_clear"), oldX, oldY, bgColor, fgColor);
	}

	// WARNING: ONLY BE CALLED IN CONSTRUCTOR OF GAMELANE
	// YOU HAVE BEEN WARNED
	// dont care, imma fuck this up
	virtual void resetPos(int lane, GraphicsController*& graphic, bool left = true) {
		this->x = 0;
		this->y = 0;
	}
};