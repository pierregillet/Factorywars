#include "multiplayer.h"

int
handle_data_from_network_pipe (int read_pipe, std::vector<Player>& players,
			       const char* save_file_path)
{
  const int BUFFER_SIZE = 512;

  char buffer[BUFFER_SIZE], *token;
  if (read_pipe_until_null (buffer, BUFFER_SIZE, read_pipe) == 0)
    return 0;

  int command_type = get_command_type (buffer);

  struct coordinates coords;
  Player new_player;

  switch (command_type)
    {
    case -1:
      return -1;
      break;

    case 4:
      token = strtok (buffer, " "); // The command
      token = strtok (NULL, " ");   // Player name

      coords = {.x = 0, .y = 0};

      new_player = Player (100, std::string (token), coords, 10);
      players.push_back (new_player);

      break;

    case 5:
      token = strtok (buffer, " ");
      token = strtok (NULL, " ");

      for (unsigned int i = 0; i < players.size (); i++)
	{
	  if (players[i].getName () == std::string (token))
	    {
	      token = strtok (NULL, " ");
	      struct coordinates coords;

	      coords = get_coordinates_from_string (token);

	      players[i].setCoordinates (coords);
	    }
	}
      break;
    }
  
  return 1;
}

void send_move_command (int write_pipe, struct coordinates screen_origin, int screen_height, int screen_width)
{
  const int COORDINATES_STR_SIZE = 128;
  const int MSG_LEN = 256;

  struct coordinates hero_coords;
  hero_coords.x = screen_origin.x + screen_width / 2;
  hero_coords.y = screen_origin.y + screen_height / 2;
    
  char coordinates_str[COORDINATES_STR_SIZE], msg[MSG_LEN];

  coordinates_to_string (hero_coords, coordinates_str, COORDINATES_STR_SIZE);

  snprintf (msg, MSG_LEN, "MOVE %s", coordinates_str);

  write (write_pipe, msg, strlen (msg) + 1);
}
