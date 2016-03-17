/* main.cpp -- contain the code of the main function

Copyright (C) 2016 Corentin Bocquillon <corentin@nybble.fr>
Copyright (C) 2016 Loup Fourment
Copyright (C) 2016 Pierre Gillet

factorywars is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

factorywars is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */

#include string

/* Player :
- health --> int
- items --> tableau
- name --> string
- coordinates

 */

class player
{
public:
  int playerGetsAttacked()


private:
  int health;
  //  int items[];
  string name;
  long coordinates[];

}


class A
{
public:
  int getValue() const { return this->value; } // définition au sein de la classe
  void setValue(int value);
  void print() const;
private:
  int value;
};

inline void A::setValue(int value) // définition en dehors de la classe (inline)
{
  this->value = value;
}

void A::print() const // définition en dehors de la classe (non inline)
{
  std::cout << "Value=" << this->value << std::endl;
}
