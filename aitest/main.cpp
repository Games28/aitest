#include "SDL.h"
#include "rayCast.h"
#include "Character.h"
#include "Constants.h"
#include <iostream>


const  int map[MAP_NUM_ROWS][MAP_NUM_COLS] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}

};

//class objects
Character player;
rayCast playerrays[NUM_RAYS];
Character enemy;
rayCast enemyrays[NUM_RAYS];

//variables
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
Uint32* colorBuffer = NULL;
SDL_Texture* colorBufferTexture;
int ticksLastFrame = 0;
int gameRunning = FALSE;


int initializeWindow()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << stderr << "error initializing sdl." << std::endl;
		return FALSE;
	}

	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_INPUT_FOCUS
	);

	if (!window)
	{
		std::cout << stderr << "error creating sdl window." << std::endl;
		return FALSE;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer)
	{
		std::cout << stderr << "error creating renderer." << std::endl;
		return FALSE;
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	return TRUE;
}

void destroyWindow()
{
	free(colorBuffer);
	SDL_DestroyTexture(colorBufferTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void setup()
{
	//player
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
	//enemy
	enemy.x = WINDOW_WIDTH / 4;
	enemy.y = WINDOW_HEIGHT / 2;
	enemy.width = 1;
	enemy.height = 1;
	enemy.turnDirection = 1;
	//enemy.walkDirection = 0;
	enemy.rotationAngle = PI / 2;
	//enemy.walkSpeed = 100;
	enemy.turnSpeed = 45 * (PI / 180);
	//enemy.lookUpDown = 0;
	enemy.playerLookAngle = 45 * (PI / 180);
	//enemy.lookUpDown = 0;
	

	colorBuffer = (Uint32*)malloc(sizeof(Uint32) * (Uint32)WINDOW_WIDTH * (Uint32)WINDOW_HEIGHT);

	colorBufferTexture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		WINDOW_WIDTH,
		WINDOW_HEIGHT
	);

}

int mapHasWallAt(float x, float y) {
	if (x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT) {
		return true;
	}
	int mapGridIndexX = floor(x / TILE_SIZE);
	int mapGridIndexY = floor(y / TILE_SIZE);
	return map[mapGridIndexY][mapGridIndexX] != 0;

}
void castAllRays(Character& p, rayCast r[]) {
	float rayAngle = p.rotationAngle - (FOV_ANGLE / 2);

	for (int stripId = 0; stripId < NUM_RAYS; stripId++) {
		r[stripId].castRay(rayAngle, stripId,p);
		
		
		rayAngle += FOV_ANGLE / NUM_RAYS;
	}

}

//a test ray casing for the searching rays of the enemy. supposed to update if the enemy found the player or not.
void castraytest(Character& p, rayCast r[]) {
	float rayAngle = p.rotationAngle - (FOV_ANGLE / 2);
	
		for (int stripId = 0; stripId < NUM_RAYS; stripId++) {
			r[stripId].track(rayAngle, stripId, p, player);
			enemy.playerfound = r[stripId].found;

			rayAngle += FOV_ANGLE / NUM_RAYS;
		}
	
}


void processInput()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	
	switch (event.type)
	{
	case SDL_QUIT: {
		gameRunning = FALSE;
		break;
	}
	case SDL_KEYDOWN: {
		if (event.key.keysym.sym == SDLK_ESCAPE)
			gameRunning = FALSE;
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
	player.movePlayer(deltaTime);
	enemy.enemymove(deltaTime);
	castAllRays(player,playerrays);
	castAllRays(enemy, enemyrays);
	//the impleement enemy tracking rays
	castraytest(enemy, enemyrays);
	

}

void renderMap() {
	for (int i = 0; i < MAP_NUM_ROWS; i++) {
		for (int j = 0; j < MAP_NUM_COLS; j++) {
			int tileX = j * TILE_SIZE;
			int tileY = i * TILE_SIZE;
			int tileColor = map[i][j] != 0 ? 255 : 0;

			SDL_SetRenderDrawColor(renderer, 0, 0, tileColor, 255);
			SDL_Rect mapTileRect = {
				tileX ,
				tileY ,
				TILE_SIZE ,
				TILE_SIZE 
			};
			SDL_RenderFillRect(renderer, &mapTileRect);
		}
	}

}

void renderPlayer(Character& p) {
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_Rect playerRect = {
		p.x,
		p.y,
		p.width ,
		p.height 
	};
	SDL_RenderFillRect(renderer, &playerRect);

	SDL_RenderDrawLine(
		renderer,
		p.x,
		p.y,
		p.x + cos(p.rotationAngle),
		p.y + sin(p.rotationAngle)
	);
}
// does a test tracking line between the enemy and player resembling the player having the enemies attention.
void targettest()
{
	if (enemy.playerfound == true)
	{
		enemy.turnDirection = 0;
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderDrawLine(renderer, enemy.x, enemy.y, player.x, player.y);
	}
	else {
		enemy.turnDirection = 1;
	}
}

void renderRays(Character& p,rayCast r[]) {
	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
	for (int i = 0; i < NUM_RAYS; i++) {
		SDL_RenderDrawLine(
			renderer,
			p.x,
			p.y,
			r[i].wallHitX,
			r[i].wallHitY
		);

	}
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

	renderMap();
	renderPlayer(player);
	renderRays(player, playerrays);
	renderPlayer(enemy);

	//if enemy finds player then aline is supposed to track the player until they hid behind a wall which will 
	//put the enemy back in to search mode. though not working quite well.
	if (enemy.playerfound == true)
	{
		targettest();
	}
	else {
		renderRays(enemy, enemyrays);
	}
		
	
	SDL_RenderPresent(renderer);
}

int main(int argc, char* args[])
{
	gameRunning = initializeWindow();

	setup();

	while (gameRunning) {
		processInput();
		update();
		render();

	}

	destroyWindow();
	return 0;
}