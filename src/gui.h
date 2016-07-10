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
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include "structures.h"
#include "display_item.h"
#include "player.h"
#include "display_map.h"
#include "display_item.h"
#include "multiplayer.h"
#include "game_events.h"
#include "menu.h"
#include "game.h"

extern "C" {
  #include "save.h"
  #include "config.h"
}

// Enumeration to use with keys_state
enum KEYBOARD_BUTTONS
  {
    key_up,
    key_down,
    key_left,
    key_right,
    key_escape
  };

// Enumeration to use with clicks_state
enum MOUSE_BUTTONS
  {
    click_left,
    click_middle,
    click_right,
    click_x1,
    click_x2
  };

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
	   const int screen_width);

/**
 * Blit Textures at given coordinates x,y.
 *
 * @return int true if there is no error
 */
int blit (SDL_Renderer* Renderer,
	  struct size blit_origin,
	  int width,
	  int height,
	  SDL_Texture* texture);

/**
 * Free the textures
 * 
 * @param current_texture is the player’s texture.
 * @param textures is the array containing all the textures.
 */
void free_textures (SDL_Texture* textures[][10],
		    SDL_Texture** current_texture);

/**
 * Quits the SDL and TTF.
 *
 * @param main_window is the main window.
 * @param main_renderer is the main renderer.
 */
void quit_sdl (SDL_Window** main_window,
	       SDL_Renderer** main_renderer);

/**
 * Run the gui
 *
 * @param read_pipe is the read end of the pipe to communicate with the network process.
 * @param write_pipe is the write end of the pipe to communicate with the network process.
 * @param players is a vector containing every players connected.
 * @return -1 if there is an error or 0 if there is no error.
 */
int run_gui (int read_pipe, int write_pipe,
	     std::vector<Player>& players);

/**
 * Calculates the fps using start_time and resets it.
 * 
 * @param start_time contains the time in milliseconds elapsed since the SDL was initialized.
 * 
 * @return The number of fps.
 */
int
get_fps (unsigned int* start_time);

/**
 * Gets and displays the fps in the top-right corner.
 * 
 * @param main_renderer is the renderer used.
 * @param start_time contains the time in milliseconds elapsed since the SDL was initialized.
 * @param
 */
void
display_fps (SDL_Renderer* main_renderer,
	     unsigned int* start_time,
	     TTF_Font* ttf_freesans);

/**
 * Copy a surface in another and return the new surface
 *
 * @param src is the surface to copy
 * @return a pointer to the new surface.
 */
SDL_Surface* copy_surface (SDL_Surface* src);

/**
 * Display a rectangle of a specific color.
 *
 * @param renderer is the renderer used to display the rectangle.
 * @param color is the rectangle’s color.
 * @param rectangle is the position of the rectangle.
 */
void
blit_rect (SDL_Renderer* renderer, SDL_Color color,
	   SDL_Rect rectangle);
