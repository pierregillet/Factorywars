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
 * Copyright (C) 2016 Loup Fourment
 *
 * Copyright (C) 2016 Pierre Gillet <pierre.gillet+factorywars@linuxw.info>
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
 * game_events.h is the header of game_events.cpp.
 */

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>

#include "structures.h"
#include "player.h"
#include "gui.h"

/**
 * Handle the keydown events.
 */
int handle_keydown (SDL_Keycode event_keycode,
		    bool *keys_state,
		    SDL_Texture* textures[][10],
		    SDL_Texture** current_texture);

/**
 * Handle the keyup events.
 */
int handle_keyup (SDL_Keycode event_keycode,
		  bool *keys_state);

/**
 * Handle the clickdown events.
 */
int handle_clickdown (int button,
		      coordinates click_coords,
		      bool *clicks_state,
		      struct coordinates* screen_origin,
		      struct map_coordinates* click_map_coords);

/**
 * Handle the SDL events.
 */
int handle_events (SDL_Texture* textures[][10],
		   SDL_Texture** CurrentTexture,
		   bool* keys_state,
		   bool* clicks_state,
		   int screen_height,
		   int screen_width,
		   struct coordinates* screen_origin,
		   struct map_coordinates* click_map_coords,
		   std::vector<Player>& players);

int handle_mousewheel (int wheel_x, std::vector<Player>& players);

/**
 * Get the chunk coordinates and the square coordinates where the player 
 * clicked.
 *
 * @param click_coords is the coordinates relative to the origin of the screen
 * where the player clicked.
 * @param screen_height is screen’s height.
 * @param screen_width is screen’s width.
 * @param screen_origin is the offset in pixels from the origin of the map.
 * @return a structure map_coordinates containing the chunk and square
 * coordinates where the player clicked.
 */
struct map_coordinates get_map_coords (coordinates click_coords,
				       struct coordinates screen_origin);
