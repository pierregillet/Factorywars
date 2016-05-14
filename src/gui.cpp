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
#include "player.h"
#include "action.h"
#include "display_map.cpp"

enum KeyPressTexture
  {
    KEY_PRESS_SURFACE_DEFAULT, 
    KEY_PRESS_SURFACE_UP, 
    KEY_PRESS_SURFACE_DOWN,        
    KEY_PRESS_SURFACE_LEFT, 
    KEY_PRESS_SURFACE_RIGHT, 
    KEY_PRESS_SURFACE_TOTAL
  };
  
SDL_Renderer* gRenderer = NULL; 

SDL_Texture*
loadTexture(std::string path)
{
  SDL_Texture* NewTexture = NULL;
  SDL_Surface* loadedSurface = IMG_Load(path.c_str ());

  NewTexture = SDL_CreateTextureFromSurface (gRenderer, loadedSurface);
  SDL_FreeSurface (loadedSurface);

  return NewTexture;
}

bool 
init (SDL_Window** Window,
      SDL_Texture** KeyPressTexture,
      SDL_Texture** biomes)
{
  const int screen_height = atoi (get_config_value ("height"));
  const int screen_width = atoi (get_config_value ("width"));

  bool success = true;
	
  if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
      printf ("Error: %s\n", SDL_GetError ());
      success = false;
    }
	
  else // if the SDL launched correctly
    {
      *Window = SDL_CreateWindow ("Factorywars",
				  SDL_WINDOWPOS_UNDEFINED,
				  SDL_WINDOWPOS_UNDEFINED,
				  screen_width, screen_height,
				  SDL_WINDOW_SHOWN);
	  
      if (Window == NULL) 
	{
	  success = false;
	  printf ("Couldn’t create window: %s\n", SDL_GetError());
	}
	  
      else // if window has been created without errors
	{
	  gRenderer = SDL_CreateRenderer (*Window, -1,
					   SDL_RENDERER_ACCELERATED);
	  SDL_SetRenderDrawColor (gRenderer, 0xFF,0xFF,0xFF,0xFF);
	}
    }

  if (!loadMedia (KeyPressTexture))
    success = false;
  load_biomes(biomes);
  return success;
}


bool 
loadMedia (SDL_Texture** KeyPressTexture)
{
  bool success = true;
  
  // every box of the table is associated to an image
  KeyPressTexture[KEY_PRESS_SURFACE_DEFAULT] = loadTexture ("media/textures/LEFT.png");
  if (KeyPressTexture[KEY_PRESS_SURFACE_DEFAULT] == NULL)
    success = false;
    
  KeyPressTexture[KEY_PRESS_SURFACE_UP] = loadTexture ("media/textures/LEFT.png");
  if (KeyPressTexture[KEY_PRESS_SURFACE_UP ] == NULL)
    success = false;
    
  KeyPressTexture[KEY_PRESS_SURFACE_DOWN ] = loadTexture ("media/textures/RIGHT.png");
  if (KeyPressTexture[KEY_PRESS_SURFACE_DOWN ] == NULL)
    success = false;
    
  KeyPressTexture[KEY_PRESS_SURFACE_LEFT ] = loadTexture ("media/textures/LEFT.png");
  if (KeyPressTexture[KEY_PRESS_SURFACE_LEFT ] == NULL)
    success = false;  
    
  KeyPressTexture[KEY_PRESS_SURFACE_RIGHT ] = loadTexture ("media/textures/RIGHT.png");
  if (KeyPressTexture[KEY_PRESS_SURFACE_DEFAULT] == NULL)
    success = false;

  return success;
}

void 
refresh_renderer ()
{
  SDL_RenderClear (gRenderer);
}

void
display_blits()
{
  SDL_RenderPresent (gRenderer);
}

int
blit (int x, int y, int width, int height, SDL_Texture* texture)
{
  SDL_Rect Rect = {.x = x, .y = y, .w = width, .h = height};
  // SDL_QueryTexture (texture, NULL, NULL, &Rect.w, &Rect.h);
 
  SDL_RenderSetViewport(gRenderer, &Rect);
  SDL_RenderCopy (gRenderer, texture, NULL,NULL);

  return 1;
}

int 
run_gui ()
{
  SDL_Texture *biomes[4];

  SDL_Window *Window = NULL;
  SDL_Texture *key_press_texture [KEY_PRESS_SURFACE_TOTAL];

  if (!init (&Window, key_press_texture, biomes))
    return 1;
  
  int x = 0;
  int y = 0;
  bool quit = false;
  
  SDL_Event e;
  SDL_Texture *CurrentTexture = NULL;
  CurrentTexture = key_press_texture [KEY_PRESS_SURFACE_DEFAULT];
  
  // we don't use CurrentTexture for now
  // cause we don't display the hero, just the map
  int move_state[4] = {0};
  int keydown = 0;

  const int screen_height = atoi (get_config_value ("height"));
  const int screen_width = atoi (get_config_value ("width"));

  // We need to display the map at the beginning
  display_background ("save", biomes, x, y);
  blit (screen_width / 2, screen_height / 2, 25, 41, CurrentTexture);
  display_blits();

  while (!quit)
    {
      while (SDL_PollEvent (&e) != 0)
	{
	  if (e.type == SDL_QUIT)
	    quit = true;
	  else if (e.type == SDL_KEYDOWN)
	    {
	      keydown = 1;
	      if (e.key.repeat != 0)
		continue;
	    }
	  else if (e.type == SDL_KEYUP)
	    keydown = 0;

	  switch (e.key.keysym.sym)
	    {
	    case SDLK_UP:
	      move_state[0] = keydown;
	      break;
         
	    case SDLK_DOWN:
	      move_state[1] = keydown;
	      break;
            
	    case SDLK_LEFT:
	      move_state[2] = keydown;
	      CurrentTexture = key_press_texture[KEY_PRESS_SURFACE_LEFT];
	      break;
            
	    case SDLK_RIGHT:
	      move_state[3] = keydown;
	      CurrentTexture = key_press_texture[KEY_PRESS_SURFACE_RIGHT];
	      break;
            
	    default:
	      CurrentTexture = key_press_texture[KEY_PRESS_SURFACE_DEFAULT];
	      break;
	    }
	}

      // Update x and y
      y += (move_state[0])? (-5) : 0;
      y += (move_state[1])? 5 : 0;
      x += (move_state[2])? (-5) : 0;
      x += (move_state[3])? 5 : 0;

      for (int i = 0; i < 4; i++)
	{
	  if (move_state[i])
	    {
	      refresh_renderer ();
	      display_background ("save", biomes, x, y);
	      blit (screen_width / 2, screen_height / 2, 25, 41, CurrentTexture);
	      display_blits();
	      break;
	    }
	}

      SDL_Delay (100/6);
    }
  return 1;
}  


int
get_event ()
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
    {
      switch (event.type)
	{
	case SDL_KEYDOWN:
	  handle_keydown (event.key.keysym.sym);
	  break;
	default:
	  break;
	}
    }
  return 1;
}


