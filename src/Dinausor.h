#ifndef _DINAUSOR_H_
#define _DINAUSOR_H_

#include "Obstacle.h"

class Dinausor : public Obstacle
{
	char** a;
public:
	char** shape();
	void sound();
	Dinausor();
	~Dinausor();
	void drawD();
	void drawtobuffer(Buffer&);
};

#endif _DINAUSOR_H_
