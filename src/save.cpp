#include "save.h"

int
get_surface_item (struct coordinates chunk_coordinates, struct coordinates square_coordinates)
{
  regex_t regex;
  int reti;
  std::string regex_str;

  const unsigned int LINE_SIZE = 512;
  char line[LINE_SIZE];
  
  bool matched(false);

  find_chunk_line_in_file (chunk_coordinates, line, LINE_SIZE, "save");
  if (line == NULL)
    {
      // Not matched
      return 0;
    }
  // Else: Matched !
  // We need to ignore the chunk coordinates because it could be the same as the square_coordinates
  int old_regex_str_len = regex_str.length ();
  regex_str = std::to_string (square_coordinates.x) + ";" + std::to_string (square_coordinates.y);
  
  reti = regcomp (&regex, regex_str.c_str (), REG_EXTENDED);
  if (reti)
    {
      fprintf (stderr, "Could not compile regex\n");
      return 0;
    }

  regmatch_t regmatch[1];
  reti = regexec (&regex, line+(old_regex_str_len), 1, regmatch, 0);
  if (reti)
    {
      // Not Matched !
      return 0;
    }
  regfree (&regex);

  int spaces = 0;
  int value = 0;
  int pos = 0;
  std::string item_id = "";

  // We isolate the item_id
  for (int i = regmatch[0].rm_eo-1+old_regex_str_len; i>0; i--)
    {
      if (line[i] == ';')
	spaces = 0;
      if (line[i] == ' ')
	spaces++;
      if (spaces == 2)
	{
	  pos = i;
	  break;
	}
    }
  if (spaces != 2)
    return 0;

  for (int i = pos+1; line[i] != ' '; i++)
    {
      item_id += line[i];
    }

  return std::atoi(item_id.c_str ());
}

char*
find_chunk_line_in_file (struct coordinates chunk_coordinates, char* dst, size_t n, char* file_path)
{
  const unsigned int LINE_SIZE = n;
  const unsigned int REGEX_STR_SIZE = 14;
  const unsigned int COORDINATE_STR_SIZE = (REGEX_STR_SIZE-2)/2;
    
  char line[LINE_SIZE], regex_str[REGEX_STR_SIZE], coordinate_str[COORDINATE_STR_SIZE];

  // Filling regex_str
  memset (regex_str, 0, REGEX_STR_SIZE);
  strncat (regex_str, "^", REGEX_STR_SIZE);
  memset (coordinate_str, 0, COORDINATE_STR_SIZE);
  snprintf (coordinate_str, COORDINATE_STR_SIZE, "%d", chunk_coordinates.x);
  strncat (regex_str, coordinate_str, REGEX_STR_SIZE);
  strncat (regex_str, ";", REGEX_STR_SIZE);
  memset (coordinate_str, 0, COORDINATE_STR_SIZE);
  snprintf (coordinate_str, COORDINATE_STR_SIZE, "%d", chunk_coordinates.y);
  strncat (regex_str, coordinate_str, REGEX_STR_SIZE);

  FILE *file = fopen (file_path, "r");
  regex_t regex;
  int reti;
  int matched = 0;

  reti = regcomp (&regex, regex_str, REG_EXTENDED|REG_NOSUB);
  if (reti)
    {
      fprintf (stderr, "Could not compile regex\n");
      return NULL;
    }
  
  while (fgets (line, LINE_SIZE, file) != NULL)
    {
      reti = regexec (&regex, line, 0, NULL, 0);
      if (!reti)
  	{
	  matched = 1;
  	  break;
  	}
      else
	regerror (reti, &regex, line, LINE_SIZE);
    }
  regfree (&regex);
  fclose (file);

  // We breaked only if we reached EOF or if the patern matched
  if (!matched)
    return NULL;
  else
    {
      strncpy (dst, line, LINE_SIZE);
      return dst;
    }
}
