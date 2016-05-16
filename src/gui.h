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

enum KeyPressTexture
  {
    KEY_PRESS_SURFACE_DEFAULT, 
    KEY_PRESS_SURFACE_UP, 
    KEY_PRESS_SURFACE_DOWN,        
    KEY_PRESS_SURFACE_LEFT, 
    KEY_PRESS_SURFACE_RIGHT, 
    KEY_PRESS_SURFACE_TOTAL
  };  

SDL_Texture* loadTexture (SDL_Renderer** Renderer, std::string path);

bool loadMedia (SDL_Renderer** Renderer, SDL_Texture** KeyPressTexture);

/*
 * Initiates the SDL basics, like Window and Renderer.
 */
bool init (SDL_Renderer** Renderer, SDL_Texture** KeyPressTexture, SDL_Texture** biomes, const int screen_height, const int screen_width);

int handle_keydown (SDL_Keycode event_keycode, bool *keys_state, SDL_Texture** CurrentTexture, SDL_Texture** key_press_texture);

int handle_keyup (SDL_Keycode event_keycode, bool *keys_state, SDL_Texture** CurrentTexture, SDL_Texture** key_press_texture);

int handle_events (SDL_Texture** CurrentTexture, SDL_Texture** biomes, bool* keys_state, SDL_Texture** key_press_texture);

int move_coordinates_on_keydown (int* x, int* y, bool* keys_state);

void refresh_renderer(SDL_Renderer** Renderer);

/*
 * Blit Textures at given coordonates x,y.
 * Returns an int, which is true if there is no error.
 * The Texture needs a Renderer for being blited.
 */
int blit (SDL_Renderer** Renderer, int x, int y, int width, int height, SDL_Texture* texture);

void display_blits(SDL_Renderer** Renderer);

void close ();

int run_gui ();

