/**
 * @file
 * @author Corentin Bocquillon <0x539@nybble.fr>
 * @author Pierre Gillet <pierre.gillet+factorywars@linuxw.info>
 * @author Loup Fourment
 *
 * @section LICENSE
 *
 * Copyright (C) 2016 Corentin Bocquillon <0x539@nybble.fr>
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
 * gui_utils.cpp contains the gui's utils.
 */

#include "gui_utils.h"

SDL_Texture*
load_texture (SDL_Renderer* main_renderer, const char* path)
{
  SDL_Texture* new_texture = NULL;
  SDL_Surface* loaded_surface = IMG_Load(path);

  new_texture = SDL_CreateTextureFromSurface (main_renderer, loaded_surface);
  SDL_FreeSurface (loaded_surface);

  if (new_texture == NULL)
    fprintf (stderr, "Could not load the texture : %s", SDL_GetError);
 
  return new_texture;
}

int
blit (SDL_Renderer* main_renderer,
      struct coordinates blit_origin,
      int width,
      int height,
      SDL_Texture* texture)
{
  SDL_Rect rectangle = {.x = blit_origin.x,
			.y = blit_origin.y,
			.w = width,
			.h = height};
  
  SDL_RenderSetViewport (main_renderer, &rectangle);
  SDL_RenderCopy (main_renderer, texture, NULL, NULL);

  return 1;
}

// WORK IN PROGRESS
void
display_ground (SDL_Renderer* main_renderer,
		SDL_Texture* textures[][10],
		const struct size screen_dimensions,
		struct coordinates player_coordinates)
{
  const unsigned int NUMBER_OF_SQUARES_PER_ROW = 16; // Per chunk
  unsigned int default_square_width = 24; // In pixels

  // TODO : Make square_width change in function of zoom level.

  struct unsigned_size squares_to_draw = {.x = screen_dimensions.x / default_square_width,
					  .y = screen_dimensions.y / default_square_width};

  // Load the squares depending on the player position.
  struct coordinates square_top_left_corner = {.x = 0,
					       .y = 0};
  unsigned int biome_texture; // Contains the square's biome's number.

  for (int row = 0; row <= squares_to_draw.y; row ++)
    {
      for (int column = 0; column <= squares_to_draw.x; column ++)
	{
	  // biome_texture = map.;
	  
	  blit (main_renderer, square_top_left_corner,
		default_square_width, default_square_width,
		textures[1][biome_texture]);
	}
    }
}

void
display_fps (SDL_Renderer* main_renderer, TTF_Font* ttf_freesans,
	     unsigned int* fps_lasttime, unsigned int* fps_current,
	     unsigned int* fps_frames)
{
  SDL_Color white = {255, 255, 255};
  char fps[10];
  
  (*fps_frames)++;
  if (*fps_lasttime < SDL_GetTicks() - 500) // -1000 because we count the FPS on 1000ms = 1s
    {
      *fps_lasttime = SDL_GetTicks();
      *fps_current = *fps_frames;
      *fps_frames = 0;
    }

  snprintf (fps, 10, "%d %s", *fps_current, _("fps"));

  SDL_Surface* surface_message = TTF_RenderText_Blended (ttf_freesans,
							 fps, white);

  SDL_Texture* message = SDL_CreateTextureFromSurface (main_renderer, surface_message);
  SDL_FreeSurface (surface_message);

  struct coordinates blit_origin = {.x = 0,
				    .y = 0};
  int blit_height = 0;
  int blit_width = 0;
  SDL_QueryTexture (message, NULL, NULL,
		    &blit_width, &blit_height);

  blit (main_renderer, blit_origin, blit_width,
	blit_height, message);

  SDL_Rect message_rect = {.x = 0,
			   .y = 0,
			   .w = 150,
			   .h = 150};

  SDL_RenderCopy (main_renderer, message, NULL, &message_rect);
  SDL_DestroyTexture (message);
}

void
blit_rect (SDL_Renderer* renderer, SDL_Color color, SDL_Rect rectangle)
{
  SDL_RenderSetViewport (renderer, NULL);
  SDL_SetRenderDrawColor (renderer, color.r, color.g, color.b, color.a);
  SDL_RenderFillRect (renderer, &rectangle);
}
