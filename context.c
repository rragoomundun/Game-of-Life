#include <stdio.h>
#include <stdlib.h>

#include "input.h"
#include "context.h"

void ContextMoveView(Context *c)
{
	if(c->in->key[SDL_SCANCODE_LEFT])
		c->view.x -= SCROLL_SPEED;
	else if(c->in->key[SDL_SCANCODE_RIGHT])
		c->view.x += SCROLL_SPEED;

	if(c->in->key[SDL_SCANCODE_UP])
		c->view.y -= SCROLL_SPEED;
	else if(c->in->key[SDL_SCANCODE_DOWN])
		c->view.y += SCROLL_SPEED;

	if(c->view.x < 0)
		c->view.x = 0;
	else if(c->view.x > NB_CELLS_X * c->cellSize - c->view.w - 1)
		c->view.x = NB_CELLS_X * c->cellSize - c->view.w - 1;
		
	if(c->view.y < 0)
		c->view.y = 0;
	else if(c->view.y > NB_CELLS_Y * c->cellSize - c->view.h - 1)
		c->view.y = NB_CELLS_Y * c->cellSize - c->view.h - 1;
}

void ContextDisplayArray(Context *c)
{
	SDL_Rect rect = {0, 0, c->cellSize, c->cellSize};
	int i, j, xmin, ymin, xmax, ymax;
	
	xmin = c->view.x / c->cellSize;
	ymin = c->view.y / c->cellSize;
	xmax = (c->view.x + c->view.w) / c->cellSize;
	ymax = (c->view.y + c->view.h) / c->cellSize;
	
	for(i = xmin; i <= xmax; i++)
	{
		for(j = ymin; j <= ymax; j++)
		{
			if(c->array[i][j])
			{
				rect.x = i * c->cellSize - c->view.x;
				rect.y = j * c->cellSize - c->view.y;
				
				SDL_RenderCopy(c->renderer, c->texture, NULL, &rect);
			}
		}
	}
}

void ContextDisplay(Context *c)
{
	SDL_RenderClear(c->renderer);
	ContextDisplayArray(c);
	SDL_RenderPresent(c->renderer);
}

int ContextGetCellNeighbours(Context *c, int x, int y)
{
	int i, j, neighbours = 0;
	int xmin, ymin, xmax, ymax;
	
	!x ? (xmin = 0) : (xmin = x - 1);
	!y ? (ymin = 0) : (ymin = y - 1);
	
	(x < NB_CELLS_X - 1) ? (xmax = x + 1) : (xmax = NB_CELLS_X - 2);
	(y < NB_CELLS_Y - 1) ? (ymax = y + 1) : (ymax = NB_CELLS_Y - 2);
	
	for(i = xmin; i <= xmax; i++)
	{
		for(j = ymin; j <= ymax; j++)
		{
			if(c->array[i][j])
				neighbours++;
		}
	}
	
	if(c->array[x][y])
		neighbours--;
		
	return neighbours;
}

void ContextCellsUpdate(Context *c)
{
	int i, j, x, y, neighbours, nbChanges = 0;
	SDL_Point cell[NB_CELLS_X * NB_CELLS_Y];
	
	for(i = 0; i < NB_CELLS_X; i++)
	{
		for(j = 0; j < NB_CELLS_Y; j++)
		{
			neighbours = ContextGetCellNeighbours(c, i, j);
			
			if((!c->array[i][j] && neighbours == 3) ||
			(c->array[i][j] && (neighbours < 2 || neighbours > 3)))
			{
				cell[nbChanges].x = i;
				cell[nbChanges].y = j;
				
				nbChanges++;
			}
		}
	}

	for(i = 0; i < nbChanges; i++)
	{
		x = cell[i].x;
		y = cell[i].y;
		
		if(c->array[x][y])
			c->array[x][y] = SDL_FALSE;
		else
			c->array[x][y] = SDL_TRUE;
	}
}

Context* ContextCreate(void)
{
	Context *c = NULL;
	int i;
	
	if(!(c = calloc(1, sizeof(Context))))
	{
		fprintf(stderr, "ContextCreate : calloc for c failed.\n");
		return NULL;
	}
	
	if(!(c->window = SDL_CreateWindow(WINDOW_TITLE, 
	SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	WINDOW_W, WINDOW_H, 0)))
	{
		ContextDestroyError(c, "cannot create window.");
		return NULL;
	}
	
	if(!(c->renderer = SDL_CreateRenderer(c->window, -1, 0)))
	{
		ContextDestroyError(c, "cannot create renderer.");
		return NULL;
	}
	
	SDL_SetRenderDrawColor(c->renderer, BACKGROUND_R, BACKGROUND_G, BACKGROUND_B, 0x00);
	
	if(!(c->texture = SDL_CreateTexture(c->renderer, SDL_PIXELFORMAT_RGBA8888, 
	SDL_TEXTUREACCESS_STATIC, 1, 1)))
	{
		ContextDestroyError(c, "cannot create texture.");
		return NULL;
	}
	
	SDL_SetTextureColorMod(c->texture, CELL_COLOR_R, CELL_COLOR_G, CELL_COLOR_B);
	
	if(!(c->array = calloc(NB_CELLS_X, sizeof(SDL_bool*))))
	{
		ContextDestroyError(c, "calloc for c->array failed.");
		return NULL;
	}
	
	for(i = 0; i < NB_CELLS_X; i++)
	{
		if(!(c->array[i] = calloc(NB_CELLS_Y, sizeof(SDL_bool))))
		{
			ContextDestroyError(c, "calloc for c->array[i] failed.");
			return NULL;
		}
	}
	
	c->pause = SDL_TRUE;
	c->speed = 85;
	
	c->view.x = c->view.y = 0;
	c->view.w = WINDOW_W; c->view.h = WINDOW_H;
	
	c->cellSize = CELL_DEFAULT_SIZE;
	
	c->in = InputCreate();
	
	return c;
}

void ContextDestroyError(Context *c, char *msg)
{
	fprintf(stderr, "Context error : %s\n", msg);
	ContextDestroy(c);
}

void ContextDestroy(Context *c)
{
	int i;
	
	if(c)
	{
		if(c->texture)
			SDL_DestroyTexture(c->texture);
		
		if(c->renderer)	
			SDL_DestroyRenderer(c->renderer);
		
		if(c->window)
			SDL_DestroyWindow(c->window);
		
		if(c->array)
		{
			for(i = 0; i < NB_CELLS_X; i++)
			{
				if(c->array[i])
					free(c->array[i]);
			}
				
			free(c->array);
		}
		
		if(c->in)
			InputDestroy(c->in);
	
		free(c);
	}
}
