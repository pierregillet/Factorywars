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
 * config.h is the header of config.c.
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <config.h>
#include <string.h>

#include "XMLParser.h"

/**
 * Get the value of a parameter in the config file.
 * @param key is the name of the parameter.
 * @param dst is the destination string.
 * @param dst_len is the length of the destination string.
 * @return 1 if success, 0 otherwise.
 */
int get_config_value (const char* key, char* dst, size_t dst_len);

/**
 * Get the defaul value of a parameter.
 *
 * @param key is the name of the parameter.
 * @param dst is the destination string.
 * @param dst_len is the length of the destination string.
 * @return 1 if there is a default value, 0 otherwise.
 */
int default_config_value (const char* key, char* dst, size_t dst_len);
