#pragma once
#include "Map.h"
#include <math.h>
class rayCast;
class Character
{
public:
	Character() = default;
	void movePlayer(float deltaTime);
	void enemymove(float deltaTime);
	
	
public:
	float x;
	float y;
	float width;
	float height;
	int turnDirection; // -1 for left, +1 for right
	int walkDirection; // -1 for back, +1 for front
	float rotationAngle;
	float walkSpeed;
	float turnSpeed;
	int islookingupdown;
	float lookUpDown;
	float playerLookAngle;
	bool playerfound = false;
	Map map;
};

