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
 * display_map.cpp contains the functions to display the map.
 */

#include "display_map.h"

void
display_background (SDL_Renderer* Renderer,
		    Map& map,
		    struct coordinates screen_origin,
		    const struct size screen_dimensions)
{
  const int NUMBER_OF_SQUARE_PER_ROW = 16;
  const int SQUARE_WIDTH = 24;

  const int chunk_width = NUMBER_OF_SQUARE_PER_ROW * SQUARE_WIDTH;

  screen_origin.x = (screen_origin.x < 0)? 0 : screen_origin.x;
  screen_origin.y = (screen_origin.y < 0)? 0 : screen_origin.y;

  struct coordinates hero_coords;
  struct coordinates coords;
  int id;
  SDL_Texture* current_displayed_texture;
  struct coordinates temp;
  struct chunk_info current_chunk_info;

  for(int i(0); i < screen_dimensions.y + screen_origin.y % chunk_width; i += chunk_width)
    {
      for(int j(0); j < screen_dimensions.x + screen_origin.x % chunk_width ; j += chunk_width)
	{
	  hero_coords = {.x = (int) screen_origin.x + j,
			 .y = (int) screen_origin.y + i};
	  coords = get_chunk_coordinates_from_player_movement (hero_coords);
     
	  current_displayed_texture = map.get_chunk_texture (coords);
	  
	  temp = {.x = (int) (j - screen_origin.x % chunk_width),
		  .y = (int) (i - screen_origin.y % chunk_width)};
	  

	  blit (Renderer, temp, chunk_width, chunk_width,
		current_displayed_texture);
	}
    }
}  
