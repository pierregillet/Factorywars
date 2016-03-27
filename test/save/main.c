#include <stdio.h>
#include "../../src/save.h"
#include "../../src/structures.h"

int
main (int argc, char** argv)
{
  struct coordinates chunk_coordinates, square_coordinates;
  chunk_coordinates.x = 2;
  chunk_coordinates.y = 0;
  square_coordinates.x = 0;
  square_coordinates.y = 4;
  int item_id = 1;

  int ret = set_surface_item (chunk_coordinates, square_coordinates, item_id);

  printf ("ret: %d\n", ret);
  
  return 0;
}
