#include <vector>
#include "player.h"
#include "utils.h"

/**
 * Read the pipe if there is something to read and if so, update variables 
 * or save if necessary.
 *
 * @param read_pipe is the read only end of the pipe with the network process.
 * @param players is the vector containing all the connected players.
 * @param save_file_path is the path to the save file.
 * @return a positive number if success, a negative 
 * number if there is an error.
 */
int handle_data_from_network_pipe (int read_pipe, std::vector<Player>& players, const char* save_file_path);

/**
 * Send a move command to others players.
 *
 * @param write_pipe is the write end of the pipe to communicate with the 
 * network.
 * @param hero_coords contains the coordinates of the hero which will be
 * send to others players.
 */
void send_move_command (int write_pipe, struct coordinates hero_coords, int screen_height, int screen_width);
