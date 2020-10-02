#include <iostream>
#include <SDL.h>
#include "constants.h"
#include "Ray.h"
#include "Character.h"
#include "Map.h"

using namespace std;

//global variables

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int GameRunning = FALSE;
int ticksLastFrame = 0;
Ray rays[Count];
Ray ray[NUM_RAYS];
Character characters[Count];
Character player;
Map map;


Uint32* colorBuffer = NULL;
SDL_Texture* colorBufferTexture;

Uint32* wallTexture = NULL;


bool initializedWindow()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << stderr << "error initializing sdl." << std::endl;
		return false;
	}

	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_BORDERLESS
	);

	if (!window)
	{
		std::cout << stderr << "error creating std window." << std::endl;
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer)
	{
		std::cout << stderr << "error creating std window." << std::endl;
		return false;
	}
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	return true;
}

void destroyWindow()
{
	free(colorBuffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void movePlayer(float deltaTime)
{
	player.rotationAngle += player.turnDirection * player.turnSpeed * deltaTime;
	float moveStep = player.walkDirection * player.walkSpeed * deltaTime;

	float newPlayerX = player.x + cos(player.rotationAngle) * moveStep;
	float newPlayerY = player.y + sin(player.rotationAngle) * moveStep;

	//wall collision
	if (!map.mapHasWallAt(newPlayerX, newPlayerY)) {
		player.x = newPlayerX;
		player.y = newPlayerY;
	}
}

void setup()
{
	/*//player
	characters[0].x = WINDOW_WIDTH / 2;
	characters[0].y = WINDOW_HEIGHT / 2;
	characters[0].width = 1;
	characters[0].height = 1;
	characters[0].turnDirection = 0;
	characters[0].walkDirection = 0;
	characters[0].rotationAngle = PI / 2;
	characters[0].walkSpeed = 100;
	characters[0].turnSpeed = 45 * (PI / 180);
	characters[0].lookUpDown = 0;
	characters[0].playerLookAngle = 45 * (PI / 180);
	characters[0].lookUpDown = 0;

	//enemy
	characters[1].x = WINDOW_WIDTH / 4;
	characters[1].y = WINDOW_HEIGHT / 4;
	characters[1].width = 1;
	characters[1].height = 1;
	characters[1].turnDirection = 0;
	characters[1].walkDirection = 0;
	characters[1].rotationAngle = PI / 2;
	characters[1].walkSpeed = 100;
	characters[1].turnSpeed = 45 * (PI / 180);
	characters[1].lookUpDown = 0;
	characters[1].playerLookAngle = 45 * (PI / 180);
	characters[1].lookUpDown = 0;*/

	//test
	player.x = WINDOW_WIDTH / 2;
	player.y = WINDOW_HEIGHT / 2;
	player.width = 1;
	player.height = 1;
	player.turnDirection = 0;
	player.walkDirection = 0;
	player.rotationAngle = PI / 2;
	player.walkSpeed = 100;
	player.turnSpeed = 45 * (PI / 180);
	player.lookUpDown = 0;
	player.playerLookAngle = 45 * (PI / 180);
	player.lookUpDown = 0;
	

	colorBuffer = (Uint32*)malloc(sizeof(Uint32) * (Uint32)WINDOW_WIDTH * (Uint32)WINDOW_HEIGHT);

	colorBufferTexture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ABGR8888,
		SDL_TEXTUREACCESS_STREAMING,
		WINDOW_WIDTH,
		WINDOW_HEIGHT
	);

}

void castAllRays(Character& character)
{
	float rayAngle = character.rotationAngle - (FOV_ANGLE / 2);

	for (int stripId = 0; stripId < NUM_RAYS; stripId++) {
		ray[stripId].castRay(rayAngle, stripId, character);

		rayAngle += FOV_ANGLE / NUM_RAYS;
	}
}

void processInput()
{
	SDL_Event event;

	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
	{
		GameRunning = false;
		break;
	}
	case SDL_KEYDOWN: {
		if (event.key.keysym.sym == SDLK_ESCAPE)
			GameRunning = false;
		if (event.key.keysym.sym == SDLK_UP)
			player.walkDirection = +1;
		if (event.key.keysym.sym == SDLK_DOWN)
			player.walkDirection = -1;
		if (event.key.keysym.sym == SDLK_RIGHT)
			player.turnDirection = +1;
		if (event.key.keysym.sym == SDLK_LEFT)
			player.turnDirection = -1;
		if (event.key.keysym.sym == SDLK_w)

			player.lookUpDown += 4;

		if (event.key.keysym.sym == SDLK_s)

			player.lookUpDown += -4;

		break;
	}
	case SDL_KEYUP: {
		if (event.key.keysym.sym == SDLK_UP)
			player.walkDirection = 0;
		if (event.key.keysym.sym == SDLK_DOWN)
			player.walkDirection = 0;
		if (event.key.keysym.sym == SDLK_RIGHT)
			player.turnDirection = 0;
		if (event.key.keysym.sym == SDLK_LEFT)
			player.turnDirection = 0;
		if (event.key.keysym.sym == SDLK_w)

			player.lookUpDown += player.lookUpDown * 0;
		if (event.key.keysym.sym == SDLK_s)

			player.lookUpDown += player.lookUpDown * 0;

		break;
	}

	}


}

void update()
{
	
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksLastFrame + FRAME_TIME_LENGTH));
	float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;
	ticksLastFrame = SDL_GetTicks();
	/*for (int i = 0; i < Count; i++)
	{
		characters[i].movePlayer(deltaTime);
		rays[i].castAllRays(characters[i]);
	}
	*/
	movePlayer(deltaTime);
	castAllRays(player);

}

void clearColorBuffer(Uint32 color)
{
	for (int x = 0; x < WINDOW_WIDTH; x++) {
		for (int y = 0; y < WINDOW_HEIGHT; y++) {

			colorBuffer[(WINDOW_WIDTH * y) + x] = color;

		}
	}
}

void renderColorBuffer() {
	SDL_UpdateTexture(
		colorBufferTexture,
		NULL,
		colorBuffer,
		(int)((Uint32)WINDOW_WIDTH * sizeof(Uint32))
	);
	SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL);

}

void render()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	renderColorBuffer();
	//clear the color buffer
	clearColorBuffer(0xff000000);
	map.renderMap(renderer);
	/*for (int i = 0; i < Count; i++)
	{
		characters[i].renderPlayer(renderer);
		rays[i].renderRays(characters[i], renderer);
		
	}*/
	player.renderPlayer(renderer);
	for (int i = 0; i < NUM_RAYS; i++)
	{
		ray[i].renderRays(player, renderer);
	}
	SDL_RenderPresent(renderer);
	

}

int main(int argc, char* argv[])
{
	GameRunning = initializedWindow();
	setup();

	while (GameRunning)
	{
		processInput();
		update();
		render();
	}

	destroyWindow();

	return 0;

}