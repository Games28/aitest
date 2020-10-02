#include "Map.h"

int Map::mapHasWallAt(float x, float y)
{
    if (x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT)
    {
        return true;
    }

    int mapGridIndexX = floor(x / DIMENSION);
    int mapGridIndexY = floor(y / DIMENSION);
    return map[mapGridIndexY][mapGridIndexX] != 0;
}

void Map::renderMap(SDL_Renderer* render)
{
	for (int i = 0; i < MAP_NUM_ROWS; i++) {
		for (int j = 0; j < MAP_NUM_COLS; j++) {
			int tileX = j * DIMENSION;
			int tileY = i * DIMENSION;
			int tileColor = map[i][j] != 0 ? 255 : 0;

			SDL_SetRenderDrawColor(render, 0, 0, tileColor, 255);
			SDL_Rect mapTileRect = {
				tileX,
				tileY ,
				DIMENSION ,
				DIMENSION 
			};
			SDL_RenderFillRect(render, &mapTileRect);
		}
	}
}
