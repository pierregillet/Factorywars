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
 * utils.cpp
 */

#include "utils.h"

char*
find_chunk_line_in_file (struct coordinates chunk_coordinates, char* dst, size_t dst_size, const char* file_path)
{
  const unsigned int LINE_SIZE = dst_size;
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
      strncpy (dst, line, dst_size);
      return dst;
    }
}

int
find_line_number_using_chunk_coordinates (struct coordinates chunk_coordinates, const char* file_path)
{
  const unsigned int LINE_SIZE = 512;
  const unsigned int REGEX_STR_SIZE = 14;

  int line_number = -1;
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
      return -1;
    }
  
  while (fgets (line, LINE_SIZE, file) != NULL)
    {
      line_number++;
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
    return -1;
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
  FILE *file = fopen (file_path, "r");
  const int LINE_SIZE = 512;
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
