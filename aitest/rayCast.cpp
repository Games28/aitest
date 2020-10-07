#include "rayCast.h"

float rayCast::normalizeAngle(float angle)
{
	angle = remainder(angle, TWO_PI);
	if (angle < 0) {
		angle = TWO_PI + angle;
	}
	return angle;
}

float rayCast::distanceBetweenPoints(float x1, float y1, float x2, float y2)
{
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void rayCast::castRay(float rayAngle, int stripId, Character & player)
{
	rayAngle = normalizeAngle(rayAngle);

	int isRayFacingDown = rayAngle > 0 && rayAngle < PI;
	int isRayFacingUp = !isRayFacingDown;

	int isRayFacingRight = rayAngle < 0.5 * PI || rayAngle > 1.5 * PI;
	int isRayFacingLeft = !isRayFacingRight;

	float xintercept, yintercept;
	float xstep, ystep;

	///////////////////////////////////////////
	// HORIZONTAL RAY-GRID INTERSECTION CODE
	///////////////////////////////////////////
	int foundHorzWallHit = FALSE;
	float horzWallHitX = 0;
	float horzWallHitY = 0;
	int horzWallContent = 0;

	// Find the y-coordinate of the closest horizontal grid intersection
	yintercept = floor(player.y / TILE_SIZE) * TILE_SIZE;
	yintercept += isRayFacingDown ? TILE_SIZE : 0;

	// Find the x-coordinate of the closest horizontal grid intersection
	xintercept = player.x + (yintercept - player.y) / tan(rayAngle);

	// Calculate the increment xstep and ystep
	ystep = TILE_SIZE;
	ystep *= isRayFacingUp ? -1 : 1;

	xstep = TILE_SIZE / tan(rayAngle);
	xstep *= (isRayFacingLeft && xstep > 0) ? -1 : 1;
	xstep *= (isRayFacingRight && xstep < 0) ? -1 : 1;

	float nextHorzTouchX = xintercept;
	float nextHorzTouchY = yintercept;

	// Increment xstep and ystep until we find a wall
	while (nextHorzTouchX >= 0 && nextHorzTouchX <= WINDOW_WIDTH && nextHorzTouchY >= 0 && nextHorzTouchY <= WINDOW_HEIGHT) {
		float xToCheck = nextHorzTouchX;
		float yToCheck = nextHorzTouchY + (isRayFacingUp ? -1 : 0);
		//test

		if (map.mapHasWallAt(xToCheck, yToCheck)) {
			// found a wall hit
			horzWallHitX = nextHorzTouchX;
			horzWallHitY = nextHorzTouchY;
			horzWallContent = map.map[(int)floor(yToCheck / TILE_SIZE)][(int)floor(xToCheck / TILE_SIZE)];
			foundHorzWallHit = TRUE;
			break;
		}
		else {
			nextHorzTouchX += xstep;
			nextHorzTouchY += ystep;
		}
	}

	///////////////////////////////////////////
	// VERTICAL RAY-GRID INTERSECTION CODE
	///////////////////////////////////////////
	int foundVertWallHit = FALSE;
	float vertWallHitX = 0;
	float vertWallHitY = 0;
	int vertWallContent = 0;

	// Find the x-coordinate of the closest horizontal grid intersection
	xintercept = floor(player.x / TILE_SIZE) * TILE_SIZE;
	xintercept += isRayFacingRight ? TILE_SIZE : 0;

	// Find the y-coordinate of the closest horizontal grid intersection
	yintercept = player.y + (xintercept - player.x) * tan(rayAngle);

	// Calculate the increment xstep and ystep
	xstep = TILE_SIZE;
	xstep *= isRayFacingLeft ? -1 : 1;

	ystep = TILE_SIZE * tan(rayAngle);
	ystep *= (isRayFacingUp && ystep > 0) ? -1 : 1;
	ystep *= (isRayFacingDown && ystep < 0) ? -1 : 1;

	float nextVertTouchX = xintercept;
	float nextVertTouchY = yintercept;

	// Increment xstep and ystep until we find a wall
	while (nextVertTouchX >= 0 && nextVertTouchX <= WINDOW_WIDTH && nextVertTouchY >= 0 && nextVertTouchY <= WINDOW_HEIGHT) {
		float xToCheck = nextVertTouchX + (isRayFacingLeft ? -1 : 0);
		float yToCheck = nextVertTouchY;

		if (map.mapHasWallAt(xToCheck, yToCheck)) {
			// found a wall hit
			vertWallHitX = nextVertTouchX;
			vertWallHitY = nextVertTouchY;
			vertWallContent = map.map[(int)floor(yToCheck / TILE_SIZE)][(int)floor(xToCheck / TILE_SIZE)];
			foundVertWallHit = TRUE;
			break;
		}
		else {
			nextVertTouchX += xstep;
			nextVertTouchY += ystep;
		}
	}

	// Calculate both horizontal and vertical hit distances and choose the smallest one
	float horzHitDistance = foundHorzWallHit
		? distanceBetweenPoints(player.x, player.y, horzWallHitX, horzWallHitY)
		: FLT_MAX;
	float vertHitDistance = foundVertWallHit
		? distanceBetweenPoints(player.x, player.y, vertWallHitX, vertWallHitY)
		: FLT_MAX;

	if (vertHitDistance < horzHitDistance) {
		distance = vertHitDistance;
		wallHitX = vertWallHitX;
		wallHitY = vertWallHitY;
		wallHitContent = vertWallContent;
		wasHitVertical = TRUE;
	}
	else {
		distance = horzHitDistance;
		wallHitX = horzWallHitX;
		wallHitY = horzWallHitY;
		wallHitContent = horzWallContent;
		wasHitVertical = FALSE;
	}
	rayAngle = rayAngle;
	isRayFacingDown = isRayFacingDown;
	isRayFacingUp = isRayFacingUp;
	isRayFacingLeft = isRayFacingLeft;
	isRayFacingRight = isRayFacingRight;
}


//track function is supposed to find the players position while rotating around in prespetive of looking.
void rayCast::track(float rayAngle, int stripId, Character& player1, Character& player2)
{
	

		rayAngle = normalizeAngle(rayAngle);

		int isRayFacingDown = rayAngle > 0 && rayAngle < PI;
		int isRayFacingUp = !isRayFacingDown;

		int isRayFacingRight = rayAngle < 0.5 * PI || rayAngle > 1.5 * PI;
		int isRayFacingLeft = !isRayFacingRight;

		float xintercept, yintercept;
		float xstep, ystep;

		///////////////////////////////////////////
		// HORIZONTAL RAY-GRID INTERSECTION CODE
		///////////////////////////////////////////
		int foundHorzWallHit = FALSE;
		float horzWallHitX = 0;
		float horzWallHitY = 0;
		int horzWallContent = 0;

		// Find the y-coordinate of the closest horizontal grid intersection
		yintercept = floor(player1.y / TILE_SIZE) * TILE_SIZE;
		yintercept += isRayFacingDown ? TILE_SIZE : 0;

		// Find the x-coordinate of the closest horizontal grid intersection
		xintercept = player1.x + (yintercept - player1.y) / tan(rayAngle);

		// Calculate the increment xstep and ystep
		ystep = TILE_SIZE;
		ystep *= isRayFacingUp ? -1 : 1;

		xstep = TILE_SIZE / tan(rayAngle);
		xstep *= (isRayFacingLeft && xstep > 0) ? -1 : 1;
		xstep *= (isRayFacingRight && xstep < 0) ? -1 : 1;

		float nextHorzTouchX = xintercept;
		float nextHorzTouchY = yintercept;
		float tempx = player1.x;
		float tempy = player1.y;

		// Increment xstep and ystep until we find a wall
		while (nextHorzTouchX >= 0 && nextHorzTouchX <= WINDOW_WIDTH && nextHorzTouchY >= 0 && nextHorzTouchY <= WINDOW_HEIGHT) {
			float xToCheck = nextHorzTouchX;
			float yToCheck = nextHorzTouchY + (isRayFacingUp ? -1 : 0);
			float startx = tempx;
			float starty = tempy;
			float endx = xToCheck;
			float endy = yToCheck;
			//Scan(startx, starty, player2, endx, endy);



			if (map.mapHasWallAt(xToCheck, yToCheck)) {
				// found a wall hit
				horzWallHitX = nextHorzTouchX;
				horzWallHitY = nextHorzTouchY;
				horzWallContent = map.map[(int)floor(yToCheck / TILE_SIZE)][(int)floor(xToCheck / TILE_SIZE)];
				foundHorzWallHit = TRUE;
				Scan(player1.x, player1.y, player2, horzWallHitX, horzWallHitY);
				if (found == true)
				{
					break;
				}
				break;
			}
			else {
				startx = nextHorzTouchX;
				starty = nextHorzTouchY;
				nextHorzTouchX += xstep;
				nextHorzTouchY += ystep;

			}
		}

		///////////////////////////////////////////
		// VERTICAL RAY-GRID INTERSECTION CODE
		///////////////////////////////////////////
		int foundVertWallHit = FALSE;
		float vertWallHitX = 0;
		float vertWallHitY = 0;
		int vertWallContent = 0;

		// Find the x-coordinate of the closest horizontal grid intersection
		xintercept = floor(player1.x / TILE_SIZE) * TILE_SIZE;
		xintercept += isRayFacingRight ? TILE_SIZE : 0;

		// Find the y-coordinate of the closest horizontal grid intersection
		yintercept = player1.y + (xintercept - player1.x) * tan(rayAngle);

		// Calculate the increment xstep and ystep
		xstep = TILE_SIZE;
		xstep *= isRayFacingLeft ? -1 : 1;

		ystep = TILE_SIZE * tan(rayAngle);
		ystep *= (isRayFacingUp && ystep > 0) ? -1 : 1;
		ystep *= (isRayFacingDown && ystep < 0) ? -1 : 1;

		float nextVertTouchX = xintercept;
		float nextVertTouchY = yintercept;


		// Increment xstep and ystep until we find a wall
		while (nextVertTouchX >= 0 && nextVertTouchX <= WINDOW_WIDTH && nextVertTouchY >= 0 && nextVertTouchY <= WINDOW_HEIGHT) {
			float xToCheck = nextVertTouchX + (isRayFacingLeft ? -1 : 0);
			float yToCheck = nextVertTouchY;
			float startx = tempx;
			float starty = tempy;
			float endx = xToCheck;
			float endy = yToCheck;
			//Scan(startx, starty, player2, endx, endy);



			if (map.mapHasWallAt(xToCheck, yToCheck)) {
				// found a wall hit
				vertWallHitX = nextVertTouchX;
				vertWallHitY = nextVertTouchY;
				vertWallContent = map.map[(int)floor(yToCheck / TILE_SIZE)][(int)floor(xToCheck / TILE_SIZE)];
				foundVertWallHit = TRUE;
				Scan(player1.x, player1.y, player2, vertWallHitX, vertWallHitY);
				if (found == true)
				{
					break;
				}
				break;
			}
			else {
				startx = nextVertTouchX;
				starty = nextVertTouchY;
				nextVertTouchX += xstep;
				nextVertTouchY += ystep;

			}
		}

	
}

//supposed looks at the area between the beginning of the ray aka the enemy character and when the wallit hit.
//to see if the player characters position is between both parts pre ray.

void rayCast::Scan(float point1x, float point1y, Character& player, float point2x, float point2y)
{
	//looking up
	if (point1y > player.y && point2y < player.y)
	{
		found = true;
	}
	//looking down
	if (point1y < player.y && point2y > player.y)
	{
		found = true;
	}
	//looking left
	if (point1x > player.x && player.x > point2x)
	{
		found = true;
	}
	//looking right
	if (point1x < player.x && player.x < point2x)
	{
		found = true;
	}
	if (player.y > point2y)
	{
		found = false;
	}
	if (player.x > point2x)
	{
		found = false;
	}
}


