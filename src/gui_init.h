/**
 * @file
 * @author Corentin Bocquillon <0x539@nybble.fr>
 * @author Pierre Gillet <pierre.gillet+factorywars@linuxw.info>
 *
 * @section LICENSE
 *
 * Copyright (C) 2016 Corentin Bocquillon <0x539@nybble.fr>
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
 * gui_init.h is the header of gui_init.cpp.
 */

#pragma once

#include "gui.h"

/**
 * Load the game textures.
 */
void load_game_textures (SDL_Renderer* main_renderer,
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
	   const int screen_width);

