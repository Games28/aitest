#pragma once
#include "constants.h"
#include <math.h>
#include "SDL.h"
#include "Map.h"
class Character
{
public:
	Character() = default;
	void movePlayer(float deltaTime);
	void renderPlayer(SDL_Renderer* render);

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
	Map map;
};

