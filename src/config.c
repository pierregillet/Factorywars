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
 * config.c handle the config file.
 */

#include "config.h"

int
get_config_value (const char* key, char* dst, size_t dst_len)
{
  const int path_len = 16384;
  char path[path_len], conf_dir[path_len];

  char *config_value = NULL;
  const char filename[] = "factorywars.conf";

  /* We get environment variables */
  const char *xdg_config_home = getenv ("XDG_CONFIG_HOME");
  const char *home = getenv ("HOME");

  int return_default_value = 0;

  if (RUN_IN_PLACE)
    strncpy (path, filename, path_len);
  else if (xdg_config_home == NULL)
    {
      if (home == NULL)
	strncpy (path, filename, path_len);
      else
	{
	  /* We need to use the default value of XDG_CONFIG_HOME */
	  strncpy (conf_dir, home, path_len);
	  strncat (conf_dir, "/", path_len);
	  strncat (conf_dir, ".config/factorywars", path_len);

	  strncpy (path, conf_dir, path_len);
	  strncat (path, "/", path_len);
	  strncat (path, filename, path_len);
	}
    }
  else
    {
      /* We use the current value of XDG_CONFIG_HOME */
      strncpy (conf_dir, xdg_config_home, path_len);
      strncat (conf_dir, "/factorywars", path_len);
    }

  /* Test if conf_dir exists */
  struct stat file_stat;

  if (!RUN_IN_PLACE)
    {
      if (stat (conf_dir, &file_stat) == -1)
	{
	  if (default_config_value (key, dst, dst_len) == 0)
	    return 0;

	  return_default_value = 1;
	}

      else if (!S_ISDIR (file_stat.st_mode))
	{
	  if (default_config_value (key, dst, dst_len) == 0)
	    return 0;

	  return_default_value = 1;
	}

      /* mkdir (conf_dir, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP */
      /*        | S_IROTH | S_IXOTH);	/\* make the configuration directory *\/ */
    }

  /* Test if path exists */
  if (stat (path, &file_stat) == -1)
    {
      if (default_config_value (key, dst, dst_len) == 0)
	return 0;

      return_default_value = 1;
    }

  if (!return_default_value)
    {
      config_value = getValueOfElement (key, 0, NULL, NULL, path);
      strncpy (dst, config_value, dst_len);
    }
  
  return 1;
}

int
default_config_value (const char* key, char* dst, size_t dst_len)
{
  if (strcmp (key, "name") == 0)
    strncpy (dst, "Foobar", dst_len);

  else if (strcmp (key, "port") == 0)
    strncpy (dst, "1337", dst_len);

  else if (strcmp (key, "height") == 0)
    strncpy (dst, "640", dst_len);

  else if (strcmp (key, "width") == 0)
    strncpy (dst, "480", dst_len);

  else
    return 0;

  return 1;
}
