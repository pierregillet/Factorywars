/**
 * @file
 * @author Corentin Bocquillon <0x539@nybble.fr>
 * @author Pierre Gillet
 * @author Loup Fourment
 *
 * @section LICENSE
 *
 * Copyright (C) 2016 Corentin Bocquillon <0x539@nybble.fr>
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
 * along with factorywars.  If not, see <http://www.gnu.org/licenses/
 *
 * @section DESCRIPTION
 *
 * gui.h is the header of gui.cpp
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <unistd.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

/**
 * Initialize the SDL and SDL_image libraries.
 */
int init_SDL (SDL_Window** window, SDL_Texture** renderer);

/**
 * Load the textures.
 */
int load_media (SDL_Renderer** renderer, SDL_Texture** texture);

/**
 * Load PNG file.
 */
SDL_Texture* load_texture (char* path, SDL_Renderer** renderer);

/**
 * Run the gui.
 */
void gui ();

/**
 * Exit the SDL and SDL_image libraries.
 */
void exit_SDL (SDL_Texture** texture, SDL_Renderer** renderer, SDL_Window** window);
