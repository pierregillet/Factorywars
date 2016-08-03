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
 * along with factorywars.  If not, see <http://www.gnu.org/licenses/
 *
 * @section DESCRIPTION
 *
 * save.cpp provide functions to read and write save files.
 */

#include "map.h"

int
get_biome_id (struct coordinates chunk_coordinates, Map* map)
{
  int chunk_number, biome_id;
  chunk_number = find_chunk_using_chunk_coordinates (chunk_coordinates, map);

  if (chunk_number == -1)
    return -1;

  biome_id = map->chunks[chunk_number]->squares[0]->floor;
  return biome_id;
}

int set_surface_item (struct coordinates chunk_coordinates,
		      struct coordinates square_coordinates, int item_id,
		      Map* map)
{
  int chunk_number, square_number;

  chunk_number = find_chunk_using_chunk_coordinates (chunk_coordinates, map);
  if (chunk_number == -1)
    return 0;

  square_number = find_square_using_square_coordinates (chunk_coordinates,
							square_coordinates,
							map);
  if (square_number == -1)
    return 0;

  map->chunks[chunk_number]->squares[square_number]->item = item_id;

  return 1;
}

int
get_surface_item (struct coordinates chunk_coordinates,
		  struct coordinates square_coordinates,
		  Map* map)
{
  int chunk_number, surface_item, square_number;

  chunk_number = find_chunk_using_chunk_coordinates (chunk_coordinates, map);
  if (chunk_number == -1)
    return -1;

  square_number = find_square_using_square_coordinates (chunk_coordinates,
							square_coordinates,
							map);
  if (square_number == -1)
    return -1;

  surface_item = map->chunks[chunk_number]->squares[square_number]->item;

  return surface_item;
}

struct coordinates
get_chunk_coordinates_from_player_movement (struct coordinates player_offset)
{
  /* We initialize the constants */
  const int NUMBER_OF_SQUARE_PER_ROW = 16;
  const int SQUARE_WIDTH = 24;

  const int chunk_width = NUMBER_OF_SQUARE_PER_ROW * SQUARE_WIDTH;

  struct coordinates center_chunk_coordinates;

  center_chunk_coordinates.y =
    player_offset.y / chunk_width;

  center_chunk_coordinates.x =
    player_offset.x / chunk_width;

  return center_chunk_coordinates;
}

struct chunk_info
get_chunk_info (struct coordinates chunk_coordinates,
		Map* map)
{
  int chunk_number, item;
  struct coordinates square;

  struct chunk_info chunk_info;

  chunk_info.chunk = chunk_coordinates;
  chunk_info.biome_id = -1;

  chunk_number = find_chunk_using_chunk_coordinates (chunk_coordinates, map);
  if (chunk_number == -1)
    return chunk_info;

  for (int i = 0; i < map->chunks[chunk_number]->n_squares; i++)
    {
      square.x = map->chunks[chunk_number]->squares[i]->x;
      square.y = map->chunks[chunk_number]->squares[i]->y;

      if (square.x >= 16 || square.y >= 16)
	return chunk_info;

      item = map->chunks[chunk_number]->squares[i]->item;
      chunk_info.squares[square.x][square.y] = item;
    }

  chunk_info.biome_id = get_biome_id (chunk_coordinates, map);
      
  return chunk_info;
}

int
read_save_file (Map* map, const char* save_file_path)
{
  Map *tmp_map;
  Chunk **chunks;
  Square ***squares;

  uint8_t *buffer;
  buffer = (uint8_t*) malloc (sizeof (uint8_t));

  char c;
  int n = 0;

  int save_file = open (save_file_path, O_RDONLY);
  if (save_file == -1)
    return 0;

  while (read (save_file, &c, sizeof (char)) > 0)
    {
      buffer[n] = c;

      n++;
      buffer = (uint8_t*) realloc (buffer, sizeof (uint8_t) * (n + 1));
      if (buffer == NULL)
	{
	  fprintf (stderr, "Error while reading save file.\n");
	  fprintf (stderr, "It is not possible to reallocate memory for the buffer.\n");
	  return 0;
	}
    }
  
  tmp_map = map__unpack (NULL, n, buffer);
  if (tmp_map == NULL)
    {
      fprintf (stderr, "Error while reading the file\n");
      free (buffer);
      return 0;
    }

  /* On copie tmp_map dans map */
  /* We copy tmp_map in map */
  map->n_chunks = tmp_map->n_chunks;

  squares = (Square***) malloc (sizeof (Square**) * map->n_chunks);
  chunks = (Chunk**) malloc (sizeof (Chunk*) * map->n_chunks);
  for (int i = 0; i < map->n_chunks; i++)
    {
      chunks[i] = (Chunk*) malloc (sizeof (Chunk));
      chunk__init (chunks[i]);

      chunks[i]->x = tmp_map->chunks[i]->x;
      chunks[i]->y = tmp_map->chunks[i]->y;

      chunks[i]->n_squares = tmp_map->chunks[i]->n_squares;
      squares[i] = (Square**) malloc (sizeof (Square*) * chunks[i]->n_squares);

      for (int j = 0; j < chunks[i]->n_squares; j++)
	{
	  squares[i][j] = (Square*) malloc (sizeof (Square));
	  square__init (squares[i][j]);

	  squares[i][j]->x = tmp_map->chunks[i]->squares[j]->x;
	  squares[i][j]->y = tmp_map->chunks[i]->squares[j]->y;

	  squares[i][j]->floor = tmp_map->chunks[i]->squares[j]->floor;
	  squares[i][j]->item = tmp_map->chunks[i]->squares[j]->item;
	}

      chunks[i]->squares = squares[i];
    }

  map->chunks = chunks;

  map__free_unpacked (tmp_map, NULL);

  close (save_file);
  free (buffer);

  return 1;
}

int
save_to_file (Map* map, const char* save_file_path)
{
  unsigned long len = map__get_packed_size (map);

  uint8_t *buffer = (uint8_t*) malloc (len);
  if (buffer == NULL)
    return 0;

  map__pack (map, buffer);

  FILE *save_file = fopen (save_file_path, "w");
  if (save_file == NULL)
    {
      fprintf (stderr, "Error the save file cannot be opened.\n");
      return 0;
    }

  fwrite (buffer, len, 1, save_file);
  fclose (save_file);
  
  free (buffer);

  return 1;
}

void
free_map_struct (Map* map)
{
  for (int i = 0; i < map->n_chunks; i++)
    {
      for (int j = 0; j < map->chunks[i]->n_squares; j++)
	{
	  free (map->chunks[i]->squares[j]);
	}
      free (map->chunks[i]->squares);
      free (map->chunks[i]);
    }

  free (map->chunks);
}

int
find_chunk_using_chunk_coordinates (struct coordinates chunk_coordinates,
				    Map* map)
{
  int chunk_number;

  int chunk_found = 0;

  for (int i = 0; i < map->n_chunks; i++)
    {
      if (map->chunks[i]->x == chunk_coordinates.x)
	{
	  if (map->chunks[i]->y == chunk_coordinates.y)
	    {
	      chunk_number = i;
	      chunk_found = 1;
	    }
	}
    }
  if (!chunk_found)
    return -1;
  else
    return chunk_number;
}

int
find_square_using_square_coordinates (struct coordinates chunk_coordinates,
				      struct coordinates square_coordinates,
				      Map* map)
{
  int chunk_number = find_chunk_using_chunk_coordinates (chunk_coordinates,
							 map);

  if (chunk_number == -1)
    return -1;

  int square_number, square_found = 0;

  for (int i = 0; i < map->chunks[chunk_number]->n_squares; i++)
    {
      if (map->chunks[chunk_number]->squares[i]->x == square_coordinates.x)
	{
	  if (map->chunks[chunk_number]->squares[i]->y == square_coordinates.y)
	    {
	      square_number = i;
	      square_found = 1;
	    }
	}
    }

  if (!square_found)
    return -1;
  else
    return square_number;
}
