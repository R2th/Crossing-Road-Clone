#include "people.h"

void People::LoadGame(istream &is, Screen &sc) {
	is.read((char *)&x, 4);
	is.read((char *)&y, 4);
	is.read((char*)&isDead, 1);
	is.read((char *)&isCompleted, 1);
	sc.Movepeople(-1, -1, x, y);
}
void People::SaveGame(ostream &os) {
	os.write((char *)&x, 4);
	os.write((char *)&y, 4);
	os.write((char*)&isDead, 1);
	os.write((char *)&isCompleted, 1);
}
People::People()
{
	this->x = HEIGHT - 1;
	this->y = LENGTH / 2;
}
//---------------------------------
People::People(int x, int y)
{
	this->x = x;
	this->y = y;
}
//---------------------------------
void People::Reset(Screen &screen)
{
	int OldX = this->x, OldY = this->y;
	this->x = 18;
	this->y = 29;
	this->isCompleted = false;
	this->isDead = false;

	screen.Movepeople(OldX, OldY, this->x, this->y);
}
//---------------------------------
void People::Up(int k, Screen &screen)
{
	int OldX = this->x, OldY = this->y;

	this->x = this->x - k;
	if (this->x < 0)
	{
		this->isCompleted = true;
		this->x = HEIGHT - 1;
	}

	screen.Movepeople(OldX, OldY, this->x, this->y);
}
//---------------------------------
void People::Down(int k, Screen &screen)
{
	int OldX = this->x, OldY = this->y;

	this->x = this->x + k;
	if (this->x > HEIGHT - 2)
	{
		this->x = HEIGHT - 2;
	}

	screen.Movepeople(OldX, OldY, this->x, this->y);
}
//---------------------------------
void People::Left(int k, Screen &screen)
{
	int OldX = this->x, OldY = this->y;

	this->y = this->y - k;
	if (this->y < 0)
	{
		this->y = 0;
	}

	screen.Movepeople(OldX, OldY, this->x, this->y);
}
//---------------------------------
void People::Right(int k, Screen &screen)
{
	int OldX = this->x, OldY = this->y;

	this->y = this->y + k;
	if (this->y > LENGTH - 3)
	{
		this->y = LENGTH - 3;
	}

	screen.Movepeople(OldX, OldY, this->x, this->y);
}
//---------------------------------
bool People::isCompletedLevel()
{
	return this->isCompleted;
}
//---------------------------------
bool People::isImpact(Animal *animal)
{
	return animal->IsImpact(this->x, this->y);
}
//---------------------------------
bool People::isImpact(VEHICLE *vehicle)
{
	return vehicle->IsImpact(this->x, this->y);
}
//---------------------------------
ostream& operator << (ostream &fout, const People &player)
{
	fout << player.x << " " << player.y;
	return fout;
}