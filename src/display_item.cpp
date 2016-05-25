#include "display_item.h"

void 
load_items (SDL_Renderer** Renderer, SDL_Texture** table)
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
display_items (SDL_Renderer** Renderer,
	       std::string path,
	       SDL_Texture** table_items,
	       struct coordinates screen_origin,
	       struct chunk_info chunk,
	       int l,
	       int m)
{
  for (int i(0) ; i < 16 ; i += 1)
    {
      for (int j(0) ; j < 16 ; j += 1)
	{
	  struct coordinates square_coords = {.x = i , .y = j};
	  int item_id = chunk.squares[i][j];
	  if (item_id > 0 && item_id < 4)
	    {
	      struct coordinates bliting_coords;
	      bliting_coords.x = (m - screen_origin.x % 384) + 24*square_coords.x;
	      bliting_coords.y = (l - screen_origin.y % 384) + 24*square_coords.y;
	      
	      SDL_Texture* item_texture = table_items[item_id];
	      blit(Renderer, bliting_coords, 24, 24, item_texture);
	    }
	}
  }
}
