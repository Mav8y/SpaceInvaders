#pragma once
#include <raylib.h>
#include "Laser.h"
#include <vector>
class Spaceship
{
public:
	Spaceship();
	~Spaceship();
	void Draw();
	void MoveRight();
	void MoveLeft();
	void FireLaser();
	Rectangle getRect();
	void Reset();
	std::vector<Laser> lasers;
private:
	Texture2D image;
	Vector2 position;
	double LastFireTime;
	Sound laserSound;
};