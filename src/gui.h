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
 * gui.h is the header of gui.cpp
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "display_map.h"
#include "config.h"
#include "structures.h"

/**
  * Blit Textures at given coordonates x,y.
  * Returns an int, which is true if there is no error.
  * The Texture needs a Renderer for being blited.
  */
int blit (int x, int y, int width, int height, SDL_Texture* texture);

/**
  * Initiates the SDL basics, like Window and Renderer.
  */
bool init (SDL_Window** Window, SDL_Texture** KeyPressTexture, SDL_Texture** biomes);

bool loadMedia (SDL_Texture** KeyPressTexture);

int run_gui ();

SDL_Texture* loadTexture (std::string path);

void refresh_renderer();

void display_blits();

// int handle_events (coordinates *offset, SDL_Texture** CurrentTexture, SDL_Texture** biomes, bool* keys_state);

coordinates
move_coordinates (bool* keys_state, coordinates* offset);

int handle_keydown (SDL_Keycode event_keycode, bool *keys_state, SDL_Texture** CurrentTexture, SDL_Texture** key_press_texture);

int handle_keyup (SDL_Keycode event_keycode, bool *keys_state, SDL_Texture** CurrentTexture, SDL_Texture** key_press_texture);

