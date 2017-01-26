#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "context.h"
#include "input.h"

void manageInput(Context *c)
{
	InputUpdate(c->in);
	
	if(c->in->mouse[SDL_BUTTON_LEFT])
		c->array[(c->in->x + c->view.x ) / c->cellSize][(c->in->y + c->view.y) / c->cellSize] = SDL_TRUE;
	else if(c->in->mouse[SDL_BUTTON_RIGHT])
		c->array[(c->in->x + c->view.x ) / c->cellSize][(c->in->y + c->view.y) / c->cellSize] = SDL_FALSE;

	if(c->in->key[SDL_SCANCODE_SPACE])
	{
		c->in->key[SDL_SCANCODE_SPACE] = SDL_FALSE;
		
		if(c->pause == SDL_TRUE)
			c->pause = SDL_FALSE;
		else
			c->pause = SDL_TRUE;
	}

	if(c->in->key[SDL_SCANCODE_W] && c->speed - 1 > SPEED_MIN)
		c->speed--;
	else if(c->in->key[SDL_SCANCODE_Q] && c->speed + 1 < SPEED_MAX)
		c->speed++;

	if(c->in->key[SDL_SCANCODE_S] && c->cellSize < CELL_MAX_SIZE)
		c->cellSize++;
	else if(c->in->key[SDL_SCANCODE_A] && c->cellSize > CELL_MIN_SIZE)
		c->cellSize--;
	
	if(c->in->key[SDL_SCANCODE_R])
		c->cellSize = CELL_DEFAULT_SIZE;

	ContextMoveView(c);
}

int main(void)
{
	Context *c;
	
	if(SDL_Init(SDL_INIT_VIDEO))
	{
		fprintf(stderr, "SDL_Init failed.\n");
		exit(EXIT_FAILURE);
	}
	
	if(!(c = ContextCreate()))
		goto Quit;
	
	while(!c->in->quit)
	{
		manageInput(c);
		ContextDisplay(c);
		
		c->time = SDL_GetTicks();
		
		if(!c->pause && c->time - c->prevTime > c->speed)
		{
			ContextCellsUpdate(c);
			c->prevTime = c->time;
		}
		
		SDL_Delay(15);
	}
	
	ContextDestroy(c);
	
	Quit:
	SDL_Quit();
	
	return EXIT_SUCCESS;
}
