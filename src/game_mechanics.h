/**
 * @file
 * @author Corentin Bocquillon <0x539@nybble.fr>
 *
 * @section LICENSE
 *
 * Copyright (C) 2016 Corentin Bocquillon
 * Copyright (C) 2016 Corentin Bocquillon
 * Copyright (C) 2016 Loup Fourment
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

#include <string>

struct coordinates
{
  long x;
  long y;
};

class
Player
{
 public:
  Player(void);
  Player(int, std::string, coordinates, int);
  int getPlayerHealth () const;
  int playerGetsAttacked (std::string, int);
  void playerWalks(bool, bool);

 private:
  int m_health;
  std::string m_name;
  coordinates m_coordinates;
  int m_velocity;
};

class
Items
{
 public:
  Items (void);
  Items (int);
  
 private:
  int m_stack_size;
}

