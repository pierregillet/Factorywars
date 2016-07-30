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
 * player.h the header file of player.cpp
 */

#pragma once

#include <string>
#include <string.h>

#include "structures.h"
#include "player-data.pb-c.h"

#include "gettext.h"
#define _(string) gettext (string)

class
Player
{
 public:
  
  /**
   * Constructor that creates a player with default values.
   * It should only be used for testing purpose.
   */
  Player ();

  /**
   * Constructor that creates a player object.
   * @param health is the player's initial health.
   * @param name is the player's name.
   * @param m_coordinates are the coordinates of the player.
   * @param velocity is the velocity of the player.
   */
  Player (int health, std::string name, struct coordinates coordinates, int velocity);

  /**
   * Get the player's health.
   * @return The player's health.
   */
  int getHealth () const;

  /**
   * Decrease the player health when attacked.
   * @param enemy_name is the name of the enemy attacking.
   * @param damage is the number of health points withdrawn to the player.
   * @return The new player health
   */
  int playerIsAttacked (std::string enemy_name, int damage);

  /**
   * Changes the player's coordinates by making him walk.
   * It isn't right because like this you can only walk in two directions, not only one
   * (e.g : you can't just go horizontally). We must change it.
   * @param horizontal is True if the player goes up, False if he goes down
   * @param vertical is True if the player goes up, False if he goes down
   */
  void playerWalks (bool horizontal, bool vertical);

  /**
   * Get the player’s name.
   * @return The player’s name.
   */
  std::string getName () const;

  /**
   * Set the player’s name
   */
  void setName (const char* name);

  /**
   * Set the player’s name
   */
  void setName (std::string name);

  /**
   * Set the player’s coordinates.
   * @param The new coordinates of the player.
   */
  void setCoordinates (struct coordinates new_coords);

  /**
   * Get the player’s coordinates.
   */
  struct coordinates getCoordinates () const;

  /**
   * Change the selected tool.
   * @param The direction we scroll : -1 for previous tool, 1 for next tool.
   */
  void changeSelectedTool (int scroll);

  /**
   * Returns the selected tool.
   */
  unsigned short getSelectedTool ();

  /**
   * Save the player’s data in a file.
   */
  int save (std::string path) const;

  /**
   * Save the player’s data in the default file.
   */
  int save () const;

  /**
   * Fill the members variables with those in the given save file.
   */
  int read_save (std::string path);

  /**
   * Fill the members variables with those in the default save file.
   */
  int read_save ();

  /**
   * Set the default path to the save file.
   */
  void setSaveFilePath (std::string path);

  /**
   * Get the default path to the save file.
   */
  std::string getSaveFilePath () const;


 private:
  std::string m_name;

  int m_health;
  int m_velocity;

  coordinates m_coordinates;

  int m_inventory[100][2];
  int m_toolbar[10][2];

  unsigned short m_id;
  unsigned short m_selected_tool;

  std::string m_save_file_path;
};
