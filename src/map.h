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
 * map.h is the header of map.cpp.
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <config.h>
#include <time.h>
#include <SDL2/SDL.h>

#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <noise/noise.h>

#include "structures.h"
#include "tile.pb-c.h"
#include "sdl/gui_utils.h"

extern "C"
{
  #include "config.h"
  #include "utils.h"
}

#include "gettext.h"
#define _(string) gettext (string)


struct coordinates
get_chunk_coordinates_from_player_movement (struct coordinates player_offset);

std::string get_save_directory_path ();

class Chunk
{
 public:
  Chunk (struct coordinates chunk_coordinates, TileProto* tile, SDL_Renderer* window_renderer);
  ~Chunk ();
  Chunk (const Chunk& other);

  inline struct coordinates getChunkCoordinates () const;
  inline struct coordinates getTileCoordinates () const;

  SDL_Texture* get_chunk_texture ();

  void set_square_item (struct coordinates square_coordinates, int item_id,
			int quantity = 1);
  int get_square_item_id (struct coordinates square_coordinates);
  int get_square_item_quantity (struct coordinates square_coordinates);

  time_t getLastUse () const;

 private:
  std::shared_ptr<SDL_Texture> m_chunk_texture;
  std::shared_ptr<SDL_Surface> m_ground, m_items;
  SDL_Renderer* m_window_renderer;

  ChunkProto* m_me;
  struct coordinates m_chunk_coordinates, m_tile_coordinates;

  time_t m_last_use;

  void generate_ground_surface ();
  void generate_item_surface ();
  void generate_texture ();
};


class Map
{
 public:
  /**
   * Default constructor.
   * 
   * @param game_name is the name of the game.
   * @param generate_seed, if it’s true, the function will generate a seed
   * unless a seed has already been generated for this game.
   */
  Map (std::string game_name, SDL_Renderer* window_renderer, bool generate_seed = true);

  /**
   * Same as default constructor but if the game does not exist,
   * it uses the seed given as parameter if a seed has not already been 
   * generated for this game.
   * @param game_name is the name of the game.
   * @param seed is used to generate chunks randomly.
   */
  Map (std::string game_name, int seed, SDL_Renderer* window_renderer);

    
  SDL_Texture* get_chunk_texture (struct coordinates chunk_coordinates);

  int get_surface_item (struct coordinates chunk_coordinates,
			struct coordinates square_coordinates);

  void set_surface_item (struct coordinates chunk_coordinates,
			 struct coordinates square_coordinates, int item_id,
			 int quantity = 1);

  void save();

 private:
  int m_seed;
  std::string m_game_path;
  std::string m_tiles_directory_path;
  noise::module::Perlin m_map_noise;

  std::vector<Chunk> m_chunks;
  std::vector<TileProto*> m_tiles;

  SDL_Renderer* m_window_renderer;

  void load_chunk (struct coordinates chunk_coordinates);
  void load_tile (struct coordinates tile_coordinates);

  void save_tile (TileProto* tile);
  void save_tiles ();

  /**
   * Il faudra vérifier à chaque tronçon déchargé si c’était le dernier 
   * utilisé de sa dalle.
   */
  void unload_unused_chunks ();
  void generate_tile (struct coordinates tile_coordinates);
  struct coordinates get_tile_by_chunk (struct coordinates
					chunk_coordinates) const;

  int get_floor_id (double random_value);
};
