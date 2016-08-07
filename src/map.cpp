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
 * map.cpp provide functions to read and write save files.
 */

#include "map.h"

struct coordinates
get_chunk_coordinates_from_player_movement (struct coordinates player_offset)
{
  /* We initialize the constants */
  const int NUMBER_OF_SQUARE_PER_ROW = 16;
  const int SQUARE_WIDTH = 24;

  const int chunk_width = NUMBER_OF_SQUARE_PER_ROW * SQUARE_WIDTH;

  struct coordinates center_chunk_coordinates;

  center_chunk_coordinates.y =
    player_offset.y / chunk_width;

  center_chunk_coordinates.x =
    player_offset.x / chunk_width;

  return center_chunk_coordinates;
}

std::string
get_save_directory_path ()
{
  std::string save_directory_path;

  const char *xdg_data_home = getenv ("XDG_DATA_HOME");
  const char *home = getenv ("HOME");

  if (RUN_IN_PLACE)
    save_directory_path = "saves/";

  else if (xdg_data_home == NULL)
    {
      if (home == NULL)
	save_directory_path = "saves/";
      else
	{
	  // On utilise la valeur par défaut de XDG_DATA_HOME
	  save_directory_path = std::string (home) + "/.local/share/factorywars/saves/";
	}
    }

  else
    save_directory_path = std::string (xdg_data_home) + "/factorywars/saves/";

  return save_directory_path;
}

Map::Map (std::string game_name, SDL_Renderer* window_renderer, bool generate_seed)
{
  m_game_path = get_save_directory_path () + game_name;
  m_tiles_directory_path = m_game_path + "/tiles/";

  printf ("%s\n", m_game_path.c_str ());

  std::string seed_path = m_game_path + "/" + "seed";

  mode_t rwx_rx_rx = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP
    | S_IROTH | S_IXOTH;

  if (!is_existing (m_game_path.c_str ()))
    {
      mkdir (m_game_path.c_str (), rwx_rx_rx);
      mkdir (m_tiles_directory_path.c_str (), rwx_rx_rx);
    }
  else
    {
      if (!is_existing (m_tiles_directory_path.c_str ()))
	{
	  mkdir (m_tiles_directory_path.c_str (), rwx_rx_rx);
	}

      if (is_existing (seed_path.c_str ()))
	{
	  std::ifstream seed_file (seed_path, std::ifstream::in);
	  seed_file >> m_seed;
	  seed_file.close ();

	  m_map_noise.SetSeed (m_seed);
	}

      else if (generate_seed)
	{
	  srand (time (NULL));
	  m_seed = rand ();

	  std::ofstream seed_file (seed_path, std::ofstream::out);
	  seed_file << m_seed;
	  seed_file << std::endl;
	  seed_file.close ();

	  m_map_noise.SetSeed (m_seed);
	}
    }

  m_window_renderer = window_renderer;
}

Map::Map (std::string game_name, int seed, SDL_Renderer* window_renderer)
{
  m_seed = 0;

  Map (game_name, false, window_renderer);

  if (m_seed == 0)
    {
      m_seed = seed;
      std::ofstream seed_file ("seed", std::ofstream::out);
      seed_file << m_seed;
      seed_file.close ();

      m_map_noise.SetSeed (m_seed);
    }
}

SDL_Texture*
Map::get_chunk_texture (struct coordinates chunk_coordinates)
{
  for (Chunk chunk : m_chunks)
    {
      if (chunk.getChunkCoordinates ().x == chunk_coordinates.x
	  && chunk.getChunkCoordinates ().y == chunk_coordinates.y)
	return chunk.get_chunk_texture ();
    }

  // Le tronçon n’est pas encore en mémoire.
  load_tile (get_tile_by_chunk (chunk_coordinates));

  // On cherche de nouveau.
  for (Chunk chunk : m_chunks)
    {
      if (chunk.getChunkCoordinates ().x == chunk_coordinates.x
	  && chunk.getChunkCoordinates ().y == chunk_coordinates.y)
	return chunk.get_chunk_texture ();
    }

  // Erreur
  return NULL;
}

void
Map::set_surface_item (struct coordinates chunk_coordinates,
		       struct coordinates square_coordinates, int item_id,
		       int quantity)
{
  for (Chunk chunk : m_chunks)
    {
      if (chunk.getChunkCoordinates ().x == chunk_coordinates.x
	  && chunk.getChunkCoordinates ().y == chunk_coordinates.y)
	{
	  chunk.set_square_item (square_coordinates, item_id, quantity);

	  return;
	}
    }
}

int
Map::get_surface_item (struct coordinates chunk_coordinates,
		       struct coordinates square_coordinates)
{
  for (Chunk chunk : m_chunks)
    {
      if (chunk.getChunkCoordinates ().x == chunk_coordinates.x
	  && chunk.getChunkCoordinates ().y == chunk_coordinates.y)
	{
	  return chunk.get_square_item_id (square_coordinates);
	}
    }
}

void
Map::load_tile (struct coordinates tile_coordinates)
{
  // Il faut vérifier que la dalle n’est pas déjà chargée.
  for (TileProto* tile : m_tiles)
    {
      printf ("%d;%d\n", tile->x, tile->y);

      if (tile->x == tile_coordinates.x && tile->y == tile_coordinates.y)
	return;
    }

  // On mémorise le chemin vers le fichier
  std::string tile_path = m_tiles_directory_path;
  tile_path += std::to_string (tile_coordinates.x) + ";";
  tile_path += std::to_string (tile_coordinates.y);

  if (!is_existing (tile_path.c_str ()))
    {
      generate_tile (tile_coordinates);
      load_tile (tile_coordinates);
      return;
    }

  // Il faut charger la dalle et la mettre à la fin du vecteur m_tiles.
  TileProto *tile;
  char c;
  int n = 0;
  uint8_t *buffer = NULL;

  FILE *tile_file = fopen (tile_path.c_str (), "r");
  if (tile_file == NULL)
    {
      fprintf (stderr, _("Error while opening the tile file."));
      fprintf (stderr, "\n");
      return;
    }

  while (fread (&c, sizeof (char), 1, tile_file))
    {
      buffer = (uint8_t*) realloc (buffer, sizeof (uint8_t) * (n + 1));
      if (buffer == NULL)
	{
	  fprintf (stderr, _("Failed to allocate memory."));
	  fprintf (stderr, "\n");
	  return;
	}

      buffer[n] = c;
      n++;
    }
  fclose (tile_file);

  tile = tile_proto__unpack (NULL, n, buffer);
  if (tile == NULL)
    {
      fprintf (stderr, _("Error while reading the tile’s file."));
      fprintf (stderr, "\n");
      free (buffer);
      return;
    }
  
  free (buffer);

  m_tiles.push_back (tile);

  // Il faut créer un à un les tronçons en parcourant les coordonnées des
  // tronçons dans la dalle et ajouter les tronçons au fur et à mesure dans
  // le vecteur m_chunks.
  for (int i = 0; i < tile->n_chunks; i++)
    {
      printf ("i : %d\n", i);
      m_chunks.push_back (Chunk ({tile->chunks[i]->x, tile->chunks[i]->y}, tile, m_window_renderer));
      printf ("i2 : %d\n", i);
    }
}

void
Map::generate_tile (struct coordinates tile_coordinates)
{
  TileProto tile = TILE_PROTO__INIT;

  tile.x = tile_coordinates.x;
  tile.y = tile_coordinates.y;

  double random_value;
  struct coordinates_l square_coordinates;

  tile.n_chunks = 8 * 8;
  tile.chunks = (ChunkProto**) malloc (sizeof (ChunkProto*) * tile.n_chunks);
  if (tile.chunks == NULL)
    {
      fprintf (stderr, _("Failed to allocate memory."));
      fprintf (stderr, "\n");
      return;
    }
  
  for (int i = 0; i < tile.n_chunks; i++)
    {
      printf ("i : %d\n", i);
      printf ("1\n");
      tile.chunks[i] = (ChunkProto*) malloc (sizeof (ChunkProto));
      if (tile.chunks[i] == NULL)
	{
	  fprintf (stderr, _("Failed to allocate memory."));
	  fprintf (stderr, "\n");
	  free (tile.chunks);
	  return;
	}
      printf ("2\n");

      chunk_proto__init (tile.chunks[i]);
      printf ("3\n");
      tile.chunks[i]->x = i / 8 + 8 * tile.x;
      tile.chunks[i]->y = i % 8 + 8 * tile.y;
      printf ("4\n");
      
      tile.chunks[i]->n_squares = 16 * 16;
      tile.chunks[i]->squares = (SquareProto**)
	malloc (sizeof (SquareProto*) * tile.chunks[i]->n_squares);
      if (tile.chunks[i]->squares == NULL)
	{
	  fprintf (stderr, _("Failed to allocate memory."));
	  fprintf (stderr, "\n");

	  free (tile.chunks);

	  for (int k = 0; k < i + 1; k++)
	    {
	      free (tile.chunks[k]);
	    }
	  return;
	}

      printf ("%d cases dans %d;%d\n", tile.chunks[i]->n_squares, tile.chunks[i]->x, tile.chunks[i]->y);
      for (int j = 0; j < tile.chunks[i]->n_squares; j++)
	{
	  printf ("j : %d\n", j);
	  tile.chunks[i]->squares[j] = (SquareProto*) malloc (sizeof (SquareProto));
	  if (tile.chunks[i]->squares[j] == NULL)
	    {
	      fprintf (stderr, _("Failed to allocate memory."));
	      fprintf (stderr, "\n");
	      return;
	    }

	  square_proto__init (tile.chunks[i]->squares[j]);
	  tile.chunks[i]->squares[j]->x = j / 16;
	  tile.chunks[i]->squares[j]->y = j % 16;

	  square_coordinates.x = tile.chunks[i]->squares[j]->x + tile.chunks[i]->x * 16;
	  square_coordinates.y = tile.chunks[i]->squares[j]->y + tile.chunks[i]->y * 16;
	  random_value = m_map_noise.GetValue (square_coordinates.x * (1.0/1000.0),
					       square_coordinates.y * (1.0/1000.0),
					       0);
	  // printf ("%f\n", random_value);
	  tile.chunks[i]->squares[j]->floor = get_floor_id (random_value);

	  // printf ("%d;%d %d\n", tile.chunks[i]->squares[j]->x, tile.chunks[i]->squares[j]->y, tile.chunks[i]->squares[j]->floor);

	  tile.chunks[i]->squares[j]->item = -1;
	  tile.chunks[i]->squares[j]->quantity = 0;
	}
      printf ("Ici\n");
    }
  printf ("Là\n");

  // On l’écrit dans son fichier.
  unsigned long len = tile_proto__get_packed_size (&tile);

  std::string tile_file_path = m_tiles_directory_path;
  tile_file_path += std::to_string (tile_coordinates.x);
  tile_file_path += ";" + std::to_string (tile_coordinates.y);

  uint8_t *buffer = (uint8_t*) malloc (len);
  if (buffer == NULL)
    return;

  tile_proto__pack (&tile, buffer);

  FILE *tile_file = fopen (tile_file_path.c_str (), "w");
  if (tile_file == NULL)
    {
      fprintf (stderr, _("Error, the tile file cannot be opened."));
      fprintf (stderr, "\n");
      return;
    }
  fwrite (buffer, len, 1, tile_file);
  fclose (tile_file);

  // Il faut libèrer la mémoire allouer avec malloc.
  free (buffer);

  for (int i = 0; i < tile.n_chunks; i++)
    {
      for (int j = 0; j < tile.chunks[i]->n_squares; j++)
	{
	  free (tile.chunks[i]->squares[j]);
	}

      free (tile.chunks[i]->squares);
      free (tile.chunks[i]);
    }

  free (tile.chunks);
}

struct coordinates
Map::get_tile_by_chunk (struct coordinates chunk_coordinates) const
{
  struct coordinates tile_coordinates;

  tile_coordinates.x = chunk_coordinates.x / 8;
  tile_coordinates.y = chunk_coordinates.y / 8;
    
  return tile_coordinates;
}

int
Map::get_floor_id (double random_value)
{
  int floor_id = -1;

  // [-1 ; -0,33]
  if (random_value >= -1 && random_value <= -1.0 / 3.0)
    {
      // Biome 1
      floor_id = 1;
    }

  // ]-0,33 ; 0,33]
  else if (random_value > -1.0 / 3.0 && random_value <= 1.0 / 3.0)
    {
      // Biome 2
      floor_id = 2;
    }

  // ]0,33 ; 1]
  else if (random_value > 1.0 / 3.0 && random_value <= 1)
    {
      // Biome 3
      floor_id = 3;
    }

  return floor_id;
}

void
Map::unload_unused_chunks ()
{
  time_t last_use;
  time_t now = time (NULL);

  struct coordinates chunk_coordinates, tile_coordinates;
  bool delete_tile = false;

  for (int i = 0; i < m_chunks.size (); i++)
    {
      last_use = m_chunks[i].getLastUse ();

      // Si ça fait plus d’une minute que le tronçon n’est pas utilisé.
      if (difftime (now, last_use) > 60.0)
	{
	  // On récupère les coordonnées du tronçon et de la dalle le contenant.
	  chunk_coordinates = m_chunks[i].getChunkCoordinates ();
	  tile_coordinates = m_chunks[i].getTileCoordinates ();

	  // On le supprime.
	  m_chunks.erase (m_chunks.begin () + i);

	  // On regarde si d’autres tronçons contenus dans cette dalle existe
	  // encore.
	  for (Chunk chunk : m_chunks)
	    {
	      if (chunk.getTileCoordinates ().x == tile_coordinates.x
		  && chunk.getTileCoordinates ().y == tile_coordinates.y)
		{
		  delete_tile = true;
		  break;
		}
	    }

	  // S’il faut supprimer la dalle.
	  if (delete_tile)
	    {
	      // On parcourt la liste des dalles.
	      for (int i = 0; i < m_tiles.size (); i++)
		{
		  // Si la dalle correspond.
		  if (m_tiles[i]->x == tile_coordinates.x
		      && m_tiles[i]->y == tile_coordinates.y)
		    {
		      tile_proto__free_unpacked (m_tiles[i], NULL);
		      m_tiles.erase (m_tiles.begin () + i);
		    }
		}
	    }
	}
    }
}

Chunk::Chunk (struct coordinates chunk_coordinates, TileProto* tile, SDL_Renderer* window_renderer)
{
  // On initialise les coordonnées.
  m_chunk_coordinates = chunk_coordinates;
  m_tile_coordinates.x = tile->x;
  m_tile_coordinates.y = tile->y;

  m_me = NULL;
  for (int i = 0; i < tile->n_chunks; i++)
    {
      if (tile->chunks[i]->x == chunk_coordinates.x
	  && tile->chunks[i]->y == chunk_coordinates.y)
	{
	  m_me = tile->chunks[i];
	  break;
	}
    }

  m_window_renderer = window_renderer;

  m_ground = NULL;
  m_chunk_texture = NULL;
  
  generate_ground_surface ();

  printf ("1\n");
  generate_texture ();

  // On initialise le temps de dernière utilisation à maintenant.
  m_last_use = time (NULL);
  printf ("2\n");
}

Chunk::~Chunk ()
{
  if (m_ground != NULL)
    SDL_FreeSurface (m_ground);

  if (m_chunk_texture != NULL)
    SDL_DestroyTexture (m_chunk_texture);
}

inline struct coordinates
Chunk::getChunkCoordinates () const
{
  return m_chunk_coordinates;
}

inline struct coordinates
Chunk::getTileCoordinates () const
{
  return m_tile_coordinates;
}

SDL_Texture*
Chunk::get_chunk_texture ()
{
  m_last_use = time (NULL);
  return m_chunk_texture;
}

void
Chunk::set_square_item (struct coordinates square_coordinates, int item_id,
			int quantity)
{
  SquareProto* current_square = NULL;
  
  // On doit trouver le carré.
  for (int i = 0; i < m_me->n_squares; i++)
    {
      current_square = m_me->squares[i];

      if (current_square->x == square_coordinates.x
	  && current_square->y == square_coordinates.y)
	{
	  current_square->item = item_id;
	  current_square->quantity = quantity;
	}
    }

  m_last_use = time (NULL);
}

int
Chunk::get_square_item_id (struct coordinates square_coordinates)
{
  SquareProto* current_square = NULL;
  
  // On doit trouver le carré.
  for (int i = 0; i < m_me->n_squares; i++)
    {
      current_square = m_me->squares[i];

      if (current_square->x == square_coordinates.x
	  && current_square->y == square_coordinates.y)
	{
	  return (int) current_square->item;
	}
    }
}

int
Chunk::get_square_item_quantity (struct coordinates square_coordinates)
{
  SquareProto* current_square = NULL;
  
  // On doit trouver le carré.
  for (int i = 0; i < m_me->n_squares; i++)
    {
      current_square = m_me->squares[i];

      if (current_square->x == square_coordinates.x
	  && current_square->y == square_coordinates.y)
	{
	  return (int) current_square->quantity;
	}
    }
}

time_t
Chunk::getLastUse () const
{
  return m_last_use;
}

void
Chunk::generate_texture ()
{
  if (m_chunk_texture == NULL)
    SDL_DestroyTexture (m_chunk_texture);

  SDL_Surface* items[2];
  items[0] = IMG_Load (TEXTURESDIR"/arbre.png");
  items[1] = IMG_Load (TEXTURESDIR"/pierre1.png");

  SDL_Surface* chunk_surface;
  chunk_surface = copy_surface (m_ground);

  SDL_Rect floor_pos = {.x = 0, .y = 0, .w = 24, .h = 24};
  SquareProto* current_square = NULL;

  for (int i = 0; i < m_me->n_squares; i++)
    {
      current_square = m_me->squares[i];

      if (current_square->x > 16 || current_square->y > 16)
	continue;

      switch (current_square->item)
	{
	case 1:
	  floor_pos.x = current_square->x * 24;
	  floor_pos.y = current_square->y * 24;

	  SDL_BlitSurface (items[0], NULL, chunk_surface, &floor_pos);
	  break;

	case 2:
	  floor_pos.x = current_square->x * 24;
	  floor_pos.y = current_square->y * 24;

	  SDL_BlitSurface (items[1], NULL, chunk_surface, &floor_pos);
	  break;

	default:
	  break;
	}
    }

  m_chunk_texture = SDL_CreateTextureFromSurface (m_window_renderer, chunk_surface);

  for (int i = 0; i < 2; i++)
    SDL_FreeSurface (items[i]);
  SDL_FreeSurface (chunk_surface);
}

void
Chunk::generate_ground_surface ()
{
  printf ("ici1 !\n");
  if (m_ground == NULL)
    SDL_FreeSurface (m_ground);
  
  Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif

  m_ground = SDL_CreateRGBSurface (0, 24*16, 24*16, 32,
				   rmask, gmask, bmask, amask);
  if (m_ground == NULL)
    {
      fprintf (stderr, _("CreateRGBSurface failed: "));
      fprintf (stderr, "%s\n", SDL_GetError ());
    }

  SDL_Surface* biomes[2];
  biomes[0] = IMG_Load (TEXTURESDIR"/biome1.png");
  biomes[1] = IMG_Load (TEXTURESDIR"/biome2.png");

  for (int i = 0; i < 2; i++)
    {
      if (biomes[i] == NULL)
	{
	  fprintf (stderr, _("Error while loading biomes’ textures"));
	  fprintf (stderr, "\n");
	}
    }

  SDL_Rect floor_pos = {.x = 0, .y = 0, .w = 24, .h = 24};
  SquareProto* current_square = NULL;

  for (int i = 0; i < m_me->n_squares; i++)
    {
      current_square = m_me->squares[i];

      if (current_square->x > 16 || current_square->y > 16)
	continue;

      switch (current_square->floor)
	{
	case 1:
	  floor_pos.x = current_square->x * 24;
	  floor_pos.y = current_square->y * 24;

	  SDL_BlitSurface (biomes[0], NULL, m_ground, &floor_pos);
	  break;

	case 2:
	  floor_pos.x = current_square->x * 24;
	  floor_pos.y = current_square->y * 24;

	  SDL_BlitSurface (biomes[1], NULL, m_ground, &floor_pos);
	  break;

	default:
	  break;
	}
    }

  // Il faut libérer les surfaces des biomes
  for (int i = 0; i < 2; i++)
    SDL_FreeSurface (biomes[i]);

  printf ("ici2 !\n");
}
