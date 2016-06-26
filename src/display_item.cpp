#include "display_item.h"

void
display_items (SDL_Renderer** Renderer,
	       std::string path,
	       SDL_Texture* textures[][10],
	       struct coordinates screen_origin,
	       struct chunk_info chunk,
	       int l,
	       int m)
{
  for (int i(16) ; i > 0 ; i -= 1)
    {
      for (int j(0) ; j < 16 ; j += 1)
	{
	  int item_id = chunk.squares[i][j];
	  if (item_id > 0 && item_id < 4)
	    {
	      int blit_x = (item_id == 1)? 48 :24;
	      int blit_y = (item_id == 1)? 48 :24;
	      struct coordinates bliting_coords;
	      
	      bliting_coords.x = (m - screen_origin.x % 384) + 24 * i - blit_x / 4;
	      bliting_coords.y = (l - screen_origin.y % 384) + 24 * j - blit_y / 2;
	      
	      SDL_Texture* item_texture = textures[2][item_id];
	      blit (Renderer, bliting_coords, blit_x, blit_y, item_texture);
	    }
	}
  }
}
