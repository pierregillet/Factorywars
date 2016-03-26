/**
 * @file
 * @author Corentin Bocquillon <0x539@nybble.fr>
 *
 * @section LICENSE
 *
 * Copyright (C) 2016 Corentin Bocquillon <0x539@nybble.fr>
 *
 * factorywars is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * factorywars is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with factorywars.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 *
 * gui.cpp is the code for the gui.
 */

#include "gui.h"

int
init_SDL (SDL_Window** window, SDL_Renderer** renderer)
{
  if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
      fprintf (stderr, "SDL could not initialize! SDL Error: %s\n", SDL_GetError ());
      return 0;
    }

  //Create window
  *window = SDL_CreateWindow ("factorywars", SDL_WINDOWPOS_UNDEFINED,
			      SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
			      SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (*window == NULL)
    {
      fprintf(stderr, "Window could not be created! SDL Error: %s\n", SDL_GetError());
      return 0;
    }
 
  *renderer = SDL_CreateRenderer (*window, -1, SDL_RENDERER_ACCELERATED);
  if (*renderer == NULL)
    {
      fprintf(stderr, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
      return 0;
    }
  SDL_SetRenderDrawColor (*renderer, 0xFF, 0xFF, 0xFF, 0xFF);
 
  //Initialize PNG loading
  int img_flags = IMG_INIT_PNG;
  if (!(IMG_Init (img_flags) & img_flags))
    {
      fprintf(stderr, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError ());
      return 0;
    }

  return 1;
}

int
load_media (SDL_Renderer** renderer, SDL_Texture** texture)
{
  *texture = load_texture ("media/factorywars.png", renderer);
  if (*texture == NULL)
    {
      fprintf(stderr, "Failed to load PNG image!\n");
      return 0;
    }

  return 1;
}

SDL_Texture*
load_texture (char* path, SDL_Renderer** renderer)
{
  SDL_Texture* new_texture = NULL;

  SDL_Surface* loaded_surface = IMG_Load (path);
  if (loaded_surface == NULL)
    {
      fprintf (stderr, "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError () );
    }
  else
    {
      new_texture = SDL_CreateTextureFromSurface (*renderer, loaded_surface);
      if (new_texture == NULL)
  	{
  	  fprintf (stderr, "Unable to optimize image %s! SDL Error: %s\n", path, SDL_GetError ());
  	}
      SDL_FreeSurface (loaded_surface);
    }

  return new_texture;
}

void
gui ()
{
  SDL_Window *window = NULL;
  SDL_Texture *PNG_texture = NULL;
  SDL_Renderer *renderer = NULL;

  if (!init_SDL (&window, &renderer))
    exit (EXIT_FAILURE);
  if (!load_media (&renderer, &PNG_texture))
    exit (EXIT_FAILURE);

  // Clear screen
  SDL_RenderClear (renderer);

  // Render texture to screen
  SDL_RenderCopy (renderer, PNG_texture, NULL, NULL);

  // Update screen
  SDL_RenderPresent (renderer);

  // Event Loop
  int quit = 0;
  SDL_Event e;

  while (!quit)
    {
      while (SDL_PollEvent (&e))
	{
	  switch (e.type)
	    {
	    case SDL_QUIT:
	      quit = 1;
	      break;
	    }
	}
    }

  exit_SDL (&PNG_texture, &renderer, &window);
}

void
exit_SDL (SDL_Texture** texture, SDL_Renderer** renderer, SDL_Window** window)
{
  SDL_DestroyTexture (*texture);
  *texture = NULL;

  // Destroy texture
  SDL_DestroyRenderer (*renderer);
  *renderer = NULL;

  // Destroy window
  SDL_DestroyWindow (*window);
  *window = NULL;
  
  //Quit SDL subsystems
  IMG_Quit();
  SDL_Quit();
}
