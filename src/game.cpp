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

void
load_game_textures (SDL_Renderer* main_renderer,
		    SDL_Texture* textures[][10])
{
  const char*
    player_textures_paths[] = {TEXTURESDIR"/LEFT.png",
			       TEXTURESDIR"/RIGHT.png",
			       TEXTURESDIR"/LEFT.png",
			       TEXTURESDIR"/RIGHT.png"};

  const char*
    biomes_textures_paths[] = {TEXTURESDIR"/biome1.png",
			       TEXTURESDIR"/biome1.png",
			       TEXTURESDIR"/biome2.png",
			       TEXTURESDIR"/biome1.png",
			       TEXTURESDIR"/biome1.png"};

  const char*
    objects_textures_paths[] = {TEXTURESDIR"/arbre.png",
				TEXTURESDIR"/pierre1.png",
				TEXTURESDIR"/pierre2.png",
				TEXTURESDIR"/pierre3.png"};

  const char*
    hud_textures_paths[] = {TEXTURESDIR"/toolbar.png"};

  int i = 0;
  
  for (const char* path : player_textures_paths)
    {
      textures[0][i] = load_texture (main_renderer,
				     path);
      i++;
    }

  i = 0;
  
  for (const char* path : biomes_textures_paths)
    {
      textures[1][i] = load_texture (main_renderer,
				     path);
      i++;
    }

  i = 0;

  for (const char* path : objects_textures_paths)
    {
      textures[2][i] = load_texture (main_renderer,
				     path);
      i++;
    }

  i = 0;
  
  for (const char* path : hud_textures_paths)
    {
      textures[3][i] = load_texture (main_renderer,
				     path);
      i++;
    }
}

int
run_game (SDL_Renderer* main_renderer, const char* save_path,
	  struct size screen_dimensions)
{
  SDL_Texture* textures[4][10];
  load_game_textures (main_renderer, textures);

  SDL_Texture *player_texture = NULL;
  player_texture = textures[0][1];

  int ret;
  
  unsigned int start_time = SDL_GetTicks ();

  struct coordinates screen_center; 
  screen_center.x = screen_dimensions.x / 2;
  screen_center.y = screen_dimensions.y / 2;
  
  struct coordinates screen_origin = {.x = 0,
				      .y = 0};

  const int config_value_len = 256;
  char config_value[config_value_len];

  std::vector<Player> players (1, Player ());
  get_config_value ("name", config_value, config_value_len);
  players[0].setName (config_value);
  players[0].setCoordinates ({screen_center.x, screen_center.y});
  
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

  const int map_path_len = 256;
  char map_path[map_path_len];

  snprintf (map_path, map_path_len, "%s/%s", save_path, "map");

  Map map = MAP__INIT;
  read_save_file (&map, map_path);

  
  // We need to display the map at the beginning
  display_background (&main_renderer, &map,
		      textures, screen_origin,
		      screen_dimensions.y, screen_dimensions.x);

  // Display character
  blit (main_renderer, screen_center, 25, 41, player_texture);
  
  // Display HUD
  struct coordinates toolbar_origin;
  toolbar_origin.x = (int) (screen_dimensions.x / 4);
  toolbar_origin.y = (int) (screen_dimensions.y - (screen_dimensions.x / 2 * 0.11));

  struct size
    toolbar_size = {.x = (int) (screen_dimensions.x / 2),
		    .y = (int) (screen_dimensions.x / 2 * 0.11)};

  blit (main_renderer, toolbar_origin, toolbar_size.x, toolbar_size.y,
	textures[3][0]);

  display_fps (main_renderer, &start_time, ttf_freesans);
  SDL_RenderPresent (main_renderer);

  while (handle_events (textures, &player_texture, keys_state,
			clicks_state, screen_dimensions.y,
			screen_dimensions.x, &screen_origin,
			&click_map_coords, players) != 0)
    {
      start_time = SDL_GetTicks();

      // Keyboard handling
      for (int i = 0; i < 4; i++)
	{
	  if (keys_state[i])
	    {
              move_coordinates_on_keydown (&screen_origin, keys_state,
					   screen_dimensions.y, screen_dimensions.x,
					   players[0]);

	      // send_move_command (network_write_pipe, screen_origin,
	      // 		    screen_dimensions.y, screen_dimensions.x);
	      break;
	    }
	}

      if (keys_state[key_escape])
	{
	  ret = display_in_game_menu (main_renderer, screen_dimensions);
	  keys_state[key_escape] = 0;

	  if (ret == 0)
	    break;

	  else if (ret == 1)
	    save_to_file (&map, map_path);

	  else if (ret == 2)
	    {
	      save_to_file (&map, map_path);
	      break;
	    }

	  else if (ret == 4)
	    break;
	}
      // End of keyboard handling

      // Left click handling
      if (clicks_state[0])
	{
	  if (get_surface_item (click_map_coords.chunk,
				click_map_coords.square,
				&map) == -1)
	    {
	      set_surface_item(click_map_coords.chunk,
			       click_map_coords.square,
			       1, &map);
	    }
	    
	  clicks_state[0] = 0;
	}
      // End of left click handling
      
      // Right click handling
      if (clicks_state[2])
	{
	  if (get_surface_item (click_map_coords.chunk,
				click_map_coords.square,
				&map) != -1)
	    {
	      set_surface_item(click_map_coords.chunk,
			       click_map_coords.square,
			       -1, &map);
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

      display_background (&main_renderer, &map,
			  textures, screen_origin,
			  screen_dimensions.y, screen_dimensions.x);
      blit (main_renderer,
	    players[0].getCoordinates (),
	    25,
	    41,
	    player_texture);

      blit (main_renderer,
	    toolbar_origin,
	    toolbar_size.x,
	    toolbar_size.y,
	    textures[3][0]);

      display_players (players, screen_origin,
		       main_renderer, player_texture,
		       screen_dimensions);

      display_fps (main_renderer,
		   &start_time,
		   ttf_freesans);
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

int
move_coordinates_on_keydown (struct coordinates* screen_origin,
			     bool* keys_state, int screen_height,
			     int screen_width, Player& me)
{
  struct coordinates hero_coords = me.getCoordinates ();

  if (hero_coords.x >= screen_width / 2 && hero_coords.y >= screen_height / 2)
    {
      screen_origin->y -= (keys_state[key_up])? 5 : 0;
      screen_origin->y += (keys_state[key_down])? 5 : 0;
      screen_origin->x -= (keys_state[key_left])? 5 : 0;
      screen_origin->x += (keys_state[key_right])? 5 : 0;
    }
  if (screen_origin->y <= 0 || screen_origin->x <= 0)
    {
      hero_coords.y -= (keys_state[key_up])? 5 : 0;
      hero_coords.y += (keys_state[key_down])? 5 : 0;
      hero_coords.x -= (keys_state[key_left])? 5 : 0;
      hero_coords.x += (keys_state[key_right])? 5 : 0;
    }
  
  hero_coords.x = (hero_coords.x < 0 )? 0 : hero_coords.x;
  hero_coords.y = (hero_coords.y < 0 )? 0 : hero_coords.y;

  if (hero_coords.x > screen_width / 2)
    hero_coords.x = screen_width / 2;

  if (hero_coords.y > screen_height / 2)
    hero_coords.y = screen_height / 2;

  me.setCoordinates (hero_coords);
 
  return 1;
}

void
display_players (std::vector<Player>& players, struct coordinates screen_origin,
		 SDL_Renderer* renderer, SDL_Texture* player_texture,
		 struct size screen_dimensions)
{
  std::string my_name = players[0].getName ();

  struct coordinates player_coordinates;

  // Display players
  for (Player player : players)
    {
      if (player.getName () == my_name)
	continue;

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
