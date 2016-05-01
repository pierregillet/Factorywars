/**
 * @file
 * @author Corentin Bocquillon <0x539@nybble.fr>
 * @author Pierre Gillet
 * @author Loup Fourment
 *
 * @section LICENSE
 *
 * Copyright (C) 2016 Corentin Bocquillon <corentin@nybble.fr>
 *
 * Copyright (C) 2016 Loup Fourment
 *
 * Copyright (C) 2016 Pierre Gillet
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
 * gui.cpp contains the gui
 */

#include "gui.h"

enum KeyPressTexture
  {
    KEY_PRESS_SURFACE_DEFAULT, 
    KEY_PRESS_SURFACE_UP, 
    KEY_PRESS_SURFACE_DOWN,        
    KEY_PRESS_SURFACE_LEFT, 
    KEY_PRESS_SURFACE_RIGHT, 
    KEY_PRESS_SURFACE_TOTAL
  };
 
SDL_Texture*
loadTexture(std::string path, SDL_Renderer* gRenderer)
{
  SDL_Texture* NewTexture = NULL;
  SDL_Surface* loadedSurface = IMG_Load(path.c_str ());

  NewTexture = SDL_CreateTextureFromSurface (gRenderer, loadedSurface);
  SDL_FreeSurface (loadedSurface);

  return NewTexture;
}

bool 
init (SDL_Window** Window, SDL_Renderer** gRenderer,
      SDL_Texture** KeyPressTexture)
{
  bool success = true;
	
  if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
      printf ("Error: %s\n", SDL_GetError ());
      success = false;
    }
	
  else //si la SDL s'est bien lancee	
    {
      *Window = SDL_CreateWindow ("factorywars", SDL_WINDOWPOS_UNDEFINED,
				  SDL_WINDOWPOS_UNDEFINED, 640, 480,
				  SDL_WINDOW_SHOWN);
	  
      if (Window == NULL) 
	{
	  success = false;
	  printf ("Couldn’t create window: %s\n", SDL_GetError());
	}
	  
      else //si la fenetre est bien cree
	{
	  *gRenderer = SDL_CreateRenderer (*Window, -1,
					   SDL_RENDERER_ACCELERATED);
	  SDL_SetRenderDrawColor (*gRenderer, 0xFF,0xFF,0xFF,0xFF);
	}
    }

  if (!loadMedia (KeyPressTexture, *gRenderer))
    success = false;

  return success;
}


bool 
loadMedia (SDL_Texture** KeyPressTexture, SDL_Renderer* gRenderer)
{
  bool success = true;
  
  //chaque case du tableau se voit atribuer une image
  KeyPressTexture[KEY_PRESS_SURFACE_DEFAULT] = loadTexture ("media/textures/LEFT.png", gRenderer);
  if (KeyPressTexture[KEY_PRESS_SURFACE_DEFAULT] == NULL)
    success = false;
    
  KeyPressTexture[KEY_PRESS_SURFACE_UP] = loadTexture ("media/textures/LEFT.png", gRenderer);
  if (KeyPressTexture[KEY_PRESS_SURFACE_UP ] == NULL)
    success = false;
    
  KeyPressTexture[KEY_PRESS_SURFACE_DOWN ] = loadTexture ("media/textures/RIGHT.png", gRenderer);
  if (KeyPressTexture[KEY_PRESS_SURFACE_DOWN ] == NULL)
    success = false;
    
  KeyPressTexture[KEY_PRESS_SURFACE_LEFT ] = loadTexture ("media/textures/LEFT.png", gRenderer);
  if (KeyPressTexture[KEY_PRESS_SURFACE_LEFT ] == NULL)
    success = false;  
    
  KeyPressTexture[KEY_PRESS_SURFACE_RIGHT ] = loadTexture ("media/textures/RIGHT.png", gRenderer);
  if (KeyPressTexture[KEY_PRESS_SURFACE_DEFAULT] == NULL)
    success = false;

  return success;
}


bool
blit (int x, int y, int width, int height, SDL_Texture* texture, SDL_Renderer* gRenderer)
{
  bool success = true;
  SDL_Rect Rect = {.x = x, .y = y, .w = width, .h = height};
  // SDL_QueryTexture (texture, NULL, NULL, &Rect.w, &Rect.h);
 
  SDL_RenderSetViewport(gRenderer, &Rect);
  SDL_RenderClear (gRenderer);
  SDL_RenderCopy (gRenderer, texture, NULL,NULL);
  SDL_RenderPresent (gRenderer);

  return success;
}

int 
run_gui ()
{
  SDL_Window *Window = NULL;
  SDL_Renderer *gRenderer = NULL;

  SDL_Texture *key_press_texture [KEY_PRESS_SURFACE_TOTAL];

  if (!init (&Window, &gRenderer, key_press_texture))
    return 1;
  
  int x = 0;
  int y = 0;
  bool quit = false;
  
  SDL_Event e;
  SDL_Texture *CurrentTexture = NULL;
  CurrentTexture = key_press_texture [KEY_PRESS_SURFACE_DEFAULT];

  blit(x, y, 50, 82, CurrentTexture, gRenderer);
  while (!quit)
    {
      while (SDL_PollEvent (&e) != 0)
	{
	  if (e.type == SDL_QUIT)
	    quit = true;
	  else if (e.type == SDL_KEYDOWN)
	    {
	      switch (e.key.keysym.sym)
		{
		case SDLK_UP:
		  y -= 5;
		  CurrentTexture = key_press_texture[KEY_PRESS_SURFACE_UP];
		  break;
         
		case SDLK_DOWN:
		  y += 5;
		  CurrentTexture = key_press_texture[KEY_PRESS_SURFACE_DOWN];
		  break;
            
		case SDLK_LEFT:
		  x -= 5;
		  CurrentTexture = key_press_texture[KEY_PRESS_SURFACE_LEFT];
		  break;
            
		case SDLK_RIGHT:
		  x += 5; 
		  CurrentTexture = key_press_texture[KEY_PRESS_SURFACE_RIGHT];
		  break;
            
		default:
		  CurrentTexture = key_press_texture[KEY_PRESS_SURFACE_DEFAULT];
		  break;
		}
	      blit(x, y, 50, 82, CurrentTexture, gRenderer);
	    }
	}
      SDL_Delay (100/6);
    }
  return 1;
}  

