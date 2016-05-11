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
  for(int i(0) ; i<2; i+=1)
  {
    for(int j(0); j<2; j+=1)
      {
        struct coordinates hero_coords= {.x = -x+384*i, .y = -y+384*j};
        struct coordinates coords = get_chunk_coordinates_from_player_movement (hero_coords);
        int id = get_biome_id (coords, path.c_str ());

        SDL_Texture* display_id = table[id];  
        
        long xmax = x + 384 + i*384;
        long ymin = y + 384 + j*384; 
                
        for (long l(x+384*i) ; l < xmax ; l+=24)
          {
            for (long m(y+384*j) ; m < ymin ; m+=24)
            {
              blit (l, m, 24, 24,display_id); 
            }
          }
      }  
  }
}  
  
