#ifndef CONTEXT_H
#define CONTEXT_H

#ifndef _SDL_H
	#include <SDL2/SDL.h>
#endif

#ifndef INPUT_H
	#include "input.h"
#endif

#define SCROLL_SPEED 10

#define BACKGROUND_R 0xFF
#define BACKGROUND_G 0xFF
#define BACKGROUND_B 0xFF

#define CELL_MIN_SIZE 3
#define CELL_MAX_SIZE 80
#define CELL_DEFAULT_SIZE 10
#define CELL_COLOR_R 0x4D
#define CELL_COLOR_G 0x4D
#define CELL_COLOR_B 0x4D
#define NB_CELLS_X 300
#define NB_CELLS_Y 200

#define WINDOW_W 640
#define WINDOW_H 480
#define WINDOW_TITLE "Game of Life"

#define SPEED_MAX 500
#define SPEED_MIN 0

typedef struct Context
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	SDL_Rect view;
	SDL_bool **array;
	Input *in;
	SDL_bool pause;
	Uint32 time, prevTime, speed;
	int cellSize;
}Context;

void ContextMoveView(Context *c);
void ContextDisplay(Context *c);
void ContextCellsUpdate(Context *c);

Context* ContextCreate(void);
void ContextDestroyError(Context *c, char *msg);
void ContextDestroy(Context *c);
#endif
