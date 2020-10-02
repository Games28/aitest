#pragma once
#include "constants.h"
#include <math.h>
#include "Map.h"
#include "Character.h"
#include "SDL.h"
#include "constants.h"
class Ray
{
public:
	Ray() = default;
	float normalizeAngle(float angle);
	float distanceBetweenPoints(float x1, float y1, float x2, float y2);
	void castRay(float rayAngle, int stripId, Character& character);
	void castAllRays(Character& character);
	void renderRays(Character& character,SDL_Renderer* render);
public:
	float rayangle;
	float wallHitX;
	float wallHitY;
	float distance;
	int wasHitVertical;
	int isRayFacingUp;
	int isRayFacingDown;
	int isRayFacingLeft;
	int isRayFacingRight;
	int wallHitContent;
	Map map;
	
};

