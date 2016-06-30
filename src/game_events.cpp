/**
 * @file
 * @author Corentin Bocquillon <0x539@nybble.fr>
 * @author Pierre Gillet <pierre.gillet+factorywars@linuxw.info>
 * @author Loup Fourment
 *
 * @section LICENSE
 *
 * Copyright (C) 2016 Corentin Bocquillon <corentin@nybble.fr>
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
 * game_events.cpp process the game events.
 */

#include "game_events.h"

int
handle_keydown (SDL_Keycode event_keycode,
		bool* keys_state,
		SDL_Texture* textures[][10],
		SDL_Texture** current_texture)
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
      *current_texture = textures[0][2];
      break;
    case SDLK_RIGHT:
      keys_state[3] = keydown;
      *current_texture = textures[0][3];
      break;
    default:
      break;
    }
  return 1;
}

int
handle_keyup (SDL_Keycode event_keycode,
	      bool* keys_state)
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

int
handle_clickdown (int button,
		  coordinates click_coords,
		  bool* clicks_state,
		  struct coordinates* screen_origin,
		  struct map_coordinates* click_map_coords)
{
  bool clickdown = 1;
  *click_map_coords = get_map_coords (click_coords, *screen_origin);
  switch (button)
    {
    case SDL_BUTTON_LEFT:
	clicks_state[0] = clickdown;
	break;
    case SDL_BUTTON_MIDDLE:
	clicks_state[1] = clickdown;
	break;
    case SDL_BUTTON_RIGHT:
      clicks_state[2] = clickdown;
      break;
    case SDL_BUTTON_X1:
      clicks_state[3] = clickdown;
      break;
    case SDL_BUTTON_X2:
      clicks_state[4] = clickdown;
      break;
    default:
      break;
    }
  return 1;
}

int
handle_mousewheel (int wheel_x,
		   std::vector<Player>& players)
{
  players[0].changeSelectedTool (wheel_x);
  return 1;
}

int
handle_events (SDL_Texture* textures[][10],
	       SDL_Texture** CurrentTexture,
	       bool* keys_state,
	       bool* clicks_state,
	       int screen_height,
	       int screen_width,
	       struct coordinates* screen_origin,
	       struct map_coordinates* click_map_coords,
	       std::vector<Player>& players)
{
  SDL_Event event;
  coordinates click_coords;
  
  while (SDL_PollEvent (&event) != 0)
    {
      if (event.key.repeat != 0)
	continue;
      switch (event.type)
	{
	case SDL_QUIT:
	  return 0;
	  break;
	      
	case SDL_KEYDOWN:
	  handle_keydown (event.key.keysym.sym, keys_state,
			  textures, CurrentTexture);
	  break;

	case SDL_KEYUP:
	  handle_keyup (event.key.keysym.sym, keys_state);
	  break;

	case SDL_MOUSEBUTTONDOWN:
	  click_coords.x = event.button.x;
	  click_coords.y = event.button.y;
	  handle_clickdown (event.button.button, click_coords,
			    clicks_state, screen_origin,
			    click_map_coords);
	  break;

	case SDL_MOUSEBUTTONUP:
	  break;

	case SDL_MOUSEWHEEL:
	  handle_mousewheel (event.wheel.y, players);
	  break;
	  
	default:
	  break;
	}
    }
  
  return 1;
}

struct map_coordinates
get_map_coords (struct coordinates click_coords,
		struct coordinates screen_origin)
{
  struct map_coordinates click_map_coords;

  float x_float = screen_origin.x;
  float y_float = screen_origin.y;

  click_map_coords.chunk.x = (int) (x_float + (float) click_coords.x) / 24.0 / 16.0;
  click_map_coords.chunk.y = (int) (y_float + (float) click_coords.y) / 24.0 / 16.0;
  click_map_coords.square.x = (int) ((x_float + (float) click_coords.x) / 24.0) - ((float) click_map_coords.chunk.x * 16.0);
  click_map_coords.square.y = (int) ((y_float + (float) click_coords.y) / 24.0) - ((float) click_map_coords.chunk.y * 16.0);

  return click_map_coords;
}
