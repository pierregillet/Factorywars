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
coordinates_to_string (struct coordinates coordinates, char *dst, size_t dst_len)
{
  snprintf (dst, dst_len, "%ld;%ld", coordinates.x, coordinates.y);
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

  /* On écrit le début du fichier */
  // We write the beginning of the file
  while (line_number != position)
    {
      read (pipe_read_file[0], &c, sizeof (char));
      fwrite (&c, 1, sizeof (char), file);
      if (c == '\n')
	line_number++;
    }
  fwrite (line, line_size, sizeof (char), file);

  /* Si le remplacement est activé, on ignore la ligne suivante */
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
  /* Nombre d’octets */
  /* number of octets */
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
      ret.x = -2147483647;
      ret.y = 2147483647;
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

struct directory_list*
list_directory (const char* dir_name, int only_directories)
{
  struct directory_list *dir_list, *head;
  struct dirent* cur_entry;

  dir_list = (struct directory_list*) malloc (sizeof (struct directory_list));
  head = dir_list;

  dir_list->dir_name = NULL;
  dir_list->prev = NULL;
  dir_list->next = NULL;

  DIR *dir = opendir (dir_name);
  if (dir == NULL)
    return NULL;

  errno = 0;
  cur_entry = readdir (dir);
  if (errno == EBADF)
    {
      closedir (dir);
      return NULL;
    }

  /* Pour savoir si c’est un dossier */
  struct stat file_stat;
  char *file_path;
  struct tm *timeinfo;

  while (cur_entry != NULL)
    {
      /* On ignore le dossier « . » */
      if (strcmp (cur_entry->d_name, ".") == 0)
	{
	  cur_entry = readdir (dir);
	  continue;
	}

      if (strcmp (cur_entry->d_name, "..") == 0)
	{
	  cur_entry = readdir (dir);
	  continue;
	}

      if (only_directories)
	{
	  file_path = (char*) malloc ((strlen (cur_entry->d_name)
				       + strlen (dir_name)) * sizeof (char));
	  if (file_path == NULL)
	    return NULL;

	  strcpy (file_path, dir_name);
	  strcat (file_path, "/");
	  strcat (file_path, cur_entry->d_name);

	  stat (file_path, &file_stat);
	  if (!S_ISDIR (file_stat.st_mode))
	    {
	      cur_entry = readdir (dir);
	      free (file_path);
	      continue;
	    }

	  free (file_path);
	}

      if (dir_list->dir_name == NULL)
	{
	  dir_list->dir_name = (char*) malloc ((strlen (cur_entry->d_name) * sizeof (char)));
	  strcpy (dir_list->dir_name, cur_entry->d_name);
	}
      else
	{
	  dir_list->next = (struct directory_list*) malloc
	    (sizeof (struct directory_list));

	  /* L’élément précédent de la nouvelle liste est l’élément actuel */
	  dir_list->next->prev = dir_list;

	  dir_list = dir_list->next;
	  dir_list->next = NULL;

	  dir_list->dir_name = (char*) malloc ((strlen (cur_entry->d_name) * sizeof (char)));
	  strcpy (dir_list->dir_name, cur_entry->d_name);
	}

      timeinfo = localtime (&file_stat.st_mtime);
      strftime (dir_list->last_modification, 32, "%FT%X%z", timeinfo);

      cur_entry = readdir (dir);
    }

  closedir (dir);
  return head;
}

void
free_dir_list (struct directory_list* dir_list)
{
  struct directory_list *next_element;

  /* On atteint le début de la liste */
  while (dir_list->prev != NULL)
    {
      dir_list = dir_list->prev;
    }

  /* S’il n’y a qu’un seul élément, on le libère et on quitte */
  if (dir_list->next == NULL)
    {
      free (dir_list->dir_name);
      free (dir_list);

      return;
    }

  while (dir_list->next != NULL)
    {
      next_element = dir_list->next;
      
      free (dir_list->dir_name);
      free (dir_list);

      dir_list = next_element;
    }

  return;
}
