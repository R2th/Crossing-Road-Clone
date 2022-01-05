#ifndef _POSITION_H_
#define _POSITION_H_

class Position 
{
private:
	int x;
	int y;
public:
	int getX(); 
	int getY();
	void setX(int a);
	void setY(int b);
	Position() : x(0), y(0) {};
	Position(int a, int b) : x(a), y(b) {};
	~Position() {};
};


#endif _POSITION_H_
