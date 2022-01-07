#pragma once
#include <vector>
#include <string>
#include "Utils.h"

using namespace std;

class Items {
protected:
	string bufferKey;
public:
	Items() : bufferKey("none") {}

	Items(string bufferKey) : bufferKey(bufferKey) {}

	string getBufferKey() 
	{
		return bufferKey;
	}

	virtual coord getPos() = 0;
};