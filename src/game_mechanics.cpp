/**
 * @file
 * @author Pierre Gillet
 *
 * @section LICENSE
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
 * along with factorywars.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 *
 * game_mechanics.cpp contain all the code for the game mechanics
 */

#include <string>

/* Player :
- health --> int
- items --> tableau
- name --> string
- coordinates

 */

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
  Player(int, std::string);
  Player(int, std::string, coordinates);
  int getPlayerHealth () const;
  int playerGetsAttacked (std::string, int);
  int playerWalks(bool, bool);

private:
  int m_health;
  std::string m_name;
  coordinates m_coordinates;

};

Player::Player(void)
{
  m_health = 100;
  m_name = "Foobar";
  m_coordinates.x = 0;
  m_coordinates.y = 0;
}

Player::Player(int health,
	       std::string name)
{
  m_health = health;
  m_name = name;
  m_coordinates.x = 0;
  m_coordinates.y = 0;
}

Player::Player(int health,
	       std::string name,
	       coordinates m_coordinates)
{
  m_health = health;
  m_name = name;
  m_coordinates.x = 0;
  m_coordinates.y = 0;
}

inline int
Player::getPlayerHealth () const
{
  return this->m_health;
}

inline int
Player::playerGetsAttacked (std::string enemy_name,
			    int damage)
{
  this->m_health -= damage;
}

inline void
Player::playerWalks (bool vertical,
		     bool horizontal)
{
  if (vertical == 1)
    {
      this->m_coordinates.y ++;
    }
  else
    {
      this->m_coordinates.y--;
    }
  if (horizontal == 1)
    {
      this->m_coordinates.x ++;
    }
  else
    {
      this->m_coordinates--;
    }
}

int main ()
{

}

