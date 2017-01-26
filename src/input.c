#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "input.h"
#include "context.h"

void InputUpdate(Input *in)
{
	SDL_Event event;

	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				in->quit = SDL_TRUE;
				break;
			case SDL_KEYDOWN:
				in->key[event.key.keysym.scancode] = SDL_TRUE;
				break;
			case SDL_KEYUP:
				in->key[event.key.keysym.scancode] = SDL_FALSE;
				break;
			case SDL_MOUSEMOTION:
				in->x = event.motion.x;
				in->y = event.motion.y;
				break;
			case SDL_MOUSEBUTTONDOWN:
				in->mouse[event.button.button] = SDL_TRUE;
				break;
			case SDL_MOUSEBUTTONUP:
				in->mouse[event.button.button] = SDL_FALSE;
				break;
			default:
				break;
		}
	}
}

Input* InputCreate(void)
{
	Input *in = NULL;
	
	if(!(in = calloc(1, sizeof(Input))))
	{
		fprintf(stderr, "InputCreate : calloc for in failed.\n");
		exit(EXIT_FAILURE);
	}
	
	return in;
}

void InputDestroy(Input *in)
{
	free(in);
}
