#include "save.h"

int
get_surface_item (struct coordinates chunk_coordinates, struct coordinates square_coordinates)
{
  regex_t regex;
  int reti;
  const unsigned int REGEX_STR_SIZE = 512;
  char regex_str[REGEX_STR_SIZE];

  const unsigned int LINE_SIZE = 512;
  const unsigned int COORDINATES_STR_SIZE = 20;
  char line[LINE_SIZE], chunk_coordinates_str[COORDINATES_STR_SIZE];
  
  bool matched(false);

  find_chunk_line_in_file (chunk_coordinates, line, LINE_SIZE, "save");
  if (line == NULL)
    {
      // Not matched
      return 0;
    }
  // Else: Matched !
  // We need to ignore the chunk coordinates because it could be the same as the square_coordinates
  int chunk_coordinates_len = strlen (coordinates_to_string (chunk_coordinates, chunk_coordinates_str, COORDINATES_STR_SIZE));
  
  coordinates_to_string (square_coordinates, regex_str, REGEX_STR_SIZE);
  
  reti = regcomp (&regex, regex_str, REG_EXTENDED);
  if (reti)
    {
      fprintf (stderr, "Could not compile regex\n");
      return 0;
    }

  regmatch_t regmatch[1];
  reti = regexec (&regex, line+(chunk_coordinates_len), 1, regmatch, 0);
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
  for (int i = regmatch[0].rm_eo-1+chunk_coordinates_len; i>0; i--)
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
    
  char line[LINE_SIZE], regex_str[REGEX_STR_SIZE], coordinates_str[REGEX_STR_SIZE];

  // Filling regex_str
  memset (regex_str, 0, REGEX_STR_SIZE);
  strncat (regex_str, "^", REGEX_STR_SIZE);
  coordinates_to_string (chunk_coordinates, coordinates_str, REGEX_STR_SIZE);
  strncat (regex_str, coordinates_str, REGEX_STR_SIZE);

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
      strncpy (dst, line, n);
      return dst;
    }
}

char*
coordinates_to_string (struct coordinates coordinates, char *dst, size_t dst_size)
{
  const unsigned int STR_SIZE = dst_size;
  char str[STR_SIZE], tmp_str[STR_SIZE];
  memset (str, 0, STR_SIZE);
  memset (tmp_str, 0, STR_SIZE);

  snprintf (tmp_str, STR_SIZE, "%d", coordinates.x);
  strncat (str, tmp_str, STR_SIZE);
  strncat (str, ";", STR_SIZE);
  memset (tmp_str, 0, STR_SIZE);
  snprintf (tmp_str, STR_SIZE, "%d", coordinates.y);
  strncat (str, tmp_str, STR_SIZE);

  strncpy (dst, str, dst_size);
  return dst;
}

