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
 * gui_utils.h is the header of gui_utils.cpp
 */

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "../structures.h"
#include "gui_exit.h"

#include "../gettext.h"
#define _(string) gettext (string)

extern "C"
{
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
SDL_Texture* load_texture (SDL_Renderer* Renderer,
			   const char* path);

/**
 * Blit Textures at given coordinates x,y.
 *
 * @return int true if there is no error
 */
int blit (SDL_Renderer* Renderer,
	  struct coordinates blit_origin,
	  int width,
	  int height,
	  SDL_Texture* texture);

/**
 * Gets and displays the fps in the top-right corner.
 * 
 * @param main_renderer is the renderer used.
 * @param start_time contains the time in milliseconds elapsed since the SDL was initialized.
 * @param
 */
void
display_fps (SDL_Renderer* main_renderer,
	     TTF_Font* ttf_freesans,
	     unsigned int* fps_lasttime,
	     unsigned int* fps_current,
	     unsigned int* fps_frames);

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
