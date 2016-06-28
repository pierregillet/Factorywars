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
 * gui.h is the header of gui.cpp
 */

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "structures.h"
#include "display_item.h"
#include "player.h"
#include "display_map.h"
#include "display_item.h"
#include <vector>
#include "multiplayer.h"

extern "C" {
  #include "save.h"
  #include "config.h"
}


/**
 * Store an image into a texture
 *
 * @param Renderer is the renderer.
 * @param path is the path to the image.
 * @return a pointer to the texture.
 */
SDL_Texture* loadTexture (SDL_Renderer** Renderer,
			  std::string path);

/**
 * Loads the textures.
 *
 * @param Renderer is the renderer.
 * @param textures is an array of arrays containing the textures.
 */
void loadMedia (SDL_Renderer** Renderer,
		SDL_Texture* textures[][10]);

/**
 * Initiates the SDL basics, like the window and the renderer.
 *
 * @param main_window is the main window.
 * @param main_renderer is the main renderer.
 * @param screen_height is an int pointer where we will store the screen height.
 * @param screen_width is an int pointer where we will store the screnn width.
 * @param textures is an array of arrays containing the textures.
 */
void init (SDL_Window** main_window,
	   SDL_Renderer** main_renderer,
	   const int screen_height,
	   const int screen_width,
	   SDL_Texture* textures[][10]);

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
		   /* SDL_Texture** biomes, */
		   bool* keys_state,
		   bool* clicks_state,
		   int* screen_height,
		   int* screen_width,
		   struct coordinates* screen_origin,
		   struct coordinates* click_map_coords,
		   std::vector<Player>& players);

/**
 * Move screen_origin and hero_coords coordinates if a key is pressed.
 */
int move_coordinates_on_keydown (struct coordinates* screen_origin,
				 bool* keys_state,
				 struct size* hero_coords,
				 struct size screen_center);

/**
 * Blit Textures at given coordinates x,y.
 *
 * @return int true if there is no error
 */
int blit (SDL_Renderer** Renderer,
	  struct size blit_origin,
	  int width,
	  int height,
	  SDL_Texture* texture);

/**
 * Update the screen. Need to be deleted.
 */
void display_blits(SDL_Renderer** Renderer);

/**
 * Free what needed to be freed from the SDL library.
 *
 * @param main_window is the main window.
 * @param main_renderer is the main renderer.
 * @param current_texture is the player’s texture.
 */
void quit_sdl (SDL_Window** main_window,
	       SDL_Renderer** main_renderer,
	       SDL_Texture** current_texture,
	       SDL_Texture* textures[][10]);

/**
 * Run the gui
 *
 * @param read_pipe is the read end of the pipe to communicate with the network process.
 * @param write_pipe is the write end of the pipe to communicate with the network process.
 * @param players is a vector containing every players connected.
 * @return -1 if there is an error or 0 if there is no error.
 */
int run_gui (int read_pipe, int write_pipe, std::vector<Player>& players);


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
				       int* screen_height,
				       int* screen_width,
				       struct coordinates screen_origin);

/**
 * Blit the players in “players” on the renderer “renderer”
 *
 * @param players is the vector containing every players.
 * @param screen_origin is the offset in pixels from the origin of the map.
 * @param renderer is the renderer where we blit the players.
 * @param player_texture is the texture to blit.
 * @param screen_height is the screen’s height.
 * @param screen_width is the screen’s width.
 */
void display_players (std::vector<Player>& players,
		      struct coordinates screen_origin,
		      SDL_Renderer** renderer,
		      SDL_Texture* player_texture,
		      int screen_height,
		      int screen_width);
