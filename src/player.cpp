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
  m_selected_tool = 0;
  m_velocity = 10;
  for (int i = 0; i < 100; i++) // Filling the inventory with zeroes
    for (int j = 0; j < 2; j++)
      m_inventory[i][j] = 0;
}

Player::Player(int health,
	       std::string name,
	       coordinates coordinates,
	       int velocity)
{
  m_health = health;
  m_name = name;
  m_coordinates.x = coordinates.x;
  m_coordinates.y = coordinates.y;
  m_selected_tool = 0;
  m_velocity = velocity;
  m_id = 1;
  m_save_file_path = "";
}

inline int
Player::getHealth () const
{
  return m_health;
}

std::string
Player::getName () const
{
  return m_name;
}

void
Player::setName (const char* name)
{
  m_name = name;
}

void
Player::setName (std::string name)
{
  m_name = name;
}

void
Player::setCoordinates (struct coordinates new_coords)
{
  m_coordinates.x = new_coords.x;
  m_coordinates.y = new_coords.y;
}

struct coordinates
Player::getCoordinates () const
{
  return m_coordinates;
}

inline int
Player::playerIsAttacked (std::string enemy_name,
			  int damage)
{
  m_health -= damage;
  return m_health;
}

inline void
Player::playerWalks (bool horizontal,
		     bool vertical)
{
  if (horizontal == 1)
    m_coordinates.x ++;
  else
    m_coordinates.x --;

  if (vertical == 1) 
    m_coordinates.y ++;
  else
    m_coordinates.y --;
}

void
Player::changeSelectedTool (int scroll)
{
  if (scroll >= 1)
    m_selected_tool += (m_selected_tool < 9)? 1 : -9;
  else
    m_selected_tool -= (m_selected_tool > 0)? 1 : -9;
}

unsigned short
Player::getSelectedTool ()
{
  return m_selected_tool;
}

int
Player::save (std::string path) const
{
  PlayerData player_data = PLAYER_DATA__INIT;

  // Fill the player’s name
  player_data.name = (char*) malloc (sizeof (char) * m_name.size ());
  strcpy (player_data.name, m_name.c_str ());

  // Coordinates
  player_data.x = (int64_t) m_coordinates.x;
  player_data.y = (int64_t) m_coordinates.y;

  // Velocity
  player_data.velocity = m_velocity;

  // Selected tool
  player_data.selected_tool = (int32_t) m_selected_tool;

  // Inventory
  const int number_of_items_inventory = 100;

  player_data.n_inventory_item_id = number_of_items_inventory;
  player_data.n_inventory_number = number_of_items_inventory;

  player_data.inventory_item_id = (int32_t*) malloc (sizeof (int32_t) * number_of_items_inventory);
  player_data.inventory_number = (int32_t*) malloc (sizeof (int32_t) * number_of_items_inventory);

  if (player_data.inventory_item_id == NULL)
    {
      fprintf (stderr, _("Error, cannot allocate memory.\n"));
      return 0;
    }

  if (player_data.inventory_number == NULL)
    {
      fprintf (stderr, _("Error, cannot allocate memory.\n"));
      return 0;
    }
  
  for (int i = 0; i < number_of_items_inventory; i++)
    {
      player_data.inventory_item_id[i] = m_inventory[i][0];
      player_data.inventory_number[i] = m_inventory[i][1];
    }

  // Toolbar
  const int number_of_items_toolbar = 10;

  player_data.n_toolbar_item_id = number_of_items_toolbar;
  player_data.n_toolbar_number = number_of_items_toolbar;

  player_data.toolbar_item_id = (int32_t*) malloc (sizeof (int32_t) * number_of_items_toolbar);
  player_data.toolbar_number = (int32_t*) malloc (sizeof (int32_t) * number_of_items_toolbar);

  if (player_data.toolbar_item_id == NULL)
    {
      fprintf (stderr, _("Error, cannot allocate memory.\n"));
      return 0;
    }
  if (player_data.toolbar_number == NULL)
    {
      fprintf (stderr, _("Error, cannot allocate memory.\n"));
      return 0;
    }
  
  for (int i = 0; i < number_of_items_toolbar; i++)
    {
      player_data.toolbar_item_id[i] = m_toolbar[i][0];
      player_data.toolbar_number[i] = m_toolbar[i][1];
    }

  // Serializing
  unsigned long len = player_data__get_packed_size (&player_data);
  uint8_t *buffer = (uint8_t*) malloc (len);
  if (buffer == NULL)
    {
      fprintf (stderr, _("Error, cannot allocate memory.\n"));
      return 0;
    }

  player_data__pack (&player_data, buffer);

  // Write to file
  FILE *save_file = fopen (path.c_str (), "w");
  if (save_file == NULL)
    {
      fprintf (stderr, _("Error the file cannot be opened.\n"));
      return 0;
    }

  fwrite (buffer, len, 1, save_file);
  fclose (save_file);

  // Freeing
  free (player_data.name);

  free (player_data.inventory_item_id);
  free (player_data.inventory_number);

  free (player_data.toolbar_item_id);
  free (player_data.toolbar_number);

  free (buffer);

  return 1;
}

int
Player::save () const
{
  if (m_save_file_path.empty ())
    return 0;

  return save (m_save_file_path);
}

int
Player::read_save (std::string path)
{
  PlayerData *player_data;

  uint8_t *buffer;
  buffer = (uint8_t*) malloc (sizeof (uint8_t));
  if (buffer == NULL)
    {
      fprintf (stderr, _("Failed to allocate memory.\n"));
      return 0;
    }

  char c;
  int n = 0;

  FILE *save_file = fopen (path.c_str (), "r");
  if (save_file == NULL)
    {
      return 0;
    }

  while (fread (&c, sizeof (char), 1, save_file))
    {
      buffer[n] = c;
      n++;

      buffer = (uint8_t*) realloc (buffer, sizeof (uint8_t) * (n + 1));
      if (buffer == NULL)
	{
	  fprintf (stderr, _("Failed to reallocate memory.\n"));
	  return 0;
	}
    }
  fclose (save_file);

  player_data = player_data__unpack (NULL, n, buffer);
  if (player_data == NULL)
    {
      fprintf (stderr, _("Error while reading the file.\n"));
      free (buffer);
      return 0;
    }

  free (buffer);

  // Fill player’s informations
  m_name = std::string (player_data->name);
  m_health = player_data->health;

  m_coordinates.x = player_data->x;
  m_coordinates.y = player_data->y;

  m_velocity = player_data->velocity;

  if (player_data->n_inventory_item_id < 100)
    return 0;

  if (player_data->n_inventory_number < 100)
    return 0;

  if (player_data->n_toolbar_item_id < 10)
    return 0;

  if (player_data->n_toolbar_number < 10)
    return 0;

  for (int i = 0; i < 100; i++)
    {
      m_inventory[i][0] = player_data->inventory_item_id[i];
      m_inventory[i][1] = player_data->inventory_number[i];
    }

  for (int i = 0; i < 10; i++)
    {
      m_toolbar[i][0] = player_data->toolbar_item_id[i];
      m_toolbar[i][1] = player_data->toolbar_number[i];
    }

  m_selected_tool = player_data->selected_tool;
  
  player_data__free_unpacked (player_data, NULL);

  return 1;
}

int
Player::read_save ()
{
  if (m_save_file_path.empty ())
    return 0;

  return read_save (m_save_file_path);
}

void
Player::setSaveFilePath (std::string path)
{
  m_save_file_path = path;
}

std::string
Player::getSaveFilePath () const
{
  return m_save_file_path;
}

int
Player::add_object_to_inventory (int item_id, int number)
{
  const int NUMBER_OF_ELEMENTS = 100;
  const int STACK_LIMIT = 100;

  // On vérifie que « number » ne dépasse pas la limite d’empilement.
  if (number > STACK_LIMIT)
    return 0;

  int same_id = -1;
  int first_empty_element = -1;

  for (int i = 0; i < NUMBER_OF_ELEMENTS; i++)
    {
      // Si un ou plusieurs objets du même type sont déjà présent, on regarde
      // si on peut en rajouter « number ».
      if (m_inventory[i][0] == item_id)
	{
	  if (m_inventory[i][1] <= STACK_LIMIT + number)
	    {
	      same_id = i;
	      m_inventory[i][1] += number;
	      return 1;
	    }
	}

      if (m_inventory[i][0] == 0 && first_empty_element != -1)
	first_empty_element = i;
    }

  // Si on arrive ici, c’est qu’aucun élément du même type n’a été trouvé dans
  // le tableau ou bien il n’était plus possible d’empiler « number » nouveaux
  // objets.

  // On vérifie donc si nous avons trouvé une case vide.
  if (first_empty_element != -1)
    {
      // Si c’est le cas, on ajoute « number » objets dans cette case
      m_inventory[first_empty_element][0] = item_id;
      m_inventory[first_empty_element][1] = number;
    }
  else
    return 0;			// Il n’y a plus de place 

  return 1;
}

int
Player::remove_object_from_inventory (int item_id, int number)
{
  const int NUMBER_OF_ELEMENTS = 100;
  const int STACK_LIMIT = 100;

  // Si on essaye de retirer plus d’objets que l’inventaire ne peut en
  // contenir.
  if (number > NUMBER_OF_ELEMENTS * STACK_LIMIT)
    return 0;

  int number_of_items = 0;
  std::vector<int> relevant_squares;

  for (int i = 0; i < NUMBER_OF_ELEMENTS; i++)
    {
      if (m_inventory[i][0] == item_id)
	{
	  number_of_items += m_inventory[i][1];
	  relevant_squares.push_back (i);
	}
    }

  if (number_of_items < number)
    return 0;

  for (int square : relevant_squares)
    {
      if (number == 0)
	break;

      if (number >= STACK_LIMIT)
	{
	  number -= m_inventory[square][1];

	  m_inventory[square][0] = 0;
	  m_inventory[square][1] = 0;
	}
      else
	{
	  if (number >= m_inventory[square][1])
	    {
	      number -= m_inventory[square][1];

	      m_inventory[square][0] = 0;
	      m_inventory[square][1] = 0;
	    }
	  else
	    m_inventory[square][1] -= number;
	}
    }
  
  return 1;
}
