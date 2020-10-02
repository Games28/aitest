#include "Ray.h"
#include <limits.h>
#include <float.h>


float Ray::normalizeAngle(float angle)
{
	angle = remainder(angle, TWO_PI);
	if (angle < 0) {
		angle = TWO_PI + angle;
	}
	return angle;
}

float Ray::distanceBetweenPoints(float x1, float y1, float x2, float y2)
{
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void Ray::castRay(float rayAngle, int stripId, Character& character)
{
	rayangle = normalizeAngle(rayAngle);

	int isRayFacingDown = rayangle > 0 && rayangle < PI;
	int isRayFacingUp = !isRayFacingDown;

	int isRayFacingRight = rayangle < 0.5 * PI || rayangle > 1.5 * PI;
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
	yintercept = floor(character.y / DIMENSION) * DIMENSION;
	yintercept += isRayFacingDown ? DIMENSION : 0;

	// Find the x-coordinate of the closest horizontal grid intersection
	xintercept = character.x + (yintercept - character.y) / tan(rayAngle);

	// Calculate the increment xstep and ystep
	ystep = DIMENSION;
	ystep *= isRayFacingUp ? -1 : 1;

	xstep = DIMENSION / tan(rayangle);
	xstep *= (isRayFacingLeft && xstep > 0) ? -1 : 1;
	xstep *= (isRayFacingRight && xstep < 0) ? -1 : 1;

	float nextHorzTouchX = xintercept;
	float nextHorzTouchY = yintercept;

	// Increment xstep and ystep until we find a wall
	while (nextHorzTouchX >= 0 && nextHorzTouchX <= WINDOW_WIDTH && nextHorzTouchY >= 0 && nextHorzTouchY <= WINDOW_HEIGHT) {
		float xToCheck = nextHorzTouchX;
		float yToCheck = nextHorzTouchY + (isRayFacingUp ? -1 : 0);

		if (map.mapHasWallAt(xToCheck, yToCheck)) {
			// found a wall hit
			horzWallHitX = nextHorzTouchX;
			horzWallHitY = nextHorzTouchY;
			horzWallContent = map.map[(int)floor(yToCheck / DIMENSION)][(int)floor(xToCheck / DIMENSION)];
			foundHorzWallHit = FALSE;
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
	xintercept = floor(character.x / DIMENSION) * DIMENSION;
	xintercept += isRayFacingRight ? DIMENSION : 0;

	// Find the y-coordinate of the closest horizontal grid intersection
	yintercept = character.y + (xintercept - character.x) * tan(rayangle);

	// Calculate the increment xstep and ystep
	xstep = DIMENSION;
	xstep *= isRayFacingLeft ? -1 : 1;

	ystep = DIMENSION * tan(rayangle);
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
			vertWallContent = map.map[(int)floor(yToCheck / DIMENSION)][(int)floor(xToCheck / DIMENSION)];
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
		? distanceBetweenPoints(character.x, character.y, horzWallHitX, horzWallHitY)
		: FLT_MAX;
	float vertHitDistance = foundVertWallHit
		? distanceBetweenPoints(character.x, character.y, vertWallHitX, vertWallHitY)
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
	//rayangle = rayAngle;
	//isRayFacingDown = isRayFacingDown;
	//isRayFacingUp = isRayFacingUp;
	//isRayFacingLeft = isRayFacingLeft;
	//isRayFacingRight = isRayFacingRight;

}

void Ray::castAllRays(Character& character)
{
	float rayAngle = character.rotationAngle - (FOV_ANGLE / 2);

	for (int stripId = 0; stripId < NUM_RAYS; stripId++) {
		castRay(rayAngle, stripId, character);

		rayAngle += FOV_ANGLE / NUM_RAYS;
	}
}

void Ray::renderRays(Character& character, SDL_Renderer* render)
{
	SDL_SetRenderDrawColor(render, 255, 0, 255, 255);
	for (int i = 0; i < NUM_RAYS; i++) {
		SDL_RenderDrawLine(
			render,
			 character.x,
			 character.y,
			 wallHitX,
			 wallHitY
		);

	}
}
