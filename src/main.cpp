/**
 * @file
 * @author Corentin Bocquillon <0x539@nybble.fr>
 * @author Pierre Gillet
 * @author Loup Fourment
 *
 * @section LICENSE
 *
 * Copyright (C) 2016 Corentin Bocquillon <corentin@nybble.fr>
 * Copyright (C) 2016 Loup Fourment
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
 * along with factorywars.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 *
 * main.cpp contain the code of the main function
 */

#include "gui.h"
#include "save.h"
#include "structures.h"

int
main (int argc, char *argv[])
{
  struct coordinates chunk_coordinates, square_coordinates;
  char coordinates_str[512];

  chunk_coordinates.x = 0;
  chunk_coordinates.y = 1;
  square_coordinates.x = 5;
  square_coordinates.y = 8;
  
  printf ("item: %d\n", get_surface_item (chunk_coordinates, square_coordinates));
  printf ("coord: %s\n", coordinates_to_string (square_coordinates, coordinates_str, 512));
  
  gui ();
  return 0;
}
