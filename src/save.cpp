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
 * save.cpp provide functions to read and write the save file
 */

#include "save.h"

int
get_surface_item (struct coordinates chunk_coordinates, struct coordinates square_coordinates)
{
  regex_t regex;
  int reti;
  const unsigned int REGEX_STR_SIZE = 512;
  char regex_str[REGEX_STR_SIZE];

  const unsigned int LINE_SIZE = 512;
  const unsigned int COORDINATES_STR_SIZE = 20;
  char line[LINE_SIZE], chunk_coordinates_str[COORDINATES_STR_SIZE];
  bool matched(false);

  find_chunk_line_in_file (chunk_coordinates, line, LINE_SIZE, "save");
  if (line == NULL)
    {
      // Not matched
      return 0;
    }
  // Else: Matched !
  // We need to ignore the chunk coordinates because it could be the same as the square_coordinates
  int chunk_coordinates_len = strlen (coordinates_to_string (chunk_coordinates, chunk_coordinates_str, COORDINATES_STR_SIZE)) + 1;
  
  coordinates_to_string (square_coordinates, regex_str, REGEX_STR_SIZE);
  
  reti = regcomp (&regex, regex_str, REG_EXTENDED);
  if (reti)
    {
      fprintf (stderr, "Could not compile regex\n");
      return 0;
    }

  regmatch_t regmatch[1];
  reti = regexec (&regex, line+(chunk_coordinates_len), 1, regmatch, 0);
  if (reti)
    {
      // Not Matched !
      return 0;
    }
  regfree (&regex);

  int spaces = 0;
  int value = 0;
  int pos = 0;
  std::string item_id = "";

  // We isolate the item_id
  for (int i = regmatch[0].rm_eo-1+chunk_coordinates_len; i>0; i--)
    {
      if (line[i] == ';')
	spaces = 0;
      if (line[i] == ' ')
	spaces++;
      if (spaces == 2)
	{
	  pos = i;
	  break;
	}
    }
  if (spaces != 2)
    return 0;

  for (int i = pos+1; line[i] != ' '; i++)
    {
      item_id += line[i];
    }

  return std::atoi(item_id.c_str ());
}

int
get_biome_id (struct coordinates chunk_coordinates)
{
  const unsigned int LINE_SIZE = 512;
  const unsigned int BIOME_ID_STR_SIZE = 4;
  char line[LINE_SIZE], biome_id_str[BIOME_ID_STR_SIZE];
  char* SAVE_FILE_PATH = "save";

  memset (line, 0, LINE_SIZE);
  memset (biome_id_str, 0, BIOME_ID_STR_SIZE);

  if (find_chunk_line_in_file (chunk_coordinates, line, LINE_SIZE, SAVE_FILE_PATH) == NULL)
    return 0;

  int spaces_number = 0;
  for (int i= 0; i < LINE_SIZE; i++)
    {
      if (line[i] == ' ')
	{
	  for (int j = i+1; line[j] != ' ' && j < LINE_SIZE; j++)
	    {
	      biome_id_str[j-(i+1)] = line[j];
	    }
	  break;
	}
    }

  return atoi (biome_id_str);
}

char*
find_chunk_line_in_file (struct coordinates chunk_coordinates, char* dst, size_t n, char* file_path)
{
  const unsigned int LINE_SIZE = n;
  const unsigned int REGEX_STR_SIZE = 14;
    
  char line[LINE_SIZE], regex_str[REGEX_STR_SIZE], coordinates_str[REGEX_STR_SIZE];

  // Filling regex_str
  memset (regex_str, 0, REGEX_STR_SIZE);
  strncat (regex_str, "^", REGEX_STR_SIZE);
  coordinates_to_string (chunk_coordinates, coordinates_str, REGEX_STR_SIZE);
  strncat (regex_str, coordinates_str, REGEX_STR_SIZE);

  FILE *file = fopen (file_path, "r");
  regex_t regex;
  int reti;
  int matched = 0;

  reti = regcomp (&regex, regex_str, REG_EXTENDED|REG_NOSUB);
  if (reti)
    {
      fprintf (stderr, "Could not compile regex\n");
      return NULL;
    }
  
  while (fgets (line, LINE_SIZE, file) != NULL)
    {
      reti = regexec (&regex, line, 0, NULL, 0);
      if (!reti)
  	{
	  matched = 1;
  	  break;
  	}
      else
	regerror (reti, &regex, line, LINE_SIZE);
    }
  regfree (&regex);
  fclose (file);

  // We breaked only if we reached EOF or if the patern matched
  if (!matched)
    return NULL;
  else
    {
      strncpy (dst, line, n);
      return dst;
    }
}

int
set_surface_item (struct coordinates chunk_coordinates, struct coordinates square_coordinates, int item_id)
{
  const unsigned int LINE_SIZE = 512;
  const unsigned int REGEX_STR_SIZE = 512;
  const unsigned int COORDINATES_STR_SIZE = 20;
  char* SAVE_FILE_PATH = "save";
  
  char line[LINE_SIZE], tmp_line[LINE_SIZE],
    chunk_coordinates_str[COORDINATES_STR_SIZE],
    square_coordinates_str[COORDINATES_STR_SIZE];

  memset (line, 0, LINE_SIZE);
  memset (tmp_line, 0, LINE_SIZE);
  if (find_chunk_line_in_file (chunk_coordinates, line, LINE_SIZE, SAVE_FILE_PATH) == NULL)
    return 0;

  // We need to delete the chunk coordinates and the biome_id from the line to avoid
  // matching the chunk coordinates instead of the square coordinates and because we don’t need the biome_id
  int chunk_coordinates_str_len = strlen (coordinates_to_string (chunk_coordinates, chunk_coordinates_str, COORDINATES_STR_SIZE)) + 1;
  strncpy (tmp_line, line+chunk_coordinates_str_len, LINE_SIZE);

  // Then we delete the biome_id
  for (int i = 0; i < LINE_SIZE; i++)
    {
      if (tmp_line[i] == ' ')
	{
	  strncpy (tmp_line, tmp_line+i+1, LINE_SIZE);
	  break;
	}
    }

  // We search the square_coordinates with regexp
  regex_t regex;
  int reti;
  char regex_str[REGEX_STR_SIZE];
  memset (regex_str, 0, REGEX_STR_SIZE);
  coordinates_to_string (square_coordinates, square_coordinates_str, COORDINATES_STR_SIZE);
  strncpy (regex_str, square_coordinates_str, REGEX_STR_SIZE);

  reti = regcomp (&regex, regex_str, REG_EXTENDED);
  if (reti)
    {
      fprintf (stderr, "Could not compile regex\n");
      return 0;
    }
  
  regmatch_t regmatch[1];
  reti = regexec (&regex, tmp_line, 1, regmatch, 0);
  regfree (&regex);
  memset (regex_str, 0, REGEX_STR_SIZE);

  snprintf (regex_str, REGEX_STR_SIZE, "([^;]|^)%d[^;]", item_id);
  if (reti)
    {
      // Not Matched !
      // we search the provided item_id
      reti = regcomp (&regex, regex_str, REG_EXTENDED);
      if (reti)
	{
	  fprintf (stderr, "Could not compile regex\n");
	  return 0;
	}
      reti = regexec (&regex, tmp_line, 1, regmatch, 0);
      regfree (&regex);
      if (reti)
	{
	  // item_id not found
	  // so we add it at the end of line with square_coordinates
	  memset (tmp_line, 0, LINE_SIZE);
	  snprintf (tmp_line, LINE_SIZE, " %d %s", item_id, square_coordinates_str);

	  // We delete the “\n” at the end of the line
	  line[strlen (line) - 1] = 0;
	  strncat (line, tmp_line, LINE_SIZE);
	  printf ("line: %s\n", line);
	}
      else
	{
	  // item id found
	  // we add the square coordinates and a space
	  //   just after the item_id and a space
	  char right_part_of_line[LINE_SIZE], left_part_of_line[LINE_SIZE];

	  memset (right_part_of_line, 0, LINE_SIZE);
	  memset (left_part_of_line, 0, LINE_SIZE);

	  printf ("rm_so: %d, rm_eo: %d\n", regmatch[0].rm_so, regmatch[0].rm_eo);

	  strncpy (right_part_of_line, line+regmatch[0].rm_eo+1, LINE_SIZE);
	  for (int i = 0; i <= regmatch[0].rm_so; i++)
	    {
	      left_part_of_line[i] = line[i];
	    }

	  printf ("line: %s\n", line);
	  printf ("right_part_of_line: %s\n", right_part_of_line);
	  printf ("left_part_of_line: %s\n", left_part_of_line);

	  memset (line, 0, strlen (line));
	  strncpy (line, left_part_of_line, LINE_SIZE);
	  snprintf (tmp_line, LINE_SIZE, " %s", square_coordinates_str);
	  strncat (line, tmp_line, LINE_SIZE);
	  strncat (line, right_part_of_line, LINE_SIZE);

	  printf ("line: %s\n", line);
	  printf ("right_part_of_line: %s\n", right_part_of_line);
	  printf ("left_part_of_line: %s\n", left_part_of_line);
	}
    }
  else
    {
      //   If they exist, we search for a semicolon before and after the coordinates
      //     if a semicolon is found before or after the coordinates without 2 spaces with no semicolon between them
      //       we just delete the coordinates
      //     else we delete the coordinates and the item_id
      //
      // Then, we search for the item_id,
      //   If it does not exist, we add at the end of the line the item id, a space and the square coordinates
      //   If it is already present in the line, we add the square coordinates and a space just after the item_id and a space
      printf ("line: %sMatched, rm_so: %d, rm_eo: %d\n", line, regmatch[0].rm_so, regmatch[0].rm_eo);
    }
  
  // Algorithm:
  // search by regexp the square_coordinates  
  
  return 1;
}

char*
coordinates_to_string (struct coordinates coordinates, char *dst, size_t dst_size)
{
  const unsigned int STR_SIZE = dst_size;
  char str[STR_SIZE], tmp_str[STR_SIZE];
  memset (str, 0, STR_SIZE);
  memset (tmp_str, 0, STR_SIZE);

  snprintf (tmp_str, STR_SIZE, "%d", coordinates.x);
  strncat (str, tmp_str, STR_SIZE);
  strncat (str, ";", STR_SIZE);
  memset (tmp_str, 0, STR_SIZE);
  snprintf (tmp_str, STR_SIZE, "%d", coordinates.y);
  strncat (str, tmp_str, STR_SIZE);

  strncpy (dst, str, dst_size);
  return dst;
}
