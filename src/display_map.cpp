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
display_background (std::string path, SDL_Texture** table)
{
  for(int i(0) ; i<2; i+=1)
  {
    for(int j(0); j<2; j+=1)
      {
        struct coordinates coords = {.x = i, .y = j};
        int id = get_biome_id (coords, path.c_str ());
        SDL_Texture* display_id = table[id];  
        
        int xmax = i*380 + 380;
        int ymax = j*380 + 380;        
        
        for(int x(i*380); x<xmax; x+=24)
        {
          for(int y(j*380); y<ymax; y+=24)
          {
            blit(x, y,24,24, display_id);
          }
        }
      }  
  
  }  
}  
  
