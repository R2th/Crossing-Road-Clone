#include "Obstacle.h"

int Obstacle::getX()
{
	return p.getX();
}

int Obstacle::getY()
{
	return p.getY();
}

Obstacle::Obstacle()
{
	p.setX(0);
	p.setY(0);
	outMap = false;
}

Obstacle::Obstacle(Position o)
{
	p.setX(o.getX());
	p.setY(o.getY());
	outMap = false;
}

void Obstacle::setPositionO(int a, int b)
{
	p.setX(a);
	p.setY(b);
	outMap = false;
}

bool Obstacle::isOutofMap()
{
	return outMap;
}

void Obstacle::OutMap()
{
	outMap = true;
}

int Obstacle::height()
{
	return 3;
}

int Obstacle::width()
{
	return 5;
}

void Obstacle::deleteObstacle()
{
	for (int i = p.getY(); i < i + 3; i++)
	{
		for (int j = p.getX(); j < j + 5; j++)
		{
			gotoxy(j, i);
			cout << " ";
		}
	}
}
void Obstacle::deleteObstaclebuffer()
{
	for (int i = p.getY(); i < i + 3; i++)
	{
		for (int j = p.getX(); j < j + 5; j++)
		{
			curFrame[i][j] = ' ';
		}
	}
}
