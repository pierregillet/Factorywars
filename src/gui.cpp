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
init_SDL (SDL_Window** window, SDL_Surface** screen_surface)
{
  int success = 1;

  if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
      fprintf (stderr, "SDL could not initialize! SDL Error: %s\n", SDL_GetError ());
      success = 0;
    }
  else
    {
      //Create window
      *window = SDL_CreateWindow ("factorywars", SDL_WINDOWPOS_UNDEFINED,
				 SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
				 SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
      if(*window == NULL)
	{
	  fprintf(stderr, "Window could not be created! SDL Error: %s\n", SDL_GetError());
	  success = 0;
	}
      else
	{
	  //Initialize PNG loading
	  int img_flags = IMG_INIT_PNG;
	  if (!(IMG_Init (img_flags) & img_flags))
	    {
	      fprintf(stderr, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError ());
	      success = 0;
	    }
	  else
	    {
	      //Get window surface
	      *screen_surface = SDL_GetWindowSurface (*window);
	    }
	}
    }

  return success;
}

int
load_media (SDL_Surface** screen_surface, SDL_Surface** PNG_surface)
{
  int success = 1;
  
  *PNG_surface = load_surface ("media/factorywars.png", screen_surface);
  if (*PNG_surface == NULL)
    {
      fprintf(stderr, "Failed to load PNG image!\n");
      success = 0;
    }

  return success;
}

SDL_Surface*
load_surface (char* path, SDL_Surface** screen_surface)
{
  SDL_Surface* surface = NULL;

  SDL_Surface* loaded_surface = IMG_Load (path);
  if (loaded_surface == NULL)
    {
      fprintf (stderr, "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError () );
    }
  else
    {
      surface = SDL_ConvertSurface (loaded_surface, (**screen_surface).format, 0);
      if (surface == NULL)
  	{
  	  fprintf (stderr, "Unable to optimize image %s! SDL Error: %s\n", path, SDL_GetError ());
  	}
	    
      SDL_FreeSurface (loaded_surface);
    }

  return surface;
}

void
gui ()
{
  SDL_Window *window = NULL;
  SDL_Surface *screen_surface = NULL;
  SDL_Surface *PNG_surface = NULL;

  if (!init_SDL (&window, &screen_surface))
    exit (EXIT_FAILURE);
  if (!load_media (&screen_surface, &PNG_surface))
    exit (EXIT_FAILURE);

  SDL_BlitSurface (PNG_surface, NULL, screen_surface, NULL);
  SDL_UpdateWindowSurface (window);

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

  exit_SDL (&PNG_surface, &window);
}

void
exit_SDL (SDL_Surface** PNG_surface, SDL_Window** window)
{
	SDL_FreeSurface (*PNG_surface);
	*PNG_surface = NULL;

	//Destroy window
	SDL_DestroyWindow (*window);
	*window = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}
