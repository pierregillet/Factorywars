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
 * gui_init.cpp contains functions used on the game start.
 */

#include "gui_init.h"

void
load_game_textures (SDL_Renderer* main_renderer,
		    SDL_Texture* textures[][10])
{
  const char*
    player_textures_paths[] = {TEXTURESDIR"/LEFT.png",
			       TEXTURESDIR"/RIGHT.png",
			       TEXTURESDIR"/LEFT.png",
			       TEXTURESDIR"/RIGHT.png"};

  const char*
    biomes_textures_paths[] = {TEXTURESDIR"/biome1.png",
			       TEXTURESDIR"/biome1.png",
			       TEXTURESDIR"/biome2.png",
			       TEXTURESDIR"/biome1.png",
			       TEXTURESDIR"/biome1.png"};

  const char*
    objects_textures_paths[] = {TEXTURESDIR"/arbre.png",
				TEXTURESDIR"/pierre1.png",
				TEXTURESDIR"/pierre2.png",
				TEXTURESDIR"/pierre3.png"};

  const char*
    hud_textures_paths[] = {TEXTURESDIR"/toolbar.png"};

  for (int i = 0;
       i < sizeof (player_textures_paths) / sizeof (char*);
       i++)
    {
      textures[0][i] = load_texture (main_renderer, player_textures_paths[i]);
    }
  
  for (int i = 0;
       i < sizeof (biomes_textures_paths) / sizeof (char*);
       i++)
    {
      textures[1][i] = load_texture (main_renderer, biomes_textures_paths[i]);
    }

  for (int i = 0;
       i < sizeof (objects_textures_paths) / sizeof (char*);
       i++)
    {
      textures[2][i] = load_texture (main_renderer, objects_textures_paths[i]);
    }

  for (int i = 0;
       i < sizeof (hud_textures_paths) / sizeof (char*);
       i++)
    {
      textures[3][i] = load_texture (main_renderer, hud_textures_paths[i]);
    }
}

void
init (SDL_Window** main_window,
      SDL_Renderer** main_renderer,
      const int screen_height,
      const int screen_width)
{
  if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
      fprintf (stderr, "Error: %s\n", SDL_GetError ());
    }

  if( TTF_Init() == -1 )
    {
      fprintf(stderr, "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
      SDL_Quit();
    }
	
  // if the SDL launched correctly
  *main_window = SDL_CreateWindow ("Factorywars",
				   SDL_WINDOWPOS_UNDEFINED,
				   SDL_WINDOWPOS_UNDEFINED,
				   screen_width,
				   screen_height,
				   SDL_WINDOW_SHOWN
				   | SDL_WINDOW_RESIZABLE);
	  
  if (*main_window == NULL) 
    {
      fprintf (stderr, "Couldn’t create window : %s\n", SDL_GetError());
      SDL_Quit();
    }
	  
  // if window has been created without errors
  *main_renderer = SDL_CreateRenderer (*main_window, -1,
				       SDL_RENDERER_ACCELERATED
				       | SDL_RENDERER_PRESENTVSYNC);

  SDL_SetRenderDrawColor (*main_renderer, 0xFF,0xFF,0xFF,0xFF);

  if (!IMG_Init (IMG_INIT_PNG) & IMG_INIT_PNG)
    fprintf (stderr, "Error while initializing SDL_image library.\n");

  if (TTF_Init () == -1)
    fprintf (stderr, "Error while initializing SDL_ttf library.\n");
}

