#ifndef _BIRD_H_
#define _BIRD_H_

#include "Obstacle.h"

class Bird : public Obstacle
{
	char** a;
public:
	char** shape();
	void sound();
	Bird();
	~Bird();
	void drawC();
	void drawtobuffer(Buffer&);
};

#endif _BIRD_H_
