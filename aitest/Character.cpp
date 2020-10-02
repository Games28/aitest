#include "Character.h"

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

void Character::renderPlayer(SDL_Renderer* render)
{
	SDL_SetRenderDrawColor(render, 0, 255, 0, 255);
	SDL_Rect playerRect = {
		x,
		y,
		width,
		height
	};

	SDL_RenderFillRect(render, &playerRect);
	SDL_RenderDrawLine(
		render,
		x,
		y,
		x + cos(rotationAngle),
		y + sin(rotationAngle)
	);
}
