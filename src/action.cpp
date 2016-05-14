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
 * action.cpp contains functions to handle user actions/input
 */

#include "action.h"


int
handle_keydown (SDL_Keycode event_keycode, bool* keys_state, SDL_Texture CurrentTexture)
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
      break;
    case SDLK_RIGHT:
      keys_state[3] = keydown;
      break;
    default:
      break;
    }
  return 1;
}

int
handle_keyup (SDL_Keycode event_keycode, bool* keys_state, SDL_Texture CurrentTexture)
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
