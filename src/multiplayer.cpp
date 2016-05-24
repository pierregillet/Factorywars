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

      for (Player player : players)
	{
	  if (player.getName () == std::string (token))
	    {
	      strtok (NULL, " ");
	      struct coordinates coords;
	      coords = get_coordinates_from_string (token);
	      player.setCoordinates (coords);
	    }
	}
      break;
    }
  return 0;
}
