#ifndef INPUT_H
#define INPUT_H

#ifndef _SDL_H
	#include <SDL2/SDL.h>
#endif

typedef struct Input
{
	SDL_bool key[SDL_NUM_SCANCODES];
	SDL_bool quit;
	int x, y;
	SDL_bool mouse[6];
}Input;

void InputUpdate(Input *in);

Input* InputCreate(void);
void InputDestroy(Input *in);
#endif
