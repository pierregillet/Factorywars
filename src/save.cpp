#include "save.h"

int
get_surface_item (struct coordinates chunk_coordinates, struct coordinates square_coordinates)
{
  regex_t regex;
  int reti;
  std::string regex_str, line;
  FILE save = fopen ("save", "r");

  regex_str = "^";
  regex_str += std::to_string (chunk_coordinates.x) + ";" + std::to_string (chunk_coordinates.y);

  reti = regcomp (&regex, regex_str.c_str (), REG_EXTENDED|REG_NOSUB);
  if (reti)
    {
      fprintf (stderr, "Could not compile regex\n");
      return 0;
    }

  while ((line = fscanf (save, "%s")) != EOF)
    {
      reti = regexec (&regexec, line);

      if (!reti)
	{
	  f
	}
    }

  regfree (regex);
  
  return 1;
}
