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
 * menu.c display the menus.
 */

#include "menu.h"

SDL_Texture*
create_texture_from_text (char* text, int font_size, SDL_Renderer** main_renderer)
{
  TTF_Font *font = TTF_OpenFont ("media/fonts/FreeSans.ttf", font_size);
  if (font == NULL)
    {
      fprintf (stderr, "Error while loading FreeSans.ttf\n");
      return NULL;
    }

  SDL_Color white_color = {255, 255, 255};
  SDL_Surface* text_surface = TTF_RenderText_Blended (font, text,
						      white_color);

  if (text_surface == NULL)
    {
      fprintf (stderr, "Error while rendering some text\n");
      return NULL;
    }

  SDL_Texture *text_texture = SDL_CreateTextureFromSurface (*main_renderer,
							    text_surface);
  
  TTF_CloseFont (font);
  SDL_FreeSurface (text_surface);

  return text_texture;
}

void
display_main_menu (SDL_Renderer** main_renderer, struct size screen_dimensions)
{
  // On remplit le fond de noir
  SDL_Texture* background;
  background = SDL_CreateTexture (*main_renderer, SDL_PIXELFORMAT_RGBA8888,
				  SDL_TEXTUREACCESS_TARGET,
				  screen_dimensions.x, screen_dimensions.y);
  SDL_SetTextureColorMod (background, 0, 0, 0);
  blit (main_renderer, {0,0}, screen_dimensions.x, screen_dimensions.y, background);


  // On charge les images du menu
  SDL_Surface *menu_bg_surface = IMG_Load ("media/menus/main_menu.png");
  SDL_Surface *button_bg_surface = IMG_Load ("media/menus/button1.png");

  SDL_Texture *menu_bg;
  menu_bg = SDL_CreateTextureFromSurface (*main_renderer, menu_bg_surface);
  SDL_FreeSurface (menu_bg_surface);

  SDL_Texture *button_bg;
  button_bg = SDL_CreateTextureFromSurface (*main_renderer, button_bg_surface);
  SDL_FreeSurface (button_bg_surface);
  
  if (menu_bg == NULL)
    {
      fprintf (stderr, "Error while loading the menu background\n");
      return;
    }

  if (button_bg == NULL)
    {
      fprintf (stderr, "Error while loading the buttons’ background\n");
      return;
    }


  // On crée les textures des textes
  const int number_of_buttons = 5;
  SDL_Texture *texts[number_of_buttons];

  char* menu_text[] = {"New game",
		       "Load game",
		       "Settings",
		       "About",
		       "Quit"};

  int font_size = 40;
  for (int i = 0; i < number_of_buttons; i++)
    {
      texts[i] = create_texture_from_text (menu_text[i], font_size, main_renderer);
    }

  // On affiche le fond du menu
  struct size blit_coords;
  SDL_Rect rect;
  blit (main_renderer,
	{screen_dimensions.x / 2 - 480 / 2, screen_dimensions.y / 2 - 640 / 2},
	480, 640, menu_bg);

  for (int i = 0; i < number_of_buttons; i++)
    {
      // Alignement
      blit_coords.x = 48 + screen_dimensions.x / 2 - 480 / 2;
      blit_coords.y = 64 + i * 112 + screen_dimensions.y / 2 - 640 / 2;

      blit (main_renderer, blit_coords, 384, 64, button_bg);

      // On récupère la largeur et la hauteur du texte
      SDL_QueryTexture (texts[i], NULL, NULL, &rect.w, &rect.h);

      // Alignement
      blit_coords.x += 384 / 2 - rect.w / 2;
      blit_coords.y += 64 / 2 - rect.h / 2;

      blit (main_renderer, blit_coords, rect.w, rect.h, texts[i]);
    }

  SDL_RenderPresent (*main_renderer);
  
  // On libère les textures
  SDL_DestroyTexture (button_bg);
  SDL_DestroyTexture (menu_bg);

  for (int i = 0; i < number_of_buttons; i++)
    {
      SDL_DestroyTexture (texts[i]);
    }
}
