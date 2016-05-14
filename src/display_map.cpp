#include "save.h"
#include "structures.h"
#include "display_map.h"
#include <stdio.h>

void load_biomes(SDL_Texture** table)
{
  table[0] = loadTexture ("media/textures/biome1.png");
  table[1] = loadTexture ("media/textures/biome1.png");    
  table[2] = loadTexture ("media/textures/biome2.png");    
  table[3] = loadTexture ("media/textures/biome1.png");    
  table[4] = loadTexture ("media/textures/biome1.png");                  
  if (table[0] == NULL)
    printf("erreur %s\n", SDL_GetError()); 
}


void
display_background (std::string path, SDL_Texture** table, int x, int y)
{
  const int NUMBER_OF_SQUARE_PER_ROW = 16;
  const int SQUARE_WIDTH = 24;

  const int chunk_width = NUMBER_OF_SQUARE_PER_ROW * SQUARE_WIDTH;
  const int screen_height = atoi (get_config_value ("height"));
  const int screen_width = atoi (get_config_value ("width"));

  x = (x < 0)? 0 : x;
  y = (y < 0)? 0 : y;

  for(int i(0) ; i < screen_height + y % chunk_width; i += chunk_width)
    {
      for(int j(0); j < screen_width + x % chunk_width; j += chunk_width)
	{
	  struct coordinates hero_coords = {.x = x + j, .y = y + i};

	  struct coordinates coords = get_chunk_coordinates_from_player_movement (hero_coords);
  
	  int id = get_biome_id (coords, path.c_str ());
	  if (id == -1)
	    {
	      fprintf (stderr, "Wrong chunk coordinates.\n");
	      continue;
	    }

	  SDL_Texture* display_id = table[id];

	  blit (j - x % chunk_width, i - y % chunk_width, chunk_width, chunk_width,  display_id);
	}
    }
}  
