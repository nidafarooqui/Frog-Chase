#include <iostream>
#include <string>
#include "SDL.h"
#include "SDL_image.h"
#define MAX_MAP_X 5
#define MAX_MAP_Y 6

#define TILE_SIZE_X 104
#define TILE_SIZE_Y 96
#define TILE_SIZE_X_MENU 120


//extern SDL_Renderer *renderer;


typedef struct Map
{
	int tile[MAX_MAP_X][MAX_MAP_Y];
	//std::vector<int> getTile();

} Map;



