/**
 * @file
 * @author Corentin Bocquillon <0x539@nybble.fr>
 * @author Pierre Gillet <pierre.gillet+factorywars@linuxw.info>
 * @author Loup Fourment
 *
 * @section LICENSE
 *
 * Copyright (C) 2016 Corentin Bocquillon <0x539@nybble.fr>
 *
 * Copyright (C) 2016 Loup Fourment
 *
 * Copyright (C) 2016 Pierre Gillet <pierre.gillet+factorywars@linuxw.info>
 *
 * factorywars is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * factorywars is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with factorywars.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 *
 * game.cpp contains functions used only to start a game.
 */

#include "game.h"


int 
run_gui ()
{
  const int config_value_len = 256;
  char config_value[config_value_len];
  struct size screen_dimensions;

  get_config_value ("height", config_value, config_value_len);
  screen_dimensions.y = atoi (config_value);

  get_config_value ("width", config_value, config_value_len);
  screen_dimensions.x = atoi (config_value);

  SDL_Window *Window = NULL;
  SDL_Renderer *Renderer = NULL;

  init (&Window, &Renderer, &screen_dimensions);
  
  const int save_name_len = 256;
  char save_name[save_name_len];
  int ret;

  int stay = 1;

  while (stay)
    {
      ret = display_main_menu (Renderer, screen_dimensions,
			       save_name, save_name_len);
      if (ret == 0)
	{
	  quit_sdl (&Window, &Renderer);
	  return 0;
	}

      // else if (ret == 2)
      //   snprintf (map_path, save_path_len, "%s/%s", save_path, "map");
      // else
      //   strncpy (map_path, "protosave", save_path_len);

      ret = run_game (Renderer, save_name, &screen_dimensions);

      if (ret == 0)
	stay = 0;
    }

  quit_sdl (&Window, &Renderer);
  
  return 0;
}

int
run_game (SDL_Renderer* main_renderer, const char* save_name,
	  struct size* screen_dimensions)
{
  SDL_Texture* textures[4][10];
  load_game_textures (main_renderer, textures);

  SDL_Texture *player_texture = NULL;
  player_texture = textures[0][1];

  int ret;
  
  unsigned int start_time = SDL_GetTicks ();

  struct coordinates screen_center; 
  screen_center.x = screen_dimensions->x / 2;
  screen_center.y = screen_dimensions->y / 2;
  
  struct coordinates screen_origin, hero_coords;

  // On charge la sauvegarde du joueur
  const int config_value_len = 256;
  char config_value[config_value_len];

  std::vector<Player> players (1, Player ());

  // S’il y a un fichier de sauvegarde pour le joueur, on le charge
  get_config_value ("name", config_value, config_value_len);

  std::string player_save_directory = get_save_directory_path () + std::string (save_name);
  player_save_directory += "/" + std::string (config_value);

  players[0].setSaveFilePath (player_save_directory);
  players[0].read_save ();

  // On utilise les coordonnées du joueur
  if (players[0].getCoordinates ().x > screen_center.x)
    {
      hero_coords.x = screen_center.x;
      screen_origin.x = players[0].getCoordinates ().x - hero_coords.x;
    }
  else
    {
      hero_coords.x = players[0].getCoordinates ().x;
      screen_origin.x = 0;
    }

  if (players[0].getCoordinates ().y > screen_center.y)
    {
      hero_coords.y = screen_center.y;
      screen_origin.y = players[0].getCoordinates ().y - hero_coords.y;
    }
  else
    {
      hero_coords.y = players[0].getCoordinates ().y;
      screen_origin.y = 0;
    }

  // On charge la sauvegarde de la carte
  Map map (save_name, main_renderer);
  
  /* 
   * keys_state contains only 4 elements
   * because we only use 4 keys as of now
   * keys_state[0] -> SDLK_UP
   * keys_state[1] -> SDLK_DOWN
   * keys_state[2] -> SDLK_LEFT
   * keys_state[3] -> SDLK_RIGHT
   * 
   * Use the enumeration KEYBOARD_BUTTONS.
   */
  bool keys_state[5] = {0};

  /* 
   * clicks_state[0] -> SDL_BUTTON_LEFT
   * clicks_state[1] -> SDL_BUTTON_MIDDLE
   * clicks_state[2] -> SDL_BUTTON_RIGHT
   * clicks_state[3] -> SDL_BUTTON_X1
   * clicks_state[4] -> SDL_BUTTON_X2
   * 
   * Use the enumeration MOUSE_BUTTONS.
   */
  bool clicks_state[5] = {0};

  struct map_coordinates click_map_coords;

  TTF_Font* ttf_freesans = TTF_OpenFont (FONTSDIR"/FreeSans.ttf", 18);

  if (ttf_freesans == NULL)
    {
      fprintf (stderr, "Error while loading FreeSans.ttf\n");
      return 0;
    }

  // Variables used for FPS calculation.
  unsigned int fps_lasttime = SDL_GetTicks(); //the last recorded time.
  unsigned int fps_current; //the current FPS.
  unsigned int fps_frames = 0; //frames passed since the last recorded fps.

  // We need to display the map at the beginning
  display_background (main_renderer, map,
		      screen_origin, *screen_dimensions);

  // Display character
  // blit (main_renderer, screen_center, 25, 41, player_texture);
  
  // Display HUD
  struct coordinates toolbar_origin;
  toolbar_origin.x = (int) (screen_dimensions->x / 4);
  toolbar_origin.y = (int) (screen_dimensions->y - (screen_dimensions->x / 2 * 0.11));

  struct size
    toolbar_size = {.x = (int) (screen_dimensions->x / 2),
		    .y = (int) (screen_dimensions->x / 2 * 0.11)};

  blit (main_renderer, toolbar_origin, toolbar_size.x,
	toolbar_size.y,	textures[3][0]);

  display_fps (main_renderer, ttf_freesans, &fps_lasttime,
	       &fps_current, &fps_frames);
      
  SDL_RenderPresent (main_renderer);

  while (handle_events (textures, &player_texture, keys_state, clicks_state,
			screen_dimensions, &screen_origin,
			&click_map_coords, players) != 0)
    {
      start_time = SDL_GetTicks();

      // Keyboard handling
      for (int i = 0; i < 4; i++)
	{
	  if (keys_state[i])
	    {
              move_coordinates_on_keydown (&screen_origin,
					   keys_state,
					   *screen_dimensions,
					   players[0]);

	      // send_move_command (network_write_pipe, screen_origin,
	      // 		    screen_dimensions.y, screen_dimensions.x);
	      break;
	    }
	}

      if (keys_state[key_escape])
	{
	  ret = display_in_game_menu (main_renderer, *screen_dimensions);
	  keys_state[key_escape] = 0;

	  if (ret == 0)
	    break;

	  else if (ret == 1)
	    {
	      map.save ();
	      save_players (players, save_name);
	    }

	  else if (ret == 2)
	    {
	      map.save ();
	      save_players (players, save_name);
	      break;
	    }

	  else if (ret == 4)
	    break;
	}
      // End of keyboard handling

      // Left click handling
      if (clicks_state[0])
	{
	  if (map.get_surface_item (click_map_coords.chunk,
				    click_map_coords.square) == -1)
	    {
	      map.set_surface_item(click_map_coords.chunk,
				   click_map_coords.square,
				   1, 1);
	    }
	    
	  clicks_state[0] = 0;
	}
      // End of left click handling
      
      // Right click handling
      if (clicks_state[2])
	{
	  if (map.get_surface_item (click_map_coords.chunk,
				    click_map_coords.square) != -1)
	    {
	      map.set_surface_item(click_map_coords.chunk,
				   click_map_coords.square,
				   -1, 0);
	    }
	  clicks_state[2] = 0;
	}
      // End of right click handling

      // // Network pipe handling
      // if (handle_data_from_network_pipe (network_read_pipe, players) > 0)
      // 	{
      // 	  display_background (&main_renderer, &map,
      // 			      textures, screen_origin,
      // 			      screen_dimensions.y, screen_dimensions.x);
      // 	  blit (main_renderer, hero_coords, 25, 41, player_texture);
      // 	  blit (main_renderer,
      // 		toolbar_origin,
      // 		toolbar_size.x,
      // 		toolbar_size.y,
      // 		textures[3][0]);
	      
      // 	  display_players (players, screen_origin,
      // 			   main_renderer, player_texture,
      // 			   screen_dimensionsx);
      // 	}

      display_background (main_renderer, map,
			  screen_origin, *screen_dimensions);

      blit (main_renderer,
	    toolbar_origin,
	    toolbar_size.x,
	    toolbar_size.y,
	    textures[3][0]);

      display_players (players, screen_origin,
		       main_renderer, player_texture,
		       *screen_dimensions);

      display_fps (main_renderer, ttf_freesans, &fps_lasttime,
		   &fps_current, &fps_frames);
      SDL_RenderPresent (main_renderer);	  
      SDL_Delay (1/200);
    }

  destroy_game_textures (player_texture, textures);

  if (ret == 4)
    return 1;

  return 0;
}

void
destroy_game_textures (SDL_Texture* player_texture, SDL_Texture* textures[][10])
{
  for (int i = 0 ; i < 4 ; i++)
    {
      SDL_DestroyTexture (textures[0][i]);
    }

  for (int i = 0 ; i < 5 ; i++)
    {
      SDL_DestroyTexture (textures[1][i]);
    }

  for (int i = 0 ; i < 4 ; i++)
    {
      SDL_DestroyTexture (textures[2][i]);
    }

  for (int i = 0 ; i < 1 ; i++)
    {
      SDL_DestroyTexture (textures[3][i]);
    }

  SDL_DestroyTexture (player_texture);
}

void
move_coordinates_on_keydown (struct coordinates* screen_origin,
			     bool* keys_state,
			     const struct size screen_dimensions,
			     Player& me)
{
  struct coordinates hero_screen_coords = me.getCoordinates ();
  hero_screen_coords.x -= screen_origin->x;
  hero_screen_coords.y -= screen_origin->y;

  if (hero_screen_coords.x >= screen_dimensions.x / 2)
    {
      screen_origin->x -= (keys_state[key_left])? 5 : 0;
      screen_origin->x += (keys_state[key_right])? 5 : 0;
    }
  if (hero_screen_coords.y >= screen_dimensions.y / 2)
    {
      screen_origin->y -= (keys_state[key_up])? 5 : 0;
      screen_origin->y += (keys_state[key_down])? 5 : 0;
    }

  if (screen_origin->x <= 0)
    {
      hero_screen_coords.x -= (keys_state[key_left])? 5 : 0;
      hero_screen_coords.x += (keys_state[key_right])? 5 : 0;
    }
  if (screen_origin->y <= 0)
    {
      hero_screen_coords.y -= (keys_state[key_up])? 5 : 0;
      hero_screen_coords.y += (keys_state[key_down])? 5 : 0;
    }

  
  hero_screen_coords.x = (hero_screen_coords.x < 0)? 0 : hero_screen_coords.x;
  hero_screen_coords.y = (hero_screen_coords.y < 0)? 0 : hero_screen_coords.y;

  if (hero_screen_coords.x > screen_dimensions.x / 2)
    hero_screen_coords.x = screen_dimensions.x / 2;

  if (hero_screen_coords.y > screen_dimensions.y / 2)
    hero_screen_coords.y = screen_dimensions.y / 2;


  struct coordinates hero_coords;
  hero_coords.x = hero_screen_coords.x + screen_origin->x;
  hero_coords.y = hero_screen_coords.y + screen_origin->y;

  me.setCoordinates (hero_coords);
}

void
display_players (std::vector<Player>& players, struct coordinates screen_origin,
		 SDL_Renderer* renderer, SDL_Texture* player_texture,
		 struct size screen_dimensions)
{
  struct coordinates player_coordinates;

  for (Player player : players)
    {
      player_coordinates = player.getCoordinates ();

      if (player_coordinates.x >= screen_origin.x
	  && player_coordinates.x <= screen_origin.x + screen_dimensions.x)
	{
	  if (player_coordinates.y >= screen_origin.y
	      && player_coordinates.y <= screen_origin.y + screen_dimensions.y)
	    {
	      player_coordinates.x -= screen_origin.x;
	      player_coordinates.y -= screen_origin.y;

	      blit (renderer, player_coordinates, 25, 41,
		    player_texture);
	    }
	}
    }
}

int
save_players (std::vector<Player>& players, std::string save_dir_path)
{
  for (Player player : players)
    {
      if (player.getName ().empty ())
	continue;

      if (player.getSaveFilePath ().empty ())
	player.setSaveFilePath (save_dir_path + "/" + player.getName ());

      player.save ();
    }
}
