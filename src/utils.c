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
 * utils.cpp
 */

#include "utils.h"

char*
find_chunk_line_in_file (struct coordinates chunk_coordinates, char* dst,
			 size_t dst_size, const char* file_path)
{
  int line_number;
  line_number = find_line_number_using_chunk_coordinates (chunk_coordinates,
							  file_path);
  if (line_number == -1)
    return NULL;
  
  FILE *file = fopen (file_path, "r");
  if (file == NULL)
    return NULL;

  for (int i = 0; i <= line_number; i++)
    {
      fgets (dst, dst_size, file);
    }

  dst[strlen (dst) - 1] = '\0';

  fclose (file);
  return dst;
}

int
find_line_number_using_chunk_coordinates (struct coordinates chunk_coordinates, const char* file_path)
{
  const unsigned int LINE_SIZE = 512;
  const unsigned int COORDINATES_STR_SIZE = 14;

  int line_number = -1;
  int matched = 0;
  char line[LINE_SIZE], coordinates_str[COORDINATES_STR_SIZE];
  char tmp_line[LINE_SIZE], *token;

  coordinates_to_string (chunk_coordinates, coordinates_str, COORDINATES_STR_SIZE);

  FILE *file = fopen (file_path, "r");
  if (file == NULL)
    return -1;

  while (fgets (line, LINE_SIZE, file) != NULL)
    {
      strncpy (tmp_line, line, LINE_SIZE);
      token = strtok (tmp_line, " ");
      line_number++;
      if (strncmp (token, coordinates_str, COORDINATES_STR_SIZE) == 0)
  	{
	  matched = 1;
  	  break;
  	}
    }
  fclose (file);

  // We breaked only if we reached EOF or if the patern matched
  if (!matched)
    line_number = -1;

  return line_number;
}

char*
coordinates_to_string (struct coordinates coordinates, char *dst, size_t dst_size)
{
  const unsigned int STR_SIZE = dst_size;
  char str[STR_SIZE], tmp_str[STR_SIZE];
  memset (str, 0, STR_SIZE);
  memset (tmp_str, 0, STR_SIZE);

  snprintf (tmp_str, STR_SIZE, "%ld", coordinates.x);
  strncat (str, tmp_str, STR_SIZE);
  strncat (str, ";", STR_SIZE);
  memset (tmp_str, 0, STR_SIZE);
  snprintf (tmp_str, STR_SIZE, "%ld", coordinates.y);
  strncat (str, tmp_str, STR_SIZE);

  strncpy (dst, str, dst_size);
  return dst;
}

int
insert_line_in_file (char* line, int line_size, int position, const char* file_path, int replace)
{
  int pipe_read_file[2];

  if (pipe (pipe_read_file))
    {
      fprintf (stderr, "Pipe failed.\n");
      return 0;
    }

  write_file_to_pipe (file_path, pipe_read_file[1]);
  const int BUFFER_SIZE = 512;
  char buffer[BUFFER_SIZE];
  memset (buffer, 0, BUFFER_SIZE);

  FILE *file = fopen (file_path, "w");
  if (file == NULL)
    return 0;
  
  char c;
  int line_number = 0;

  // We write the beginning of the file
  while (line_number != position)
    {
      read (pipe_read_file[0], &c, sizeof (char));
      fwrite (&c, 1, sizeof (char), file);
      if (c == '\n')
	line_number++;
    }
  fwrite (line, line_size, sizeof (char), file);

  // If replace is set to true we ignore the next line
  if (replace)
    {
      do
	{
	  read (pipe_read_file[0], &c, sizeof (char));
	} while (c != '\n');
    }

  int flags = fcntl (pipe_read_file[0], F_GETFL, 0);
  fcntl (pipe_read_file[0], F_SETFL, flags | O_NONBLOCK);

  while (read (pipe_read_file[0], &c, sizeof (char)) > 0)
    {
      fwrite (&c, 1, sizeof (char), file);
    }

  fclose (file);
  return 1;
}


void
write_file_to_pipe (const char* file_path, int pipe)
{
  const int LINE_SIZE = 512;
  
  FILE *file = fopen (file_path, "r");
  if (file == NULL)
    return;

  char line[LINE_SIZE];
  
  while (fgets (line, LINE_SIZE, file) != NULL)
    {
      write_to_pipe (pipe, line);
    }

  fclose (file);
}

void
write_to_pipe (int file, const char* message)
{
  write (file, message, strlen (message));
}

int
read_pipe_until_null (char* buffer, size_t buf_size, int pipe)
{
  /* number of bytes */
  int n = 0;

  int flags = fcntl (pipe, F_GETFL, 0);
  fcntl (pipe, F_SETFL, flags | O_NONBLOCK);

  if (read (pipe, buffer, sizeof (char)) == -1)
    {
      fcntl (pipe, F_SETFL, flags);
      return n;
    }
  else
    n++;

  fcntl (pipe, F_SETFL, flags);

  for (unsigned int i = 1; i < buf_size - 1; i++)
    {
      read (pipe, buffer + i, sizeof (char));
      n++;

      if (buffer[i] == '\0')
	break;
    }

  return n;
}

struct coordinates
get_coordinates_from_string (const char* coordinates_str)
{
  unsigned int coordinates_str_len = strlen (coordinates_str);
  char coord1_str[coordinates_str_len];
  char coord2_str[coordinates_str_len];
  int semicolon = 0;
  int coord1_str_filled = 0;
  int coord2_str_filled = 0;
  struct coordinates ret;

  for (unsigned int i = 0; i < coordinates_str_len; i++)
    {
      if (coordinates_str[i] == ';')
	{
	  semicolon = 1;
	  coord1_str[coord1_str_filled] = '\0';
	}
      else
	{
	  if (!semicolon)
	    {
	      coord1_str[coord1_str_filled] = coordinates_str[i];
	      ++coord1_str_filled;
	    }
	  else
	    {
	      coord2_str[coord2_str_filled] = coordinates_str[i];
	      ++coord2_str_filled;
	    }
	}
    }

  if (!semicolon)
    {
    ret = {.x = -2147483647,
	   .y = 2147483647};
    return ret;
    }
  
  coord2_str[coord2_str_filled] = '\0';
  ret.x = atol (coord1_str);
  ret.y = atol (coord2_str);
  return ret;
}

int
get_command_type (const char* data)
{
  int ret = 0;
  char *token = NULL;
  char buffer[strlen (data) + 1];

  strncpy (buffer, data, strlen (data) + 1);

  if (strlen (data) < 1)
    ret = -1;
  
  /* Il faut extraire la commande. */
  /* We need to extract the command. */
  if (ret != -1)
    token = strtok (buffer, " ");
  if (token == NULL)
    ret = -1;
  else if (strcmp (token, "QUIT") == 0)
    ret = 1;
  else if (strcmp (token, "PING") == 0)
    ret = 2;
  else if (strcmp (token, "PONG") == 0)
    ret = 3;
  else if (strcmp (token, "CONNECT") == 0)
    ret = 4;
  else if (strcmp (token, "MOVE") == 0)
    ret = 5;
  else if (strcmp (token, "NEWPLAYER") == 0)
    ret = 6;

  return ret;
}
