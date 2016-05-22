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
display_items (SDL_Renderer** Renderer, std::string path, SDL_Texture** table_items, struct coordinates chunk_coords, struct coordinates blited_chunk_coords)
{ 
  for (int i(0) ; i <= 384 ; i += 24)
  {
    for (int j(0) ; j <= 384 ; j += 24)
    {
    struct coordinates square_coords = {.x = i , .y = j};
    int item_id = get_surface_item (chunk_coords, square_coords, path.c_str());
    if (item_id > 0)
    {
      SDL_Texture* item_texture = table_items[item_id];
      blit(Renderer, square_coords.x, square_coords.y, 24, 24, item_texture);
    }  
    }
  }
}  
