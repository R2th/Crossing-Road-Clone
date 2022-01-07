#ifndef _TRUCK_H_
#define _TRUCK_H_

#include "Obstacle.h"

class Truck : public Obstacle
{
	char** a;
public:
	char** shape();
	void sound();
	Truck();
	~Truck();
	void drawC();
	void drawtobuffer(Buffer&);
};

#endif _TRUCK_H_
