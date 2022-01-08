#ifndef _CAR_H_
#define _CAR_H_

#include "Obstacle.h"

class Car : public Obstacle
{
	char** a;
public:
	char** shape();
	void sound();
	Car();
	~Car();
	void drawC();
	void drawtobuffer(Buffer&);
};

#endif _CAR_H_
