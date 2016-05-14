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
// #include "action.h"
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
  
  SDL_Event e;
  SDL_Texture *CurrentTexture = NULL;
  CurrentTexture = key_press_texture [KEY_PRESS_SURFACE_DEFAULT];
    coordinates offset;
  
  /* 
   * keys_state only has 4 elements
   * because we only use 4 keys as of now
   * keys_state[1] -> SDLK_UP
   * keys_state[2] -> SDLK_DOWN
   * keys_state[3] -> SDLK_LEFT
   * keys_state[4] -> SDLK_RIGHT
   */
  bool keys_state[4] = {0};

  const int screen_height = atoi (get_config_value ("height"));
  const int screen_width = atoi (get_config_value ("width"));
  int quit = 0;

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
	      handle_keydown (e.key.keysym.sym, keys_state, &CurrentTexture, key_press_texture);
	      if (e.key.repeat != 0)
		continue;
	    }
	  else if (e.type == SDL_KEYUP)
	    {
	      handle_keyup (e.key.keysym.sym, keys_state, &CurrentTexture, key_press_texture);
	    }
	}

      // Update x and y
      y += (keys_state[0])? (-5) : 0; 
      y += (keys_state[1])? 5 : 0;
      x += (keys_state[2])? (-5) : 0;
      x += (keys_state[3])? 5 : 0;

      // At least for now, we didn’t authorize negative coordinates
      x = (x < 0)? 0 : x;
      y = (y < 0)? 0 : y;

      for (int i = 0; i < 4; i++)
	{
	  if (keys_state[i])
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

/*
int
handle_events (coordinates *offset, SDL_Texture** CurrentTexture, SDL_Texture** biomes, bool* keys_state)
{  
  return 1;
}
  */

coordinates
move_coordinates (bool* keys_state, coordinates* offset)
{
  offset->y += (keys_state[0])? (-5) : 0;
  offset->y += (keys_state[1])? 5 : 0;
  offset->x += (keys_state[2])? (-5) : 0;
  offset->x += (keys_state[3])? 5 : 0;
  return *offset;
}

int
handle_keydown (SDL_Keycode event_keycode, bool* keys_state, SDL_Texture** CurrentTexture, SDL_Texture** key_press_texture)
{
  bool keydown = 1;
  switch (event_keycode)
    {
    case SDLK_UP:
      keys_state[0] = keydown;
      break;
    case SDLK_DOWN:
      keys_state[1] = keydown;
      break;
    case SDLK_LEFT:
      keys_state[2] = keydown;
      *CurrentTexture = key_press_texture[KEY_PRESS_SURFACE_LEFT];
      break;
    case SDLK_RIGHT:
      keys_state[3] = keydown;
      *CurrentTexture = key_press_texture[KEY_PRESS_SURFACE_RIGHT];
      break;
    default:
      break;
    }
  return 1;
}

int
handle_keyup (SDL_Keycode event_keycode, bool* keys_state, SDL_Texture** CurrentTexture, SDL_Texture** key_press_texture)
{
  bool keyup = 0;
  switch (event_keycode)
    {
    case SDLK_UP:
      keys_state[0] = keyup;
      break;
    case SDLK_DOWN:
      keys_state[1] = keyup;
      break;
    case SDLK_LEFT:
      keys_state[2] = keyup;
      break;
    case SDLK_RIGHT:
      keys_state[3] = keyup;
      break;
    default:
      break;
    }
  return 1;
}

