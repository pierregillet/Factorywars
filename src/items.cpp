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
 * items.h the header file of items.cpp
 */

#include "items.h"

ItemsName::ItemsName()
{
  std::string m_items_name[] = {"Iron ore",
				"Iron plate",
				"Copper ore",
				"Copper plate",
				"Wood",
				"Electric inserter"};
  for (std::string i : m_items_name)
    {
      m_items_map[i] = m_items_name[i];
    }
}

inline std::string
ItemsName::getItemName(int id)
{
  return m_items_map[id];
}
