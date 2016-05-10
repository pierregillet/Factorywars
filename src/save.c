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

regmatch_t
get_item_id_pos_by_square_coordinates (struct coordinates chunk_coordinates,
		 struct coordinates square_coordinates, const char* save_file_path)
{
  const unsigned int LINE_SIZE = 512;

  char line[LINE_SIZE];

  regmatch_t item_id_pos;
  item_id_pos.rm_so = 0;
  item_id_pos.rm_eo = 0;

  find_chunk_line_in_file (chunk_coordinates, line, LINE_SIZE, save_file_path);
  if (line == NULL)
    {
      /* Chunk coordinates doesn’t exist */
      return item_id_pos;
    }


  regmatch_t square_coordinates_pos;
  square_coordinates_pos = find_square_coordinates_pos (chunk_coordinates,
  							square_coordinates,
  							save_file_path);
  if (square_coordinates_pos.rm_so == 0 && square_coordinates_pos.rm_eo == 0)
    {
      return item_id_pos;
    }
  

  int spaces = 0;
  int item_id_beg, item_id_end;

  /* Finding the position of the beginning of the item id */
  for (int i = square_coordinates_pos.rm_eo - 1; i > 0; i--)
    {
      if (line[i] == ';')
  	spaces = 0;
      if (line[i] == ' ')
  	spaces++;
      if (spaces == 1)
  	item_id_end = i - 1;
      else if (spaces == 2)
  	{
  	  item_id_beg = i + 1;
  	  break;
  	}
    }

  /* Finding the position of the end of the item id */
  /* We start at the beginning of the item id */
  item_id_end = item_id_beg;
  for (int i = item_id_beg; line[i] != ' '; i++)
    item_id_end++;
  /* line[item_id_end] points to the character after the end of item id */
  item_id_end--;

  item_id_pos.rm_so = item_id_beg;
  item_id_pos.rm_eo = item_id_end;

  return item_id_pos;
}

int
get_biome_id (struct coordinates chunk_coordinates, const char* save_file_path)
{
  const unsigned int LINE_SIZE = 512;
  const unsigned int BIOME_ID_STR_SIZE = 4;
  char line[LINE_SIZE], biome_id_str[BIOME_ID_STR_SIZE];

  if (find_chunk_line_in_file (chunk_coordinates,
			       line, LINE_SIZE, save_file_path) == NULL)
    return -1;

  for (unsigned int i= 0; i < LINE_SIZE; i++)
    {
      if (line[i] == ' ')
	{
	  for (unsigned int j = i+1; line[j] != ' ' && j < LINE_SIZE; j++)
	    biome_id_str[j-(i+1)] = line[j];
	  break;
	}
    }

  return atoi (biome_id_str);
}

int
set_surface_item (struct coordinates chunk_coordinates,
		  struct coordinates square_coordinates, int item_id,
		  const char* save_file_path)
{
  const unsigned int LINE_SIZE = 512;
  const unsigned int REGEX_STR_SIZE = 512;
  const unsigned int COORDINATES_STR_SIZE = 20;
  
  char line[LINE_SIZE], tmp_line[LINE_SIZE],
    square_coordinates_str[COORDINATES_STR_SIZE];

  line[0] = '\0';
  tmp_line[0] = '\0';

  /* If it’s true, the chunk line does not exist */
  /* Si c’est vrai, la ligne décrivant le chunk n’existe pas */
  if (find_chunk_line_in_file (chunk_coordinates, line,
			       LINE_SIZE, save_file_path) == NULL)
    {
      fprintf (stderr, "Could not find the chunk line in the save file.\n");
      return 0;
    }

  int line_number;
  line_number = find_line_number_using_chunk_coordinates (chunk_coordinates,
							  save_file_path);

  /* We need to delete the chunk coordinates and the biome_id from the line */
  /* to avoid matching the chunk coordinates instead of the square */
  /* coordinates and because we don’t need the biome_id */
  int len_of_deleted_part_of_line;
  len_of_deleted_part_of_line = get_len_of_chunk_and_biome (chunk_coordinates,
							    save_file_path);

  strncpy (tmp_line, line + len_of_deleted_part_of_line, LINE_SIZE);

  int reti;
  regmatch_t regmatch;


  regmatch = find_square_coordinates_pos (chunk_coordinates,
					  square_coordinates,
					  save_file_path);
  if (regmatch.rm_so == -1)
    reti = 1;

  regex_t regex;
  char regex_str[REGEX_STR_SIZE];
  regex_str[0] = '\0';

  /* We fill the square_coordinates_str */
  coordinates_to_string (square_coordinates, square_coordinates_str, COORDINATES_STR_SIZE);

  if (reti)
    {
      /* Square_coordinates Not Matched ! */
      /* we search the provided item_id */
      /* Should use get_item_id_by_square_coordinates */
      snprintf (regex_str, REGEX_STR_SIZE, "([^;]|^)%d[^;]", item_id);
      reti = regcomp (&regex, regex_str, REG_EXTENDED);
      if (reti)
	{
	  fprintf (stderr, "Could not compile regex\n");
	  return 2;
	}

      reti = regexec (&regex, tmp_line, 1, &regmatch, 0);
      regfree (&regex);

      if (reti)
	{
	  /* item_id not found */
	  /* so we add it at the end of line with square_coordinates */
	  tmp_line[0] = '\0';

	  snprintf (tmp_line, LINE_SIZE, " %d %s", item_id,
		    square_coordinates_str);

	  /* We delete the “\n” at the end of the line */
	  line[strlen (line) - 1] = 0;

	  /* We add the result at the end of the line variable */
	  strncat (line, tmp_line, LINE_SIZE);

	  /* We add a “\n” at the end of the new line */
	  strncat (line, "\n", LINE_SIZE);

	  /* And we add it in the file */
	  insert_line_in_file (line, strlen (line), line_number,
	  		       save_file_path, 1);
	}
      else
	{
	  /* item id found */
	  /* we add the square coordinates and a space just after the item_id */
	  /* and a space */
	  char right_part_of_line[LINE_SIZE], left_part_of_line[LINE_SIZE];

	  right_part_of_line[0] = '\0';
	  left_part_of_line[0] = '\0';

	  strncpy (right_part_of_line,
		   line+regmatch.rm_eo + len_of_deleted_part_of_line,
		   LINE_SIZE);

	  int len_of_item_id = regmatch.rm_eo - 1 - (regmatch.rm_so + 1);
	  int left_part_of_line_length = regmatch.rm_so;
	  left_part_of_line_length += len_of_deleted_part_of_line;
	  left_part_of_line_length += len_of_item_id + 1;

	  strncpy (left_part_of_line, line, left_part_of_line_length);
	  
	  line[0] = '\0';
	  strncpy (line, left_part_of_line, LINE_SIZE);
	  
	  snprintf (tmp_line, LINE_SIZE, " %s ", square_coordinates_str);
	  strncat (line, tmp_line, LINE_SIZE);
	  strncat (line, right_part_of_line, LINE_SIZE);

	  // We write the line to the save file
	  insert_line_in_file (line, strlen (line), line_number,
			       save_file_path, 1);
	}
    }
  else
    {
      /* If they exist, we search for a semicolon before and after the coordinates */
      /* if a semicolon is found before or after the coordinates without 2 spaces with no semicolon between them */
      /* we just delete the coordinates */
      /* else we delete the coordinates and the item_id */
      
      /* Then, we search for the item_id, */
      /*   If it does not exist, we add at the end of the line the item id, a space and the square coordinates */
      /*   If it is already present in the line, we add the square coordinates and a space just after the item_id and a space */
      int delete_coordinates_and_item_id = 1;
      int match_beg = regmatch.rm_so + len_of_deleted_part_of_line;
      int match_end = regmatch.rm_eo + len_of_deleted_part_of_line;
      int spaces_number = 0;
      int beg_of_item_id;

      /* We search backward */
      for (int i = match_beg; i > 0; i--)
	{
	  if (spaces_number == 2)
	    {
	      beg_of_item_id = i + 2;
	      break;
	    }

	  switch (line[i])
	    {
	    case ' ':
	      spaces_number++;
	      break;
	    case ';':
	      delete_coordinates_and_item_id = 0;
	      break;
	    }
	}

      spaces_number = 0;

      /* if a semicolon was not found we search forward */
      if (delete_coordinates_and_item_id)
	{
	  for (unsigned int i = match_end; i < LINE_SIZE; i++)
	    {
	      if (spaces_number == 2)
		  break;

	      switch (line[i])
		{
		case ' ':
		  spaces_number++;
		  break;
		case ';':
		  delete_coordinates_and_item_id = 0;
		  break;
		}
	    }
	}
      if (delete_coordinates_and_item_id)
	{
	  strncpy (line + beg_of_item_id, line + match_end + 1, LINE_SIZE);
	}
      else
	{
	  strncpy (line + match_beg, line + match_end + 1, LINE_SIZE);
	}

      /* Now we need to search if the item_id exist */
      regmatch = get_item_id_pos_using_item_id (chunk_coordinates, item_id,
						save_file_path);

      /* We need to delete the \n at the end of the line */
      line [strlen (line) - 1] = 0;

      if (regmatch.rm_so == 0 && regmatch.rm_eo == 0)
	{
	  /* The item id does not exist */
	  /* We add the item_id a space and the square_coordinates */
	  /* at the end of line */
	  snprintf (tmp_line, LINE_SIZE, "%s %d %s\n", line, item_id, square_coordinates_str);
	  insert_line_in_file (tmp_line, strlen (tmp_line), line_number, save_file_path, 1);
	}
      else
	{
	  /* The item id exists */
	  /* We just add a space and the coordinates right after it */
	  tmp_line[0] = 0;
	  strncat (tmp_line, line, regmatch.rm_eo + 1);
	  snprintf (tmp_line + strlen(tmp_line), LINE_SIZE - strlen(tmp_line),
		    " %s", square_coordinates_str);
	  strncat (tmp_line, line + regmatch.rm_eo + 1, LINE_SIZE);

	  /* We add a newline character at the end of the line */
	  tmp_line[strlen (tmp_line)] = '\n';

	  insert_line_in_file (tmp_line, strlen(tmp_line), line_number, save_file_path, 1);
	}
    }
  
  return 1;
}

int
get_surface_item (struct coordinates chunk_coordinates,
		  struct coordinates square_coordinates, const char* save_file_path)
{
  const int ITEM_ID_LEN = 20;
  const int LINE_SIZE = 512;

  char item_id_str[ITEM_ID_LEN], line[LINE_SIZE];

  /* We fill the variable line with the line describing the chunk */
  find_chunk_line_in_file (chunk_coordinates, line, LINE_SIZE, save_file_path);

  /* If the line does not exist we return -1 */
  if (line == NULL)
    return -1;

  regmatch_t regmatch;
  regmatch = get_item_id_pos_by_square_coordinates (chunk_coordinates,
						    square_coordinates,
						    save_file_path);
  if (regmatch.rm_so == 0 && regmatch.rm_eo == 0)
    return -1;

  int len_of_item_id_str = regmatch.rm_eo - regmatch.rm_so + 1;
  strncpy (item_id_str, line + regmatch.rm_so, len_of_item_id_str);
  if (len_of_item_id_str + 1 <= ITEM_ID_LEN)
    item_id_str[len_of_item_id_str + 1] = 0;
  else
    {
      fprintf (stderr, "Need to increase the ITEM_ID_LEN limit in ");
      fprintf (stderr, "get_surface_item function\n");
      return -1;
    }

  return atoi (item_id_str);
}

regmatch_t
find_square_coordinates_pos (struct coordinates chunk_coordinates,
			     struct coordinates square_coordinates,
			     const char* save_file_path)
{
  const int COORDINATES_STR_SIZE = 20;
  const int REGEX_STR_SIZE = COORDINATES_STR_SIZE;
  const int LINE_SIZE = 512;

  char regex_str[REGEX_STR_SIZE], line[LINE_SIZE];
  regex_t regex;
  int reti;

  regmatch_t square_coordinates_pos;
  square_coordinates_pos.rm_so = 0;
  square_coordinates_pos.rm_eo = 0;
  
  coordinates_to_string (square_coordinates, regex_str, REGEX_STR_SIZE);
  find_chunk_line_in_file (chunk_coordinates, line, LINE_SIZE, save_file_path);

  /* We need to ignore the chunk coordinates because it could be the same 
     as the square coordinates. We also ignore the biome id. */
  int number_of_deleted_char, number_of_spaces;
  number_of_spaces = 0;
    
  for (int i = 0; i < LINE_SIZE; i++)
    {
      if (line[i] == ' ')
	number_of_spaces++;
      if (number_of_spaces == 2)
	{
	  number_of_deleted_char = i + 1;
	  break;
	}
    }
  
  reti = regcomp (&regex, regex_str, REG_EXTENDED);
  if (reti)
    {
      fprintf (stderr, "Could not compile regex\n");
      return square_coordinates_pos;
    }

  regmatch_t regmatch[1];
  reti = regexec (&regex, line + number_of_deleted_char, 1, regmatch, 0);
  if (reti)
    {
      // Not Matched !
      return square_coordinates_pos;
    }
  regfree (&regex);

  square_coordinates_pos.rm_so = regmatch[0].rm_so + number_of_deleted_char;
  square_coordinates_pos.rm_eo = regmatch[0].rm_eo + number_of_deleted_char;
  
  return square_coordinates_pos;
}

int
get_len_of_chunk_and_biome (struct coordinates chunk_coordinates,
			    const char* save_file_path)
{
  const int LINE_SIZE = 512;

  int number_of_spaces = 0;
  int len_of_chunk_and_biome;
  char line[LINE_SIZE];
  find_chunk_line_in_file (chunk_coordinates, line, LINE_SIZE, save_file_path);

  for (int i = 0; i < LINE_SIZE; i++)
    {
      if (line[i] == ' ')
	number_of_spaces++;
      if (number_of_spaces == 2)
	len_of_chunk_and_biome = i;
    }

  return len_of_chunk_and_biome;
}

regmatch_t get_item_id_pos_using_item_id (struct coordinates chunk_coordinates,
					  int item_id, const char* save_file_path)
{
  const int REGEX_STR_SIZE = 32;
  const int LINE_SIZE = 512;

  char regex_str[REGEX_STR_SIZE], line[LINE_SIZE];

  regmatch_t regmatch;
  regex_t regex;
  int reti;

  find_chunk_line_in_file (chunk_coordinates, line, LINE_SIZE, save_file_path);

  int len_of_deleted_part_of_line;
  len_of_deleted_part_of_line = get_len_of_chunk_and_biome (chunk_coordinates,
							    save_file_path);
  strncpy (line, line + len_of_deleted_part_of_line, LINE_SIZE);

  snprintf (regex_str, REGEX_STR_SIZE, " %d ", item_id);

  reti = regcomp (&regex, regex_str, REG_EXTENDED);
  if (reti)
    {
      fprintf (stderr, "Couldn’t compile regex\n");
      return regmatch;
    }

  reti = regexec (&regex, line, 1, &regmatch, 0);
  if (reti)
    {
      /* Not found */
      regmatch.rm_so = 0;
      regmatch.rm_eo = 0;
      return regmatch;
    }

  /* We add the deleted part of line length and we delete 
     spaces between the item id */
  regmatch.rm_so += len_of_deleted_part_of_line + 1;
  regmatch.rm_eo += len_of_deleted_part_of_line - 2;

  return regmatch;
}

int
set_biome_id (struct coordinates chunk_coordinates, int biome_id, const char* save_file_path)
{
  const int LINE_SIZE = 512;
  const int BIOME_ID_STR_SIZE = 20;

  char line[LINE_SIZE], tmp_line[LINE_SIZE], biome_id_str[BIOME_ID_STR_SIZE];
  line[0] = 0;
  tmp_line[0] = 0;
  biome_id_str[0] = 0;

  /* Finding the line */
  find_chunk_line_in_file (chunk_coordinates, line, LINE_SIZE, save_file_path);
  if (line == NULL)
    return 0;

  /* Store the biome id as a string in biome_id_str */
  snprintf (biome_id_str, BIOME_ID_STR_SIZE, "%d", biome_id);

  /* Copy the begining of the string until we reached the */
  /* first space (it’s included) */
  int beg_of_biome_id = 0;
  for (int i = 0; i < LINE_SIZE; i++)
    {
      if (line[i] == ' ')
	{
	  strncpy (tmp_line, line, i + 1);
	  tmp_line[i + 1] = 0;
	  beg_of_biome_id = i + 1;
	  break;
	}
    }

  /* Concatenate the string with the biome id */
  strncat (tmp_line, biome_id_str, BIOME_ID_STR_SIZE);

  /* Then we add the rest of the line without the old biome id */
  for (int i = beg_of_biome_id; i < LINE_SIZE - beg_of_biome_id; i++)
  {
    if (line[i] == ' ')
      {
	strncat (tmp_line, line + i, LINE_SIZE - i);
	break;
      }
  }

  /* Write the new line with the good biome id in the save file */
  int line_number;
  line_number = find_line_number_using_chunk_coordinates (chunk_coordinates,
							  save_file_path);
  insert_line_in_file (tmp_line, strlen (tmp_line), line_number, save_file_path, 1);

  return 1;
}

int
create_chunk_line (struct coordinates chunk_coordinates,
		   int biome_id,
		   const char* save_file_path)
{
  const int LINE_SIZE = 512;
  const int COORDINATES_STR_SIZE = 20;
  char line[LINE_SIZE], coordinates_str[COORDINATES_STR_SIZE], *ret;

  ret = find_chunk_line_in_file (chunk_coordinates, line,
				 LINE_SIZE, save_file_path);
  if (ret != NULL)
    return 0;

  /* On construit la ligne que l’on va écrire */
  /* We build the line we will write */
  line[0] = '\0';
  coordinates_to_string (chunk_coordinates, coordinates_str, COORDINATES_STR_SIZE);
  snprintf (line, LINE_SIZE, "%s %d\n", coordinates_str, biome_id);

  /* On écrit la ligne dans le fichier */
  /* We write the line in the file */
  FILE *file = fopen (save_file_path, "a");
  fprintf (file, "%s", line);
  fclose (file);

  return 1;
}

struct coordinates
get_chunk_coordinates_from_player_movement (struct coordinates player_offset)
{
  const int NUMBER_OF_SQUARE_PER_ROW = 16;
  const int SQUARE_WIDTH = 24;

  struct coordinates chunk_coordinates;
  chunk_coordinates.x =
    player_offset.x / NUMBER_OF_SQUARE_PER_ROW * SQUARE_WIDTH;
  chunk_coordinates.y =
    player_offset.y / NUMBER_OF_SQUARE_PER_ROW * SQUARE_WIDTH;

  return chunk_coordinates;
}
