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
 * save.h is the header of save.cpp
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "config.h"
#include "utils.h"
#include <sys/types.h>
#include <string.h>
#include <regex.h>
#include <math.h>

/**
 * Get the surface item at given coordinates.
 * @param chunk_coordinates is the chunk coordinates.
 * @param square_coordinates is the square coordinates.
 * @param save_file_path is the path to the save file.
 * @return -1 if there is an error, anything else if not.
 */
int get_surface_item (struct coordinates chunk_coordinates,
		      struct coordinates square_coordinates,
		      const char* save_file_path);

/**
 * Get the biome id of the given chunk.
 * @param chunk_coordinates is the coordinates of the chunk.
 * @param save_file_path is the path to the save file.
 * @return -1 if there is an error, anything else if not.
 */
int get_biome_id (struct coordinates chunk_coordinates, const char* save_file_path);

/**
 * Set the surface item at given coordinates.
 * @param chunk_coordinates is the chunk coordinates.
 * @param square_coordinates is the square coordinates.
 * @param item_id is the item id.
 * @param save_file_path is the path to the save file
 * @return 1 if there are no errors, 0 if the chunk line does not exist
 * and 2 for other errors.
 */
int set_surface_item (struct coordinates chunk_coordinates,
		      struct coordinates square_coordinates, int item_id,
		      const char* save_file_path);

/**
 * Set the biome id for the given chunk.
 * @param chunk_coordinates is the chunk coordinates.
 * @param biome_id is the biome id.
 * @param save_file_path it the path to the save file
 * @return 0 if there is an error, 1 if not.
 */
int set_biome_id (struct coordinates chunk_coordinates, int biome_id, const char* save_file_path);

/**
 * Get the position of the item id in the line describing the chunk.
 * @param chunk_coordinates are the coordinates of the chunk.
 * @param save_file_path is the path to the file.
 * @return the position of the id in the string or 0 for rm_so and rm_eo if
 * there is an error.
 */
regmatch_t
get_item_id_pos_by_square_coordinates (struct coordinates chunk_coordinates,
				       struct coordinates square_coordinates,
				       const char* save_file_path);


/**
 * Get the position of the item id by specifing which item_id we are 
 * searching for.
 * @param chunk_coordinates are the coordinates of the chunk coordinates.
 * @param item_id is the item_id.
 * @param save_file_path is the path to the save file.
 * @return the position
 */
regmatch_t get_item_id_pos_using_item_id (struct coordinates chunk_coordinates,
					  int item_id, const char* save_file_path);

/**
 * Get the position of the square coordinates in the line describing the chunk.
 * @param chunk_coordinates are the coordinates of the chunk.
 * @param square_coordinates are the coordinates of the square.
 * @param save_file_path is the path to the save file.
 * @return the position of the square coordinates in the string or -1
 * for rm_so and rm_eo if it doesn’t exist.
 */
regmatch_t find_square_coordinates_pos (struct coordinates chunk_coordinates,
					struct coordinates square_coordinates,
					const char* save_file_path);

/**
 * Return the length of the chunk coordinates plus
 * biome id (the spaces are included).
 * @param chunk_coordinates are the chunk coordinates.
 * @param save_file_path is the path to the save file.
 * @return the length of the chunk coordinates plus biome_id.
 */
int get_len_of_chunk_and_biome (struct coordinates chunk_coordinates,
				const char* save_file_path);

/**
  * Create a line describing a new chunk in the save file.
  * @param chunk_coordinates is the chunk_coordinates.
  * @param biome_id is the biome id.
  * @param save_file_path is the path to the save file.
  * @return 0 if there is an error, 1 if not.
  */
int create_chunk_line (struct coordinates chunk_coordinates,
		       int biome_id,
		       const char* save_file_path);

/**
 * Get the chunk coordinates of the bottom left corner chunk for displaying it.
 * @param player_offset is the number of pixels from the left bottom corner
 * of the map.
 * @return the chunk coordinates of the bottom left corner chunk.
 */
struct coordinates
get_chunk_coordinates_from_player_movement (struct coordinates player_offset);

/**
 * Get the chunk info.
 *
 * @param chunk_coordiantes is the coordinates of the chunk.
 * @param save_file_path is the path to the save file.
 * @return the chunk inforamations in a structure chunk_info or a biome id of
 * -1 if there is an error.
 */
struct chunk_info get_chunk_info (struct coordinates chunk_coordinates,
				  const char* save_file_path);

