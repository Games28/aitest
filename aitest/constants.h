#pragma once
#define FALSE 0
#define TRUE 1
#define PI 3.14159265
#define TWO_PI 6.28318530

#define DIMENSION 64 
#define MAP_NUM_ROWS 14
#define MAP_NUM_COLS 20

#define MINIMAP_SCALE_FACTOR 0.3

#define WINDOW_WIDTH (MAP_NUM_COLS * DIMENSION)
#define WINDOW_HEIGHT (MAP_NUM_ROWS * DIMENSION)
#define Count 2


#define FOV_ANGLE (60 * (PI / 180))

#define NUM_RAYS WINDOW_WIDTH

#define FPS 30
#define FRAME_TIME_LENGTH (1000 / FPS)