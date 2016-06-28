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
display_background (SDL_Renderer** Renderer,
		    Map* map,
		    SDL_Texture* textures[][10],
		    struct coordinates screen_origin,
		    const int screen_height,
		    const int screen_width)
{
  const int NUMBER_OF_SQUARE_PER_ROW = 16;
  const int SQUARE_WIDTH = 24;

  const int chunk_width = NUMBER_OF_SQUARE_PER_ROW * SQUARE_WIDTH;

  screen_origin.x = (screen_origin.x < 0)? 0 : screen_origin.x;
  screen_origin.y = (screen_origin.y < 0)? 0 : screen_origin.y;

  struct coordinates hero_coords;
  struct coordinates coords;
  int id;
  SDL_Texture* display_id;
  struct size temp;
  struct chunk_info current_chunk_info;

  for(int i(0) ; i < screen_height + screen_origin.y % chunk_width; i += chunk_width)
    {
      for(int j(0); j < screen_width + screen_origin.x % chunk_width ; j += chunk_width)
	{
	  hero_coords = {.x = (int) screen_origin.x + j,
			 .y = (int) screen_origin.y + i};
	  coords = get_chunk_coordinates_from_player_movement (hero_coords);
     
	  id = get_biome_id (coords, map);
	  if (id == -1 || id > 4)
	    {
	      id = 2;
	    }

	  display_id = textures[1][id];
	  
	  temp = {.x = (int) (j - screen_origin.x % chunk_width),
		  .y = (int) (i - screen_origin.y % chunk_width)};
	  

	  blit (Renderer, temp, chunk_width,
		chunk_width, display_id);
	  
	  current_chunk_info = get_chunk_info (coords, map);

	  if (current_chunk_info.biome_id == -1)
	    continue;

	  display_items (Renderer,
			 textures,
			 screen_origin,
			 current_chunk_info, i, j);
	}
    }
}  
