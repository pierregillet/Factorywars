#include "display_item.h"

void load_items (SDL_Renderer** Renderer, SDL_Texture** table)
{
  table[0] = loadTexture (Renderer, "media/textures/square1.png");
  table[1] = loadTexture (Renderer, "media/textures/square1.png");    
  table[2] = loadTexture (Renderer, "media/textures/square2.png");    
  table[3] = loadTexture (Renderer, "media/textures/square1.png");    
  table[4] = loadTexture (Renderer, "media/textures/square1.png");                  
  if (table[0] == NULL)
    printf("erreur %s\n", SDL_GetError()); 
}

void
display_items (SDL_Renderer** Renderer, std::string path, SDL_Texture** table, int x, int y)
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
	  
	  for(int l(0) ; l < 380 ; l +=24 )
	    {
	      for(int m(0) ; m < 380 ; m +=24 )
	      {
	        struct coordinates square_coords = {.x = l, .y =m};
	        int surface_id = get_surface_item(coords, square_coords, path.c_str());
	        if (surface_id > 0)
	          {
	          SDL_Texture* display_id = table[surface_id]; 
	    	    blit (Renderer, j - square_coords.x, i - square_coords.y, 16, 16,  display_id);
	    	    }
	      }
	    }
    
    
	  

	}
    }
}  
