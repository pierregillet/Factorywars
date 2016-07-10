/**
 * @file
 * @author Pierre Gillet <pierre.gillet+factorywars@linuxw.info>
 * @author Corentin Bocquillon <0x539@nybble.fr>
 *
 * @section LICENSE
 *
 * Copyright (C) 2016 Corentin Bocquillon
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
 * structures.h the header file containing most of the structures
 */

#pragma once

struct coordinates_l
{
  long x;
  long y;
};

struct coordinates
{
  int x;
  int y;
};

struct size
{
  int x;
  int y;
};

struct unsigned_size
{
  unsigned int x;
  unsigned int y;
};

struct map_coordinates
{
  struct coordinates chunk;
  struct coordinates square;
};

struct chunk_info
{
  struct coordinates chunk;
  int biome_id, squares[16][16];
};

struct directory_list
{
  char *dir_name;
  char last_modification[32];

  struct directory_list* prev;
  struct directory_list* next;
};
