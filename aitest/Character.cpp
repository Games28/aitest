#include "Character.h"
#include <math.h>
#include "rayCast.h"



void Character::movePlayer(float deltaTime)
{
	rotationAngle += turnDirection * turnSpeed * deltaTime;
	float moveStep = walkDirection * walkSpeed * deltaTime;

	float newPlayerX = x + cos(rotationAngle) * moveStep;
	float newPlayerY = y + sin(rotationAngle) * moveStep;

	//wall collision
	if (!map.mapHasWallAt(newPlayerX, newPlayerY)) {
		x = newPlayerX;
		y = newPlayerY;
	}
}

void Character::enemymove(float deltaTime)
{
	rotationAngle += turnDirection * turnSpeed * deltaTime;
	float moveStep = walkDirection * walkSpeed * deltaTime;

	float newPlayerX = x + cos(rotationAngle) * moveStep;
	float newPlayerY = y + sin(rotationAngle) * moveStep;

	//wall collision
	if (!map.mapHasWallAt(newPlayerX, newPlayerY)) {
		x = newPlayerX;
		y = newPlayerY;
	}
	
}


