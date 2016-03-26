/**
 * @file
 * @author Corentin Bocquillon <0x539@nybble.fr>
 * @author Pierre Gillet
 * @author Loup Fourment
 *
 * @section LICENSE
 *
 * Copyright (C) 2016 Corentin Bocquillon <0x539@nybble.fr>
 *
 * Copyright (C) 2016 Loup Fourment
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
 * along with factorywars.  If not, see <http://www.gnu.org/licenses/
 *
 * @section DESCRIPTION
 *
 * save.h is the header of save.cpp
 */

#include <stdio.h>
#include "structures.h"
#include <sys/types.h>
#include <string>
#include <string.h>
#include <regex.h>

/**
 * Get the surface item at given coordinates.
 * @param chunk_coordinates is the chunk coordinates.
 * @param square_coordinates is the square coordinates.
 * @return 0 if there is an error, 1 if not.
 */
int get_surface_item (struct coordinates chunk_coordinates, struct coordinates square_coordinates);

/**
 * Get the biome id of the given chunk.
 * @param chunk_coordinates is the coordinates of the chunk.
 * @return 0 if there is an error, 1 if not.
 */
int get_biome_id (struct coordinates chunk_coordinates);

/**
 * Set the surface item at given coordinates.
 * @param chunk_coordinates is the chunk coordinates.
 * @param square_coordinates is the square coordinates.
 * @param item_id is the item id.
 * @return 0 if there is an error, 1 if not.
 */
int set_surface_item (struct coordinates chunk_coordinates, struct coordinates square_coordinates, int item_id);

/**
 * Set the biome id for the given chunk.
 * @param chunk_coordinates is the chunk coordinates.
 * @param biome_id is the biome id.
 * @return 0 if there is an error, 1 if not.
 */
int set_biome_id (struct coordinates chunk_coordinates, int biome_id);

/**
 * Find the line which described the given chunk in a file.
 * @param chunk_coordinates is the coordinates of the chunk.
 * @param dst is a pointer to the variable where the line will be stored.
 * @param dst_size is the size of dst.
 * @param file_path is the path of the file where we want to find the chunk line.
 * @return A pointer to the line or NULL if there is an error.
 */
char* find_chunk_line_in_file (struct coordinates chunk_coordinates, char* dst, size_t dst_size, char* file_path);

/**
 * Translate a struct coordinates to a string.
 * @param coordinates is the struct coordinates to translate.
 * @param dst is the string where the coordinates string will be stored.
 * @param dst_size is the size of the string “dst”.
 * @return A pointer to the coordinates string.
 */
char* coordinates_to_string (struct coordinates coordinates, char* dst, size_t dst_size);
