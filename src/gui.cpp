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
 * gui.cpp contains the gui’s code.
 */

#include "gui.h"

SDL_Texture*
loadTexture (SDL_Renderer** main_renderer, std::string path)
{
  SDL_Texture* new_texture = NULL;
  SDL_Surface* loaded_surface = IMG_Load(path.c_str ());

  new_texture = SDL_CreateTextureFromSurface (*main_renderer, loaded_surface);
  SDL_FreeSurface (loaded_surface);
  
  return new_texture;
}

void
loadMedia (SDL_Renderer** main_renderer,
	   SDL_Texture* textures[][10])
{
  std::string
    player_textures_paths[] = {"media/textures/LEFT.png",
			       "media/textures/RIGHT.png",
			       "media/textures/LEFT.png",
			       "media/textures/RIGHT.png"};

  std::string
    biomes_textures_paths[] = {"media/textures/biome1.png",
			       "media/textures/biome1.png",
			       "media/textures/biome2.png",
			       "media/textures/biome1.png",
			       "media/textures/biome1.png"};

  std::string
    objects_textures_paths[] = {"media/textures/arbre.png",
				"media/textures/pierre1.png",
				"media/textures/pierre2.png",
				"media/textures/pierre3.png"};

  std::string
    hud_textures_paths[] = {"media/hud/toolbar.png"};

  int i = 0;
  
  for (std::string path : player_textures_paths)
    {
      textures[0][i] = loadTexture (main_renderer,
				    path);
      i++;
    }

  i = 0;
  
  for (std::string path : biomes_textures_paths)
    {
      textures[1][i] = loadTexture (main_renderer,
				    path);
      i++;
    }

  i = 0;

  for (std::string path : objects_textures_paths)
    {
      textures[2][i] = loadTexture (main_renderer,
				    path);
      i++;
    }

  i = 0;
  
  for (std::string path : hud_textures_paths)
    {
      textures[3][i] = loadTexture (main_renderer,
				    path);
      i++;
    }
}

void
init (SDL_Window** main_window,
      SDL_Renderer** main_renderer,
      const int screen_height,
      const int screen_width,
      SDL_Texture* textures[][10])
{
  if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
      fprintf (stderr, "Error: %s\n", SDL_GetError ());
    }
	
  // if the SDL launched correctly
  *main_window = SDL_CreateWindow ("Factorywars",
				   SDL_WINDOWPOS_UNDEFINED,
				   SDL_WINDOWPOS_UNDEFINED,
				   screen_width,
				   screen_height,
				   SDL_WINDOW_SHOWN
				   /*| SDL_WINDOW_RESIZABLE*/);
	  
  if (*main_window == NULL) 
    {
      fprintf (stderr, "Couldn’t create window : %s\n", SDL_GetError());
      SDL_Quit();
    }
	  
  // if window has been created without errors
  *main_renderer = SDL_CreateRenderer (*main_window, -1,
				       SDL_RENDERER_ACCELERATED
				       | SDL_RENDERER_PRESENTVSYNC);

  SDL_SetRenderDrawColor (*main_renderer, 0xFF,0xFF,0xFF,0xFF);

  if (!IMG_Init (IMG_INIT_PNG) & IMG_INIT_PNG)
    fprintf (stderr, "Error while initializing SDL_image library.\n");

  if (TTF_Init () == -1)
    fprintf (stderr, "Error while initializing SDL_ttf library.\n");

  loadMedia (main_renderer, textures);
}

int
move_coordinates_on_keydown (struct coordinates* screen_origin,
			     bool* keys_state, int screen_height,
			     int screen_width, struct size* hero_coords)
{
  if (hero_coords->x >= screen_width / 2 && hero_coords->y >= screen_height / 2)
    {
      screen_origin->y -= (keys_state[0])? 5 : 0;
      screen_origin->y += (keys_state[1])? 5 : 0;
      screen_origin->x -= (keys_state[2])? 5 : 0;
      screen_origin->x += (keys_state[3])? 5 : 0;
    }
  if (screen_origin->y <= 0 || screen_origin->x <= 0)
    {
      hero_coords->y -= (keys_state[0])? 5 : 0;
      hero_coords->y += (keys_state[1])? 5 : 0;
      hero_coords->x -= (keys_state[2])? 5 : 0;
      hero_coords->x += (keys_state[3])? 5 : 0;
    }
  
  hero_coords->x = (hero_coords->x < 0 )? 0 : hero_coords->x;
  hero_coords->y = (hero_coords->y < 0 )? 0 : hero_coords->y;

  if (hero_coords->x > screen_width / 2)
    hero_coords->x = screen_width / 2;

  if (hero_coords->y > screen_height / 2)
    hero_coords->y = screen_height / 2;
 
  return 1;
}

int
blit (SDL_Renderer** main_renderer,
      struct size blit_origin,
      int width,
      int height,
      SDL_Texture* texture)
{
  SDL_Rect rectangle = {.x = blit_origin.x,
			.y = blit_origin.y,
			.w = width,
			.h = height};
  
  SDL_RenderSetViewport (*main_renderer, &rectangle);
  SDL_RenderCopy (*main_renderer, texture, NULL, NULL);

  return 1;
}

void
quit_sdl (SDL_Window** main_window,
	  SDL_Renderer** main_renderer,
	  SDL_Texture** current_texture,
	  SDL_Texture* textures[][10])
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

  SDL_DestroyTexture (*current_texture);
  SDL_DestroyRenderer (*main_renderer);
  SDL_DestroyWindow (*main_window);

  TTF_Quit ();
  IMG_Quit ();
  SDL_Quit ();
}

int 
run_gui (int read_pipe,
	 int write_pipe,
	 std::vector<Player>& players)
{
  const int config_value_len = 256;
  char config_value[config_value_len];

  get_config_value ("height", config_value, config_value_len);
  int screen_height = atoi (config_value);

  get_config_value ("width", config_value, config_value_len);
  int screen_width = atoi (config_value);

  struct size screen_dimensions = {.x = screen_width,
				   .y = screen_height};

  Map map = MAP__INIT;
  read_save_file (&map, "protosave");

  SDL_Window *Window = NULL;
  SDL_Renderer* Renderer = NULL;

  SDL_Texture* textures[4][10];

  init (&Window, &Renderer, screen_height,
	screen_width, textures);

  struct size screen_center; 
  screen_center.x = screen_width / 2;
  screen_center.y = screen_height / 2;
 
  struct coordinates screen_origin = {.x = 0,
				      .y = 0};

  struct size hero_coords = {.x = screen_center.x,
			     .y = screen_center.y};
  
  SDL_Texture *current_texture = NULL;
  current_texture = textures[0][1];
  
  /* 
   * keys_state contains only 4 elements
   * because we only use 4 keys as of now
   * keys_state[0] -> SDLK_UP
   * keys_state[1] -> SDLK_DOWN
   * keys_state[2] -> SDLK_LEFT
   * keys_state[3] -> SDLK_RIGHT
   */
  bool keys_state[4] = {0};

  /* 
   * clicks_state[0] -> SDL_BUTTON_LEFT
   * clicks_state[1] -> SDL_BUTTON_MIDDLE
   * clicks_state[2] -> SDL_BUTTON_RIGHT
   * clicks_state[3] -> SDL_BUTTON_X1
   * clicks_state[4] -> SDL_BUTTON_X2
   */
  bool clicks_state[5] = {0};

  struct map_coordinates click_map_coords;

  // Debug menu
  // display_main_menu (&Renderer, screen_dimensions);
  // SDL_Delay (5000);

  // We need to display the map at the beginning
  display_background (&Renderer, &map,
		      textures, screen_origin,
		      screen_height, screen_width);

  // Display character
  blit (&Renderer, screen_center, 25, 41, current_texture);
  
  // Display HUD
  struct size toolbar_origin;
  toolbar_origin.x = (int) (screen_width / 4);
  toolbar_origin.y = (int) (screen_height - (screen_width / 2 * 0.11)); 
  struct size
    toolbar_size = {.x = (int) (screen_width / 2),
		    .y = (int) (screen_width / 2 * 0.11)};
  blit (&Renderer,
	toolbar_origin,
	toolbar_size.x,
	toolbar_size.y,
	textures[3][0]);

  SDL_RenderPresent (Renderer);

  int need_to_blit;

  while (handle_events (textures, &current_texture, keys_state,
			clicks_state, screen_height,
			screen_width, &screen_origin,
			&click_map_coords, players) != 0)
    {
      need_to_blit = 0;

      // Keyboard handling
      for (int i = 0; i < 4; i++)
	{
	  if (keys_state[i])
	    {
              move_coordinates_on_keydown (&screen_origin, keys_state,
					   screen_height, screen_width,
					   &hero_coords);

	      send_move_command (write_pipe, screen_origin, screen_height,
				 screen_width);

	      display_background (&Renderer, &map,
				  textures, screen_origin,
				  screen_height, screen_width);

	      blit (&Renderer, hero_coords, 25, 41, current_texture);

	      blit (&Renderer, toolbar_origin, toolbar_size.x, toolbar_size.y,
		    textures[3][0]);

	      display_players (players, screen_origin,
			       &Renderer, current_texture,
			       screen_height, screen_width);

	      need_to_blit = 1;
	      break;
	    }
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
			       1,
			       &map);
	      display_background (&Renderer, &map,
				  textures, screen_origin,
				  screen_height, screen_width);
	      
	      blit (&Renderer,
		    hero_coords,
		    25,
		    41,
		    current_texture);
	      blit (&Renderer,
		    toolbar_origin,
		    toolbar_size.x,
		    toolbar_size.y,
		    textures[3][0]);

	      display_players (players, screen_origin, &Renderer, current_texture,
			       screen_height, screen_width);

	      need_to_blit = 1;
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
			       -1,
			       &map);
	      display_background (&Renderer, &map,
				  textures, screen_origin,
				  screen_height, screen_width);
	      blit (&Renderer,
		    hero_coords,
		    25,
		    41,
		    current_texture);

	      blit (&Renderer,
		    toolbar_origin,
		    toolbar_size.x,
		    toolbar_size.y,
		    textures[3][0]);

	      display_players (players, screen_origin,
			       &Renderer, current_texture,
			       screen_height, screen_width);

	      need_to_blit = 1;
	    }
	  clicks_state[2] = 0;
	}
      // End of right click handling

      // Network pipe handling
      if (handle_data_from_network_pipe (read_pipe, players) > 0)
      	{
	  display_background (&Renderer, &map,
			      textures, screen_origin,
			      screen_height, screen_width);
	  blit (&Renderer, hero_coords, 25, 41, current_texture);
	  blit (&Renderer,
		toolbar_origin,
		toolbar_size.x,
		toolbar_size.y,
		textures[3][0]);
	      
	  display_players (players, screen_origin,
			   &Renderer, current_texture,
			   screen_height, screen_width);
	  need_to_blit = 1;
      	}

      if (need_to_blit)
	SDL_RenderPresent (Renderer);

      SDL_Delay (1/200);
    }

  quit_sdl (&Window, &Renderer, &current_texture, textures);
  
  return 0;
}

void
display_players (std::vector<Player>& players, struct coordinates screen_origin,
		 SDL_Renderer** renderer, SDL_Texture* player_texture,
		 int screen_height, int screen_width)
{
  std::string my_name = players[0].getName ();

  struct coordinates player_coordinates;
  struct size player_placement;

  // Display players
  for (Player player : players)
    {
      if (player.getName () == my_name)
	continue;

      player_coordinates = player.getCoordinates ();

      if (player_coordinates.x >= screen_origin.x
	  && player_coordinates.x <= screen_origin.x + screen_width)
	{
	  if (player_coordinates.y >= screen_origin.y
	      && player_coordinates.y <= screen_origin.y + screen_height)
	    {
	      player_coordinates.x -= screen_origin.x;
	      player_coordinates.y -= screen_origin.y;

	      player_placement = {.x = (int) player_coordinates.x,
				  .y = (int) player_coordinates.y};
	      blit (renderer, player_placement, 25, 41,
		    player_texture);
	    }
	}
    }
}

// WORK IN PROGRESS : just needs confirmation on how we manage the save with protobuf.
void
display_ground (SDL_Renderer* main_renderer,
		SDL_Texture* textures[][10],
		const struct size screen_dimensions,
		struct coordinates player_coordinates,
		Map map)
{
  const unsigned int NUMBER_OF_SQUARES_PER_ROW = 16; // Per chunk
  unsigned int default_square_width = 24; // In pixels

  // TODO : Make square_width change in function of zoom level.

  struct unsigned_size squares_to_draw = {.x = screen_dimensions.x / default_square_width,
					  .y = screen_dimensions.y / default_square_width};

  // Load the squares depending on the player position.
  struct size square_top_left_corner = {.x = 0,
					.y = 0};
  unsigned int biome_texture; // Contains the square's biome's number.

  for (int row = 0; row <= squares_to_draw.y; row ++)
    {
      for (int column = 0; column <= squares_to_draw.x; column ++)
	{
	  // biome_texture = ;
	  
	  blit (&main_renderer, square_top_left_corner,
		default_square_width, default_square_width,
		textures[1][biome_texture]);
	}
    }
}

int
get_fps (unsigned int* start_time)
{
  unsigned int current_fps = 0;
  unsigned int current_time = SDL_GetTicks ();
  unsigned int delta_time = current_time - *start_time;
  *start_time = current_time;
  if (delta_time != 0)
    current_fps = 1000 / delta_time;
  return current_fps;
}

SDL_Surface*
copy_surface (SDL_Surface* src)
{
  SDL_Surface *dest;
  dest = SDL_CreateRGBSurfaceFrom (src->pixels, src->w, src->h,
			       src->format->BitsPerPixel, src->pitch,
			       src->format->Rmask, src->format->Gmask,
			       src->format->Bmask, src->format->Amask);

  return dest;
}
