#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

#include <iostream>
#include "Position.h"

class Obstacle
{
private:
	Position p;
	bool outMap;
public:
	int getX();
	int getY();
	Obstacle();
	Obstacle(Position o);
	void setPositionO(int a, int b);
	virtual char** shape() = 0;
	virtual void sound() = 0;
	bool isOutofMap();
	void OutMap();
	int height();
	int width();
	void deleteObstacle();
	void deleteObstaclebuffer();
};

#endif _OBSTACLE_H_
