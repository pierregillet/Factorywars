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
 * menu.h is the header of menu.c.
 */

#pragma once

#include <SDL2/SDL_ttf.h>
#include <config.h>

#include "gui.h"

SDL_Texture* create_texture_from_text (const char* text, int font_size,
				       SDL_Color color,
				       SDL_Renderer* main_renderer);

int handle_main_menu_events (SDL_Renderer* main_renderer,
			     struct size screen_dimensions,
			     SDL_Rect* buttons, int number);

int display_main_menu (SDL_Renderer* main_renderer,
		       struct size screen_dimensions, char* dst,
		       size_t dst_len);

int get_save_path (SDL_Renderer* main_renderer, char* dst, size_t dst_len,
		   struct size screen_dimensions);

int find_button (struct coordinates click_coords, SDL_Rect* buttons,
		 int number);

int handle_load_save_menu_events (SDL_Rect* buttons, int number_of_buttons,
				  int* highlighted_line,
				  int* first_displayed_save,
				  int number_of_rows, int number_of_save);
