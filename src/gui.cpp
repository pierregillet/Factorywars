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
 * gui.cpp contains the gui
 */

#include "gui.h"

SDL_Texture*
loadTexture(SDL_Renderer** main_renderer, std::string path)
{
  SDL_Texture* new_texture = NULL;
  SDL_Surface* loaded_surface = IMG_Load(path.c_str ());

  new_texture = SDL_CreateTextureFromSurface (*main_renderer, loaded_surface);
  SDL_FreeSurface (loaded_surface);

  return new_texture;
}

void
loadMedia (SDL_Renderer** main_renderer,
	   SDL_Texture** KeyPressTexture,
	   SDL_Texture** toolbar,
	   SDL_Texture*** textures)
{
  SDL_Texture* player_textures[4];
  SDL_Texture* items_textures[4];
  
  std::string textures_paths[] = {"media/textures/LEFT.png",
  				  "media/textures/RIGHT.png",
  				  "media/textures/LEFT.png",
  				  "media/textures/RIGHT.png",
  				  //ending of player_textures

  				  // Beginning items_textures
  				  "media/textures/arbre.png",
  				  "media/textures/pierre1.png",
  				  "media/textures/pierre2.png",
  				  "media/textures/pierre3.png"};
  
  for (int i = 0; i < 4; i++)
    {
      player_textures[i] = loadTexture (main_renderer,
  					textures_paths[i]);
    }
  textures[0] = player_textures;

  for (int i = 4; i < 7; i++)
    {
      items_textures[i - 4] = loadTexture (main_renderer,
  					   textures_paths[i]);
    }
    textures[1] = items_textures;


  // every box of the table is associated to an image
  KeyPressTexture[KEY_PRESS_SURFACE_DEFAULT] = loadTexture (main_renderer, "media/textures/LEFT.png");
  // if (KeyPressTexture[KEY_PRESS_SURFACE_DEFAULT] == NULL)
  //   success = false;
  
  KeyPressTexture[KEY_PRESS_SURFACE_UP] = loadTexture (main_renderer, "media/textures/LEFT.png");
  // if (KeyPressTexture[KEY_PRESS_SURFACE_UP ] == NULL)
  //   success = false;
    
  KeyPressTexture[KEY_PRESS_SURFACE_DOWN ] = loadTexture (main_renderer, "media/textures/RIGHT.png");
  // if (KeyPressTexture[KEY_PRESS_SURFACE_DOWN ] == NULL)
  //   success = false;
    
  KeyPressTexture[KEY_PRESS_SURFACE_LEFT ] = loadTexture (main_renderer, "media/textures/LEFT.png");
  // if (KeyPressTexture[KEY_PRESS_SURFACE_LEFT ] == NULL)
  //   success = false;  
    
  KeyPressTexture[KEY_PRESS_SURFACE_RIGHT ] = loadTexture (main_renderer, "media/textures/RIGHT.png");
  // if (KeyPressTexture[KEY_PRESS_SURFACE_DEFAULT] == NULL)
  //   success = false;

  *toolbar = loadTexture (main_renderer, "media/hud/toolbar.png");
  // if (*toolbar == NULL)
  //   success = false;
}

void
init (SDL_Window** Window,
      SDL_Renderer** Renderer,
      SDL_Texture** KeyPressTexture,
      SDL_Texture** biomes,
      SDL_Texture** items,
      SDL_Texture** toolbar,
      int* screen_height,
      int* screen_width,
      SDL_Texture*** textures)
{
  if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
      printf ("Error: %s\n", SDL_GetError ());
    }
	
  // if the SDL launched correctly
  *Window = SDL_CreateWindow ("Factorywars",
			      SDL_WINDOWPOS_UNDEFINED,
			      SDL_WINDOWPOS_UNDEFINED,
			      *screen_width,
			      *screen_height,
			      SDL_WINDOW_SHOWN
			      /*| SDL_WINDOW_RESIZABLE*/);
	  
  if (*Window == NULL) 
    {
      printf ("Couldn’t create window: %s\n", SDL_GetError());
      SDL_Quit();
    }
	  
  // if window has been created without errors
  *Renderer = SDL_CreateRenderer (*Window,
				  -1,
				  SDL_RENDERER_ACCELERATED
				  | SDL_RENDERER_PRESENTVSYNC);
  SDL_SetRenderDrawColor (*Renderer, 0xFF,0xFF,0xFF,0xFF);

  loadMedia (Renderer, KeyPressTexture, toolbar, textures);
  
  load_biomes (Renderer, biomes);
  load_items (Renderer, items);
}

int
handle_keydown (SDL_Keycode event_keycode,
		bool* keys_state,
		SDL_Texture*** textures,
		SDL_Texture** current_texture,
		SDL_Texture** key_press_texture)
{
  bool keydown = 1;
  switch (event_keycode)
    {
    case SDLK_UP:
      keys_state[0] = keydown;
      break;
    case SDLK_DOWN:
      keys_state[1] = keydown;
      break;
    case SDLK_LEFT:
      keys_state[2] = keydown;
      printf ("%d\n", textures[0][2]);
      *current_texture = textures[0][2]; // left texture // key_press_texture[KEY_PRESS_SURFACE_LEFT];
      break;
    case SDLK_RIGHT:
      keys_state[3] = keydown;
      *current_texture = key_press_texture[KEY_PRESS_SURFACE_RIGHT];
      break;
    default:
      break;
    }
  return 1;
}

int
handle_keyup (SDL_Keycode event_keycode,
	      bool* keys_state)
{
  bool keyup = 0;
  switch (event_keycode)
    {
    case SDLK_UP:
      keys_state[0] = keyup;
      break;
    case SDLK_DOWN:
      keys_state[1] = keyup;
      break;
    case SDLK_LEFT:
      keys_state[2] = keyup;
      break;
    case SDLK_RIGHT:
      keys_state[3] = keyup;
      break;
    default:
      break;
    }
  return 1;
}

int
handle_clickdown (int button,
		  coordinates click_coords,
		  bool* clicks_state,
		  int* screen_height,
		  int* screen_width,
		  struct coordinates* screen_origin,
		  struct map_coordinates* click_map_coords)
{
  bool clickdown = 1;
  *click_map_coords = get_map_coords (click_coords,
				      screen_height,
				      screen_width,
				      *screen_origin);
  switch (button)
    {
    case SDL_BUTTON_LEFT:
	clicks_state[0] = clickdown;
	break;
    case SDL_BUTTON_MIDDLE:
	clicks_state[1] = clickdown;
	break;
    case SDL_BUTTON_RIGHT:
      clicks_state[2] = clickdown;
      break;
    case SDL_BUTTON_X1:
      clicks_state[3] = clickdown;
      break;
    case SDL_BUTTON_X2:
      clicks_state[4] = clickdown;
      break;
    default:
      break;
    }
  return 1;
}

int
handle_clickup (int button,
		coordinates click_coords,
		bool* clicks_state,
		int* screen_height,
		int* screen_width,
		struct coordinates* screen_origin)
{
  // Not necessary right now because the clicks_state array elements are set back to 0 once events are handled
  /*
  bool clickup = 0;

  switch (button)
    {
    case SDL_BUTTON_LEFT:
      clicks_state[0] = clickup;
      break;
    case SDL_BUTTON_MIDDLE:
      clicks_state[1] = clickup;
      break;
    case SDL_BUTTON_RIGHT:
      clicks_state[2] = clickup;
      break;
    case SDL_BUTTON_X1:
      clicks_state[3] = clickup;
      break;
    case SDL_BUTTON_X2:
      clicks_state[4] = clickup;
      break;
    default:
      break;
    }
  */
  return 1;
}

int
handle_mousewheel (int wheel_x,
		   int* screen_height,
		   int* screen_width,
		   struct coordinates* screen_origin,
		   std::vector<Player>& players)
{
  players[0].changeSelectedTool (wheel_x);
  return 1;
}

int
handle_events (SDL_Texture*** textures,
	       SDL_Texture** CurrentTexture,
	       SDL_Texture** biomes,
	       bool* keys_state,
	       bool* clicks_state,
	       SDL_Texture** key_press_texture,
	       int* screen_height,
	       int* screen_width,
	       struct coordinates screen_origin,
	       struct map_coordinates* click_map_coords,
	       std::vector<Player>& players)
{
  SDL_Event event;
  coordinates click_coords;
  
  while (SDL_PollEvent (&event) != 0)
    {
      if (event.key.repeat != 0)
	continue;
      switch (event.type)
	{
	case SDL_QUIT:
	  return 0;
	  break;
	      
	case SDL_KEYDOWN:
	  handle_keydown (event.key.keysym.sym,
			  keys_state,
			  textures,
			  CurrentTexture,
			  key_press_texture);
	  break;

	case SDL_KEYUP:
	  handle_keyup (event.key.keysym.sym,
			keys_state);
	  break;

	case SDL_MOUSEBUTTONDOWN:
	  click_coords.x = event.button.x;
	  click_coords.y = event.button.y;
	  handle_clickdown (event.button.button,
			    click_coords,
			    clicks_state,
			    screen_height,
			    screen_width,
			    &screen_origin,
			    click_map_coords);
	  break;

	case SDL_MOUSEBUTTONUP:
	  click_coords.x = event.button.x;
	  click_coords.y = event.button.y;
	  handle_clickup (event.button.button,
			  click_coords,
			  clicks_state,
			  screen_height,
			  screen_width,
			  &screen_origin);
	  break;

	case SDL_MOUSEWHEEL:
	  handle_mousewheel (event.wheel.y,
			     screen_height,
			     screen_width,
			     &screen_origin,
			     players);
	  break;
	  
	default:
	  break;
	}
    }
  
  return 1;
}

int
move_coordinates_on_keydown (struct coordinates* screen_origin,
			     bool* keys_state,
			     struct coordinates* hero_coords,
			     struct coordinates screen_center)
{
  if (hero_coords -> x >= 680 && hero_coords -> y >= 400  )
  {
        screen_origin->y += (keys_state[0])? (-5) : 0;
        screen_origin->y += (keys_state[1])? 5 : 0;
        screen_origin->x += (keys_state[2])? (-5) : 0;
        screen_origin->x += (keys_state[3])? 5 : 0;
  }  
  if (screen_origin -> y <= 0 || screen_origin->x <= 0)
  {
        hero_coords->y += (keys_state[0])? (-5) : 0;
        hero_coords->y += (keys_state[1])? 5 : 0;
        hero_coords->x += (keys_state[2])? (-5) : 0;
        hero_coords->x += (keys_state[3])? 5 : 0;        
  }
  
  hero_coords->x = (hero_coords->x < 0 )? 0 : hero_coords->x;
  hero_coords->y = (hero_coords->y < 0 )? 0 : hero_coords->y;
 
  hero_coords -> x = (hero_coords -> x > 680)? 680 : hero_coords -> x;
  hero_coords -> y = (hero_coords -> y > 400)? 400 : hero_coords -> y;
 
  return 1;
}

void 
refresh_renderer (SDL_Renderer** Renderer)
{
  SDL_RenderClear (*Renderer);
}

int
blit (SDL_Renderer** Renderer,
      struct coordinates blit_origin,
      int width,
      int height,
      SDL_Texture* texture)
{
  SDL_Rect Rect = {.x = (int) blit_origin.x,
		   .y = (int) blit_origin.y,
		   .w = width, .h = height};
  // SDL_QueryTexture (texture, NULL, NULL, &Rect.w, &Rect.h);
  
  SDL_RenderSetViewport(*Renderer, &Rect);
  SDL_RenderCopy (*Renderer, texture, NULL,NULL);

  return 1;
}

void
display_blits(SDL_Renderer** Renderer)
{
  SDL_RenderPresent (*Renderer);
}

void
quit_sdl (SDL_Window** Window,
	  SDL_Renderer** Renderer,
	  SDL_Texture** CurrentTexture,
	  SDL_Texture** biomes,
	  SDL_Texture** items)
{
  for (int i = 0; i < 5; i++)
    {
      SDL_DestroyTexture (biomes[i]);
      SDL_DestroyTexture (items[i]);
    }
  
  SDL_DestroyTexture (*CurrentTexture);
  SDL_DestroyRenderer (*Renderer);
  SDL_DestroyWindow (*Window);

  SDL_Quit();
  printf ("Goodbye !\n");
}

int 
run_gui (int read_pipe,
	 int write_pipe,
	 std::vector<Player>& players)
{
  int screen_height = atoi (get_config_value ("height"));
  int screen_width = atoi (get_config_value ("width"));
  SDL_Window *Window = NULL;
  SDL_Renderer* Renderer = NULL;

  SDL_Texture** textures[2];
  SDL_Texture *biomes[5];
  SDL_Texture *items[5];
  SDL_Texture *key_press_texture [KEY_PRESS_SURFACE_TOTAL];
  SDL_Texture* toolbar = NULL;

  init (&Window, &Renderer, key_press_texture,
	biomes,	items, &toolbar, &screen_height,
	&screen_width, textures);

  struct coordinates screen_center; 
  screen_center.x = screen_width / 2;
  screen_center.y = screen_height / 2;
 
  struct coordinates screen_origin = {.x = 0,
				      .y = 0};

  struct coordinates hero_coords = {.x = screen_center.x,
				    .y = screen_center.y};
  
  SDL_Texture *current_texture = NULL;
  current_texture = key_press_texture [KEY_PRESS_SURFACE_DEFAULT];
  
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

  // We need to display the map at the beginning
  display_background (&Renderer, "save", biomes,
		      items, screen_origin);

  // Display character
  blit (&Renderer, screen_center, 25, 41, current_texture);
  
  // Display HUD
  struct coordinates toolbar_origin = {.x = screen_width / 4 ,
				       .y = (int) (screen_height - (screen_width / 2 * 0.11))}; 
  struct coordinates toolbar_size = {.x = screen_width / 2,
				     .y = (int) (screen_width / 2 * 0.11)};
  blit (&Renderer,
	toolbar_origin,
	toolbar_size.x,
	toolbar_size.y,
	toolbar);

  display_blits(&Renderer);

  while (handle_events (textures,
			&current_texture,
			biomes,
			keys_state,
			clicks_state,
			key_press_texture,
			&screen_height,
			&screen_width,
			screen_origin,
			&click_map_coords,
			players) != 0)
    {
      // Keyboard handling
      for (int i = 0; i < 4; i++)
	{
	  if (keys_state[i])
	    {
              move_coordinates_on_keydown (&screen_origin, keys_state, &hero_coords, screen_center);

	      send_move_command (write_pipe, screen_origin, screen_height, screen_width);

	      refresh_renderer (&Renderer);
	      display_background (&Renderer, "save", biomes, items, screen_origin);
	      blit (&Renderer, hero_coords, 25, 41, current_texture);
	      blit (&Renderer, toolbar_origin, toolbar_size.x,
		    toolbar_size.y, toolbar);

	      display_players (players, screen_origin, &Renderer, current_texture,
			       screen_height, screen_width);

	      display_blits(&Renderer);
	      break;
	    }
	}
      // End of keyboard handling

      // Left click handling
      if (clicks_state[0])
	{
	  if (get_surface_item (click_map_coords.chunk,
				click_map_coords.square,
				"save") == -1)
	    {
	      set_surface_item(click_map_coords.chunk,
			       click_map_coords.square,
			       1,
			       "save");
	      refresh_renderer (&Renderer);
	      display_background (&Renderer,
				  "save",
				  biomes,
				  items,
				  screen_origin);
	      
	      blit (&Renderer,
		    hero_coords,
		    25,
		    41,
		    current_texture);
	      blit (&Renderer,
		    toolbar_origin,
		    toolbar_size.x,
		    toolbar_size.y,
		    toolbar);

	      display_players (players, screen_origin, &Renderer, current_texture,
			       screen_height, screen_width);

	      display_blits(&Renderer);
	    }
	    
	  clicks_state[0] = 0;
	}
      // End of left click handling
      
      // Right click handling
      if (clicks_state[2])
	{
	  if (get_surface_item (click_map_coords.chunk,
				click_map_coords.square,
				"save") != -1)
	    {
	      set_surface_item(click_map_coords.chunk,
			       click_map_coords.square,
			       -1,
			       "save");
	      refresh_renderer (&Renderer);
	      display_background (&Renderer,
				  "save",
				  biomes,
				  items,
				  screen_origin);
	      blit (&Renderer,
		    hero_coords,
		    25,
		    41,
		    current_texture);
	      blit (&Renderer,
		    toolbar_origin,
		    toolbar_size.x,
		    toolbar_size.y,
		    toolbar);

	      display_players (players, screen_origin, &Renderer, current_texture,
			       screen_height, screen_width);
	      
	      display_blits (&Renderer);
	    }
	  clicks_state[2] = 0;
	}
      // End of right click handling

      // Network pipe handling
      if (handle_data_from_network_pipe (read_pipe, players, "save") > 0)
      	{
	  refresh_renderer (&Renderer);
	  display_background (&Renderer, "save", biomes, items, screen_origin);
	  blit (&Renderer, hero_coords, 25, 41, current_texture);
	  blit (&Renderer,
		toolbar_origin,
		toolbar_size.x,
		toolbar_size.y,
		toolbar);
	      
	  display_players (players, screen_origin, &Renderer, current_texture,
			   screen_height, screen_width);
	      
      	  display_blits (&Renderer);
      	}

      SDL_Delay (1/200);
    }

  quit_sdl (&Window, &Renderer, &current_texture, biomes, items);
  
  return 0;
}  

struct map_coordinates
get_map_coords (struct coordinates click_coords,
		int* screen_height,
		int* screen_width,
		struct coordinates screen_origin)
{
  struct map_coordinates click_map_coords;

  float x_float = screen_origin.x;
  float y_float = screen_origin.y;

  click_map_coords.chunk.x = (int) (x_float + (float) click_coords.x) / 24.0 / 16.0;
  click_map_coords.chunk.y = (int) (y_float + (float) click_coords.y) / 24.0 / 16.0;
  click_map_coords.square.x = (int) ((x_float + (float) click_coords.x) / 24.0) - ((float) click_map_coords.chunk.x * 16.0);
  click_map_coords.square.y = (int) ((y_float + (float) click_coords.y) / 24.0) - ((float) click_map_coords.chunk.y * 16.0);

  return click_map_coords;
}

void
display_players (std::vector<Player>& players, struct coordinates screen_origin,
		 SDL_Renderer** renderer, SDL_Texture* player_texture,
		 int screen_height, int screen_width)
{
  // Display players
  for (Player player : players)
    {
      struct coordinates player_coordinates = player.getCoordinates ();
      if (player_coordinates.x >= screen_origin.x
	  && player_coordinates.x <= screen_origin.x + screen_width)
	{
	  if (player_coordinates.y >= screen_origin.y
	      && player_coordinates.y <= screen_origin.y + screen_height)
	    {
	      player_coordinates.x -= screen_origin.x;
	      player_coordinates.y -= screen_origin.y;

	      blit (renderer, player_coordinates, 25, 41, player_texture);
	    }
	}
    }
}
