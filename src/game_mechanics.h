/**
 * @file
 * @author Pierre Gillet <pierre.gillet@linuxw.info>
 *
 * @section LICENSE
 *
 * Copyright (C) 2016 Corentin Bocquillon
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
 * game_mechanics.h the header file of game_mechanics.cpp
 */

#pragma once

#include <string>
#include "structures.h"


class
Machines
{
 public:
  
  /*
   * Creates a machine object with default values.
   */
  Machines();

  /*
   * Creates a machine object
   * @param size is the size that the object will take on the map.
   */
  Machines(int size);
 private:
  int m_size;
  std::string m_path_to_image;
};

class
Inserters
{
 public :

  /*
   *
   */
  Inserters();
 private:
  int m_inserter_type;
};

class
BurnerMachines : Machines
{
 public:

  /*
   *
   */
  BurnerMachines();
 private:
  int m_energy_consumption;
};

class
BurnerInserters : BurnerMachines, Inserters
{
 public:

  /*
   *
   */
  BurnerInserters();
};

class
ElectricMachines : Machines
{
 public:

  /*
   *
   */
  ElectricMachines();
};

class
Trees
{
 public:

  /*
   *
   */
  Trees();
};

class
Ores
{
 public:

  /*
   *
   */
  Ores();
};

class
Armor
{
 public:

  /*
   *
   */
  Armor();
};

