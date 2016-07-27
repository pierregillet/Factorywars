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
#include <wchar.h>

#include "gui.h"
#include "gettext.h"

#define _(string) gettext (string)

/**
 * Create a texture from a text.
 *
 * @param text is the text to render.
 * @param font_size is the font size.
 * @param color will be the color of the text.
 * @param renderer is the renderer
 * @return a texture.
 */
SDL_Texture* create_texture_from_text (const char* text, int font_size,
				       SDL_Color color,
				       SDL_Renderer* renderer);

/**
 * Handle the main menu events.
 *
 * @param main_renderer is the renderer for the window.
 * @param screen_dimensions is the dimensions of the screen.
 * @param buttons is an array containing the hitboxes of the buttons.
 * @param number is the number of buttons.
 * @return + 0 if quit was clicked.
 * + 1 if new game was clicked.
 * + 2 if load game was clicked.
 * + 4 if about was clicked.
 */
int handle_main_menu_events (SDL_Renderer* main_renderer,
			     struct size screen_dimensions,
			     SDL_Rect* buttons, int number);

/**
 * Display the main menu.
 *
 * @param main_renderer is the renderer of the window.
 * @param screen_dimensions is the dimensions of the screen.
 * @param dst is a string which will contain the save_path.
 * @param dst_len is the lenght of the dst string.
 * @return + 0 if we need to quit.
 * + 1 if we need to launch a new game.
 * + 2 if we need to load a saved game.
 */
int display_main_menu (SDL_Renderer* main_renderer,
		       struct size screen_dimensions, char* dst,
		       size_t dst_len);

/**
 * Get the path to the save the user want to load.
 *
 * @param main_renderer is the window’s renderer.
 * @param dst is the string where we’ll store the path to the save file.
 * @param dst_len is the size of the dst array.
 * @param screen_dimensions is the dimensions of the screen.
 * @return + 0 if we need to quit
 * + 1 if a save has been chosen.
 * + 4 if we need to return to the main menu.
 */
int get_save_path (SDL_Renderer* main_renderer, char* dst, size_t dst_len,
		   struct size screen_dimensions);

/**
 * Find which button was clicked.
 *
 * @param click_coords is the coordinates of the click.
 * @param buttons is the hitboxes of the buttons.
 * @param number is the number of buttons.
 * @return + 0 if no buttons was clicked.
 * + the number of the buttons (begin at position 1).
 */
int find_button (struct coordinates click_coords, SDL_Rect* buttons,
		 int number);

/**
 * Handle the events of the load save menu.
 *
 * @param buttons is the buttons’s hitboxes.
 * @param number_of_buttons is the number of buttons.
 * @param highlighted_line is a pointer to the number of the line which is highlighted.
 * @param first_displayed_save is a pointer to the number of the first displayed save in the menu.
 * @return + 0 if we need to quit.
 * + 1 if the return key was pressed.
 * + 4 if we need to return to the main menu.
 * + 2 if the down arrow was pressed.
 * + 3 if the up arrow was pressed.
 */
int handle_load_save_menu_events (SDL_Rect* buttons, int number_of_buttons,
				  int* highlighted_line,
				  int* first_displayed_save,
				  int number_of_rows, int number_of_save);

/**
 * Display the in-game menu.
 *
 * @param main_renderer is the renderer of the window.
 * @param screen_dimensions is the dimensions of the screen.
 * @return + 0 if we need to quit.
 * + 1 if we need to save the game.
 * + 2 if we need to save and quit the game.
 * + 3 if we need to print the settings menu.
 * + 4 if we need to return to the main menu.
 * + 5 if we have nothing to do.
 */
int display_in_game_menu (SDL_Renderer* main_renderer,
			  struct size screen_dimensions);

/**
 * Handle the in-game menu events.
 *
 * @param buttons is the buttons’ hitboxes.
 * @param number_of_buttons is the number of buttons.
 * @return + 0 if the quit button was pressed.
 * + 1 if the save button was pressed.
 * + 2 if the save and quit button was pressed.
 * + 3 if the settings button was pressed.
 * + 4 if the main menu button was pressed.
 */
int handle_in_game_menu_events (SDL_Rect* buttons, int number_of_buttons);

/**
 * Print the about menu.
 *
 * @param main_renderer is the renderer of the window.
 * @param screen_dimensons is the dimensions of the screen.
 * @return 0 if we have to quit or 1 to display the main menu again.
 */
int about (SDL_Renderer* main_renderer, struct size screen_dimensions);

/**
 * Handle the about menu’s events.
 *
 * @param buttons is an array containing the buttons.
 * @param number_of_buttons is the number of buttons in the array buttons.
 * @return the clicked button or 0 if we have to quit.
 */
int handle_about_menu_events (SDL_Rect* buttons, int number_of_buttons);
