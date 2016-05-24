/**
 * @file
 * @author Pierre Gillet <pierre.gillet+factorywars@linuxw.info>
 *
 * @section LICENSE
 *
 * Copyright (C) 2016 Corentin Bocquillon <0x539@nybble.fr>
 * Copyright (C) 2016 Loup Fourment
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
 * along with factorywars.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 *
 * player.cpp contain all the code about the player
 */

#include "player.h"

Player::Player(void)
{
  m_id = 1;
  m_health = 100;
  m_name = "Foobar";
  m_coordinates.x = 0;
  m_coordinates.y = 0;
  m_velocity = 10;
  for (int i = 0; i < 100; i++) // Filling the inventory with zeroes
    for (int j = 0; j < 2; j++)
      m_inventory[i][j] = 0;
}

Player::Player(int health,
	       std::string name,
	       coordinates m_coordinates,
	       int velocity)
{
  m_health = health;
  m_name = name;
  m_coordinates.x = 0;
  m_coordinates.y = 0;
  m_velocity = velocity;
  m_id = 1;
}

inline int
Player::getHealth () const
{
  return this->m_health;
}

std::string
Player::getName () const
{
  return this->m_name;
}

void
Player::setCoordinates (struct coordinates new_coords)
{
  this->m_coordinates.x = new_coords.x;
  this->m_coordinates.y = new_coords.y;
}

inline int
Player::playerIsAttacked (std::string enemy_name,
			  int damage)
{
  this->m_health -= damage;
  return this->m_health;
}

inline void
Player::playerWalks (bool horizontal,
		     bool vertical)
{
  // We'll maybe have to add this-> before every m_coordinates, gotta check
  if (horizontal == 1)
    m_coordinates.x ++;
  else
    m_coordinates.x --;

  if (vertical == 1) 
    m_coordinates.y ++;
  else
    m_coordinates.y --;
}
