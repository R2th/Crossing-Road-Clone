#include "Player.h"

void Player::move(int x, int y)
{
	oldX = x;
	oldY = y;
}

void Player::render(GraphicsController*& graphic, int offset)
{
	graphic->setBufferWhite(graphic->getBuffer(bufferKey), oldX, oldY + offset, BG, 1);
	graphic->setBufferWhite(graphic->getBuffer(bufferKey), x, y + offset, BG, 7);

	graphic->setBuffer(graphic->getBuffer(bufferKey), oldX, oldY + offset, BG, 1);
	graphic->setBuffer(graphic->getBuffer(bufferKey), x, y + offset, BG, 7);
}

void Player::render(GraphicsController*& graphic, int offset, int bg, int ch)
{
	graphic->setBufferWhite(graphic->getBuffer(bufferKey), oldX, oldY + offset, bg, 1);
	graphic->setBufferWhite(graphic->getBuffer(bufferKey), x, y + offset, bg, ch);

	graphic->setBuffer(graphic->getBuffer(bufferKey), oldX, oldY + offset, bg, blueLight);
	graphic->setBuffer(graphic->getBuffer(bufferKey), x, y + offset, bg, ch);
}

void Player::clearOldPos(GraphicsController*& graphic)
{
	for (int i = y; i < h + y; ++i)
		for (int j = x; j < w + x; ++j)
			graphic->getBuffer("player")[i][j] = ' ';
}

void Player::setPos(int x, int y)
{
	oldX = x;
	oldY = y;
}