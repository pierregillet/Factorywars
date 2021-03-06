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
 * display_item.cpp contains the function displaying the items on the map.
 */

#include "display_item.h"

void
display_items (SDL_Renderer** Renderer,
	       SDL_Texture* textures[][10],
	       struct coordinates screen_origin,
	       struct chunk_info chunk,
	       int l,
	       int m)
{

  int item_id, blit_x, blit_y;
  struct coordinates bliting_coords;
  SDL_Texture* item_texture;
  
  if (chunk.biome_id == -1)
    return;

  for (int i(15) ; i >= 0 ; i--)
    {
      for (int j(0) ; j < 16 ; j++)
	{
	  item_id = chunk.squares[i][j];
	  if (item_id > 0 && item_id < 4)
	    {
	      blit_x = (item_id == 1)? 48 :24;
	      blit_y = (item_id == 1)? 48 :24;
	      
	      bliting_coords = {.x = (int) ((m - screen_origin.x % 384) + 24 * i - blit_x / 4),
				.y = (int) ((l - screen_origin.y % 384) + 24 * j - blit_y / 2)};
	      
	      item_texture = textures[2][item_id];
	      blit (*Renderer, bliting_coords, blit_x, blit_y, item_texture);
	    }
	}
  }
}
