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
 * Copyright (C) 2016 Loup Fourment <pierre.gillet+factorywars@linuxw.info>
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
 * display_map.h is the header of display_map.cpp
 */

#pragma once

#include "save.h"
#include "structures.h"
#include <stdio.h>
#include <string>
#include "gui.h"
#include <stdio.h>
#include "config.h"
#include "display_item.h"

/** load_biomes fills the current table with textures
  */
void load_biomes (SDL_Renderer** Renderer, SDL_Texture** table);

/** display_background displays the map
  * it takes the save path file
  * and a table with biomes
  */ 
void display_background (SDL_Renderer** Renderer, std::string path, SDL_Texture** table_biomes, SDL_Texture** table_items, struct coordinates screen_origin);
