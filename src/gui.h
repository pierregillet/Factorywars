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
// #include "action.h"
#include "display_map.h"
#include "display_item.h"
#include "config.h"
#include <vector>
#include "save.h"
#include "multiplayer.h"

enum KeyPressTexture
  {
    KEY_PRESS_SURFACE_DEFAULT, 
    KEY_PRESS_SURFACE_UP, 
    KEY_PRESS_SURFACE_DOWN,        
    KEY_PRESS_SURFACE_LEFT, 
    KEY_PRESS_SURFACE_RIGHT, 
    KEY_PRESS_SURFACE_TOTAL
  };  

SDL_Texture* loadTexture (SDL_Renderer** Renderer,
			  std::string path);

bool loadMedia (SDL_Renderer** Renderer,
		SDL_Texture** KeyPressTexture);

/*
 * Initiates the SDL basics, like Window and Renderer.
 */
 
bool init (SDL_Window** Window,
	   SDL_Renderer** Renderer,
	   SDL_Texture** KeyPressTexture,
	   SDL_Texture** biomes,
	   SDL_Texture** items,
	   int* screen_height,
	   int* screen_width);

int handle_keydown (SDL_Keycode event_keycode,
		    bool *keys_state,
		    SDL_Texture** CurrentTexture,
		    SDL_Texture** key_press_texture);

int handle_keyup (SDL_Keycode event_keycode,
		  bool *keys_state,
		  SDL_Texture** CurrentTexture,
		  SDL_Texture** key_press_texture);

int handle_clickdown (int button,
		      coordinates click_coords,
		      bool *clicks_state,
		      struct coordinates* screen_origin,
		      struct map_coordinates* click_map_coords);

int handle_clickup (int button,
		    coordinates click_coords,
		    bool *clicks_state,
		    struct coordinates* screen_origin);

int handle_events (SDL_Texture** CurrentTexture,
		   SDL_Texture** biomes,
		   bool* keys_state,
		   bool* clicks_state,
		   SDL_Texture** key_press_texture,
		   int* screen_height,
		   int* screen_width,
		   struct coordinates* screen_origin,
		   struct coordinates* click_map_coords);

int move_coordinates_on_keydown (struct coordinates* screen_origin,
				 bool* keys_state,
				 struct coordinates* hero_coords,
				 struct coordinates screen_center);

void refresh_renderer(SDL_Renderer** Renderer);

/*
 * Blit Textures at given coordinates x,y.
 * @return int true if there is no error
 * The Texture needs a Renderer to be blit.
 */
int blit (SDL_Renderer** Renderer,
	  struct coordinates screen_origin,
	  int width,
	  int height,
	  SDL_Texture* texture);

void display_blits(SDL_Renderer** Renderer);

void quit_sdl (SDL_Window** Window,
	       SDL_Renderer** Renderer,
	       SDL_Texture** CurrentTexture,
	       SDL_Texture** biomes,
	       SDL_Texture** items);

int run_gui (int read_pipe, int write_pipe, std::vector<Player>& players);

struct map_coordinates get_map_coords (coordinates click_coords,
				       int* screen_height,
				       int* screen_width,
				       struct coordinates screen_origin);

