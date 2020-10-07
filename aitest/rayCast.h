#pragma once
#include "Map.h"
#include <math.h>
#include "Character.h"
#include "Constants.h"
#include <float.h>

class rayCast
{
public:
	rayCast() = default;
	float normalizeAngle(float angle);
	float distanceBetweenPoints(float x1, float y1, float x2, float y2);
	void castRay(float rayAngle, int stripId, Character& player);
	void track(float rayAngle, int stripId, Character& player1, Character& player2);
	void Scan(float point1x, float point1y, Character& player, float point2x, float point2y);
	

public:
	float rayAngle;
	float wallHitX;
	float wallHitY;
	float distance;
	int wasHitVertical;
	int isRayFacingUp;
	int isRayFacingDown;
	int isRayFacingLeft;
	int isRayFacingRight;
	int wallHitContent;
	bool found = false;
	Map map;
	
};

