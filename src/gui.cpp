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
load_texture (SDL_Renderer* main_renderer, std::string path)
{
  SDL_Texture* new_texture = NULL;
  SDL_Surface* loaded_surface = IMG_Load(path.c_str ());

  new_texture = SDL_CreateTextureFromSurface (main_renderer, loaded_surface);
  SDL_FreeSurface (loaded_surface);
  
  return new_texture;
}

void
init (SDL_Window** main_window,
      SDL_Renderer** main_renderer,
      const int screen_height,
      const int screen_width)
{
  if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
      fprintf (stderr, "Error: %s\n", SDL_GetError ());
    }

  if( TTF_Init() == -1 )
    {
      fprintf(stderr, "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
      SDL_Quit();
    }
	
  // if the SDL launched correctly
  *main_window = SDL_CreateWindow ("Factorywars",
				   SDL_WINDOWPOS_UNDEFINED,
				   SDL_WINDOWPOS_UNDEFINED,
				   screen_width,
				   screen_height,
				   SDL_WINDOW_SHOWN
				   | SDL_WINDOW_RESIZABLE);
	  
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
}

int
blit (SDL_Renderer* main_renderer,
      struct size blit_origin,
      int width,
      int height,
      SDL_Texture* texture)
{
  SDL_Rect rectangle = {.x = blit_origin.x,
			.y = blit_origin.y,
			.w = width,
			.h = height};
  
  SDL_RenderSetViewport (main_renderer, &rectangle);
  SDL_RenderCopy (main_renderer, texture, NULL, NULL);

  return 1;
}

void
free_textures (SDL_Texture* textures[][10],
	       SDL_Texture** current_texture)
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
}

void
quit_sdl (SDL_Window** main_window,
	  SDL_Renderer** main_renderer)
{  
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

  SDL_Window *Window = NULL;
  SDL_Renderer* Renderer = NULL;

  init (&Window, &Renderer, screen_height,
	screen_width);
  
  SDL_Texture* textures[4][10];
  load_game_textures (Renderer, textures);

  SDL_Texture *player_texture = NULL;
  player_texture = textures[0][1];

  const int save_path_len = 256;
  char save_path[save_path_len], map_path[save_path_len];
  int ret;

  int stay = 1;

  while (stay)
    {
      ret = display_main_menu (Renderer, screen_dimensions,
			       save_path, save_path_len);
      if (ret == 0)
	{
	  quit_sdl (&Window, &Renderer);
	  return 0;
	}

      // else if (ret == 2)
      //   snprintf (map_path, save_path_len, "%s/%s", save_path, "map");
      // else
      //   strncpy (map_path, "protosave", save_path_len);

      ret = run_game (Renderer, save_path, read_pipe,
		      write_pipe, screen_dimensions,
		      players, textures, &player_texture);

      if (ret == 0)
	stay = 0;
    }

  free_textures (textures, &player_texture);
  quit_sdl (&Window, &Renderer);
  
  return 0;
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
	  // biome_texture = map.;
	  
	  blit (main_renderer, square_top_left_corner,
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
  if (delta_time > 0)
    current_fps = 1000 / delta_time;
  else if (delta_time < 0)
    current_fps = (1000 / -delta_time);
  else
    current_fps = 0;
  return current_fps;
}

void
display_fps (SDL_Renderer* main_renderer,
	     unsigned int* start_time,
	     TTF_Font* ttf_freesans)
{
  SDL_Color white = {255, 255, 255};

  char fps[10];
  unsigned int tmp_fps = get_fps (start_time);

  snprintf (fps, 10, "%d fps", tmp_fps);

  SDL_Surface* surface_message = TTF_RenderText_Blended (ttf_freesans,
							 fps,
							 white);

  SDL_Texture* message = SDL_CreateTextureFromSurface (main_renderer, surface_message);

  struct size blit_origin = {.x = 0,
			     .y = 0};
  int blit_height = 0;
  int blit_width = 0;
  SDL_QueryTexture (message, NULL, NULL,
		    &blit_width, &blit_height);

  blit (main_renderer, blit_origin, blit_width,
	blit_height, message);

  SDL_Rect message_rect = {.x = 0,
			   .y = 0,
			   .w = 150,
			   .h = 150};

  SDL_RenderCopy (main_renderer, message, NULL, &message_rect);
}

SDL_Surface*
copy_surface (SDL_Surface* src)
{
  SDL_Surface *dest;
  dest = SDL_CreateRGBSurfaceFrom (src->pixels, src->w, src->h,
				   src->format->BitsPerPixel,
				   src->pitch,
				   src->format->Rmask,
				   src->format->Gmask,
				   src->format->Bmask,
				   src->format->Amask);

  return dest;
}

void
blit_rect (SDL_Renderer* renderer, SDL_Color color, SDL_Rect rectangle)
{
  SDL_RenderSetViewport (renderer, NULL);
  SDL_SetRenderDrawColor (renderer, color.r, color.g, color.b, color.a);
  SDL_RenderFillRect (renderer, &rectangle);
}
