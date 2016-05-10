
#include <stdio.h>
#include <string>


/** load_biomes fills the current table with textures
  */
void load_biomes(SDL_Texture** table);

/** display_background displays the map
  * it takes the save path file
  * and a table with biomes
  */ 
void display_background (std::string path, SDL_Texture** table, int x, int y);
