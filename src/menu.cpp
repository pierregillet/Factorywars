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
create_texture_from_text (const char* text, int font_size, SDL_Color color,
			  SDL_Renderer* renderer)
{
  TTF_Font *font = TTF_OpenFont ("media/fonts/FreeSans.ttf", font_size);
  if (font == NULL)
    {
      fprintf (stderr, "Error while loading FreeSans.ttf\n");
      return NULL;
    }

  // SDL_Surface* text_surface = TTF_RenderUNICODE_Blended (font, text, color);
  SDL_Surface* text_surface = TTF_RenderUTF8_Blended (font, text, color);

  if (text_surface == NULL)
    {
      fprintf (stderr, "Error while rendering some text\n");
      return NULL;
    }

  SDL_Texture *text_texture = SDL_CreateTextureFromSurface (renderer,
							    text_surface);
  
  TTF_CloseFont (font);
  SDL_FreeSurface (text_surface);

  return text_texture;
}

int
display_main_menu (SDL_Renderer* main_renderer, struct size screen_dimensions,
		   char* dst, size_t dst_len)
{
  // On charge les images du menu
  SDL_Surface *menu_bg_surface = IMG_Load ("media/menus/main_menu.png");
  SDL_Surface *button_bg_surface = IMG_Load ("media/menus/button1.png");

  SDL_Texture *menu_bg;
  menu_bg = SDL_CreateTextureFromSurface (main_renderer, menu_bg_surface);
  SDL_FreeSurface (menu_bg_surface);

  SDL_Texture *button_bg;
  button_bg = SDL_CreateTextureFromSurface (main_renderer, button_bg_surface);
  SDL_FreeSurface (button_bg_surface);
  
  if (menu_bg == NULL)
    {
      fprintf (stderr, "Error while loading the menu background\n");
      return -1;
    }

  if (button_bg == NULL)
    {
      fprintf (stderr, "Error while loading the buttons’ background\n");
      return -1;
    }


  // On crée les textures des textes
  const int number_of_buttons = 5;
  SDL_Texture *texts[number_of_buttons];

  char* menu_text[] = {_("New game"),
		       _("Load game"),
		       _("Settings"),
		       _("About"),
		       _("Quit")};

  int font_size = 40;
  for (int i = 0; i < number_of_buttons; i++)
    {
      texts[i] = create_texture_from_text (menu_text[i], font_size,
					   {255, 255, 255}, main_renderer);
    }

  int stay = 1;
  int ret;

  // The background rectangle
  SDL_Rect bg_rect = {0, 0, screen_dimensions.x, screen_dimensions.y};

  while (stay)
    {
      // On remplit le fond de noir
      blit_rect (main_renderer, {0, 0, 0, 255}, bg_rect);

      // On affiche le fond du menu
      struct size blit_coords;
      SDL_Rect rect;
      blit (main_renderer,
	    {screen_dimensions.x / 2 - 480 / 2, screen_dimensions.y / 2 - 640 / 2},
	    480, 640, menu_bg);

      // On affiche les boutons et le texte par dessus
      SDL_Rect buttons[number_of_buttons];
      for (int i = 0; i < number_of_buttons; i++)
	{
	  // Alignement
	  blit_coords.x = 48 + screen_dimensions.x / 2 - 480 / 2;
	  blit_coords.y = 64 + i * 112 + screen_dimensions.y / 2 - 640 / 2;

	  buttons[i] = {.x = blit_coords.x,
			.y = blit_coords.y,
			.w = 384,
			.h = 64};

	  blit (main_renderer, blit_coords, 384, 64, button_bg);

	  // On récupère la largeur et la hauteur du texte
	  SDL_QueryTexture (texts[i], NULL, NULL, &rect.w, &rect.h);

	  // Alignement
	  blit_coords.x += 384 / 2 - rect.w / 2;
	  blit_coords.y += 64 / 2 - rect.h / 2;

	  blit (main_renderer, blit_coords, rect.w, rect.h, texts[i]);
	}

      SDL_RenderPresent (main_renderer);

      ret = handle_main_menu_events (main_renderer, screen_dimensions,
				     buttons, number_of_buttons);

      if (ret == 2)
	{
	  ret = get_save_path (main_renderer, dst, dst_len, screen_dimensions);
	  if (ret == 1)
	    {
	      stay = 0;
	      continue;
	    }
	}
      else if (ret == 4)
	{
	  ret = about (main_renderer, screen_dimensions);
	  if (ret != 0)
	    continue;
	}

      if (ret <= 1)
	stay = 0;
    }
  
  // On libère les textures
  SDL_DestroyTexture (button_bg);
  SDL_DestroyTexture (menu_bg);

  for (int i = 0; i < number_of_buttons; i++)
    {
      SDL_DestroyTexture (texts[i]);
    }

  return ret;
}

int
handle_main_menu_events (SDL_Renderer* main_renderer,
			 struct size screen_dimensions, SDL_Rect* buttons,
			 int number)
{
  SDL_Event event;
  struct coordinates click_coords;
  int stay = 1;
  int button;

  while (stay)
    {
      if (SDL_PollEvent (&event) == 0)
	continue;

      switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
	  if (event.button.button == SDL_BUTTON_LEFT)
	    {
	      click_coords.x = event.button.x;
	      click_coords.y = event.button.y;

	      button = find_button (click_coords, buttons, number);
	      // if (button != 0)
	      // 	stay = 0;
	      if (button == 1)
	      	stay = 0;
	      
	      else if (button == 2 || button == 4)
	      	  return button;

	      else if (button == 5)
		return 0;
	    }
	  break;

	case SDL_QUIT:
	  return 0;
	  break;

	default:
	  break;
	}
    }
  
  // Afficher les autres menus

  return 1;
}

int
find_button (struct coordinates click_coords, SDL_Rect* buttons, int number)
{
  int score = 0;

  for (int i = 0; i < number; i++)
    {
      if (click_coords.x >= buttons[i].x)
	score++;

      if (click_coords.x <= buttons[i].x + buttons[i].w)
	score++;

      if (click_coords.y >= buttons[i].y)
	score++;
      
      if (click_coords.y <= buttons[i].y + buttons[i].h)
	score++;

      if (score < 4)
	score = 0;
      else
	return i + 1;
    }

  return 0;
}

int
get_save_path (SDL_Renderer* main_renderer, char* dst, size_t dst_len,
	       struct size screen_dimensions)
{
  SDL_Texture *text;

  const int row_height = 20;

  const char *xdg_data_home = getenv ("XDG_DATA_HOME");
  const char *home = getenv ("HOME");

  if (RUN_IN_PLACE)
    strncpy (dst, "saves/", dst_len);

  else if (xdg_data_home == NULL)
    {
      if (home == NULL)
	strncpy (dst, "saves/", dst_len);
      else
	{
	  // On utilise la valeur par défaut de XDG_DATA_HOME
	  snprintf (dst, dst_len, "%s/.local/share/factorywars/saves/", home);
	}
    }

  else
    snprintf (dst, dst_len, "%s/factorywars/saves/", xdg_data_home);

  struct directory_list *dir_list = list_directory (dst, 1);
  if (dir_list == NULL)
    return 4;

  struct directory_list *head_dir_list = dir_list;

  // On enregistre le nombre de sauvegardes
  int number_of_save = number_of_files (dir_list);


  // On charge le fond du bouton
  SDL_Surface *button_bg_surface = IMG_Load ("media/menus/button1.png");
  SDL_Texture *button_bg;
  button_bg = SDL_CreateTextureFromSurface (main_renderer, button_bg_surface);
  SDL_FreeSurface (button_bg_surface);

  if (button_bg == NULL)
    fprintf (stderr, "Error while loading button1.png.");

  // On crée la texture du texte du bouton
  SDL_Texture *button_text;
  button_text = create_texture_from_text (_("Main menu"), 30,
					  {255, 255, 255}, main_renderer);


  SDL_Rect fill_rect, button, button_text_rect;

  SDL_QueryTexture (button_bg, NULL, NULL, &button.w, &button.h);
  button = {.x = screen_dimensions.x / 2 - button.w / 2,
	    .y = screen_dimensions.y - button.h - 1,
	    .w = button.w, .h = button.h};

  SDL_QueryTexture (button_text, NULL, NULL, &button_text_rect.w, &button_text_rect.h);
  button_text_rect.x = button.x + button.w / 2 - button_text_rect.w / 2;
  button_text_rect.y = button.y + button.h / 2 - button_text_rect.h / 2;

  
  int rows = (screen_dimensions.y - 66) / row_height;
  struct size blit_origin = {.x = 10, .y = 0};

  int highlighted_line = 0;
  int event_type = 0;
  int choice = 0;
  int first_displayed_save = 0;

  do
    {
      // On affiche un fond noir
      fill_rect = {0, 0, screen_dimensions.x, screen_dimensions.y};
      blit_rect (main_renderer, {0, 0, 0, 255}, fill_rect);


      // On affiche le bouton pour retourner au menu principal
      blit_origin = {.x = button.x, .y = button.y};
      blit (main_renderer, blit_origin, button.w, button.h, button_bg);

      blit_origin = {.x = button_text_rect.x, .y = button_text_rect.y};
      blit (main_renderer, blit_origin, button_text_rect.w, button_text_rect.h, button_text);


      // On passe i éléments si on veut afficher plus de sauvegardes
      // qu’il n’y a de lignes.
      for (int i = 0; i < first_displayed_save; i++)
	dir_list = dir_list->next;

      for (int i = 0; i < rows; i++)
	{
	  if (i == highlighted_line)
	    {
	      fill_rect = {0, i * row_height, screen_dimensions.x, row_height};
	      blit_rect (main_renderer, {255, 0, 0, 255}, fill_rect);
	    }


	  // On affiche le nom de la sauvegarde
	  text = create_texture_from_text (dir_list->dir_name, 16, {255, 255, 255},
					   main_renderer);
	  SDL_QueryTexture (text, NULL, NULL, &fill_rect.w, &fill_rect.h);

	  blit_origin.x = 10;
	  blit_origin.y = i * row_height;
	  blit (main_renderer, blit_origin, fill_rect.w, fill_rect.h, text);
	  SDL_DestroyTexture (text);


	  // On affiche la date de dernière modification
	  text = create_texture_from_text (dir_list->last_modification, 16,
					   {255, 255, 255}, main_renderer);
	  SDL_QueryTexture (text, NULL, NULL, &fill_rect.w, &fill_rect.h);

	  blit_origin.x = screen_dimensions.x - fill_rect.w - 10;
	  blit_origin.y = i * row_height;
	  blit (main_renderer, blit_origin, fill_rect.w, fill_rect.h, text);
	  SDL_DestroyTexture (text);

	  dir_list = dir_list->next;

	  // S’il n’y a plus rien à afficher
	  if (dir_list == NULL)
	    break;
	}

      dir_list = head_dir_list;

      SDL_RenderPresent (main_renderer);

      event_type = handle_load_save_menu_events (&button, 1, &highlighted_line,
						 &first_displayed_save, rows,
						 number_of_save);
    } while (event_type > 1 && event_type != 4);

  
  SDL_DestroyTexture (button_bg);
  SDL_DestroyTexture (button_text);

  if (event_type == 0 || event_type == 4)
    return event_type;

  
  // La touche entrée a été pressée
  // On parcourt la liste des sauvegardes jusqu’à la sauvegarde choisie
  choice = highlighted_line + first_displayed_save;
  for (int i = 0; i < choice; i++)
    dir_list = dir_list->next;

  // On l’ajoute à la chaine
  strncat (dst, dir_list->dir_name, dst_len);

  free_dir_list (dir_list);  

  return 1;
}

int
handle_load_save_menu_events (SDL_Rect* buttons, int number_of_buttons,
			      int* highlighted_line,
			      int* first_displayed_save,
			      int number_of_rows, int number_of_save)
{
  SDL_Event event;
  int stay = 1;
  int event_type = 2;

  struct coordinates click_coords;

  while (stay)
    {
      if (SDL_PollEvent (&event) == 0)
	continue;

      switch (event.type)
	{
	case SDL_QUIT:
	  stay = 0;
	  event_type = 0;
	  break;

	case SDL_MOUSEBUTTONDOWN:
	  if (event.button.button == SDL_BUTTON_LEFT)
	    {
	      click_coords.x = event.button.x;
	      click_coords.y = event.button.y;

	      if (find_button (click_coords, buttons, number_of_buttons) == 1)
		return 4;
	    }

	  break;

	case SDL_KEYDOWN:
	  switch (event.key.keysym.sym)
	    {
	    case SDLK_UP:
	      if (*highlighted_line > 0)
		{
		  if (*first_displayed_save > 0)
		    (*first_displayed_save)--;

		  else
		    (*highlighted_line)--;

		  stay = 0;
		  event_type = 2;
		}
	      break;

	    case SDLK_DOWN:
	      if (*highlighted_line < number_of_rows - 1
		  && *highlighted_line < number_of_save - 1)
		(*highlighted_line)++;
	      else if (*first_displayed_save + *highlighted_line < number_of_save - 1)
		(*first_displayed_save)++;

	      stay = 0;
	      event_type = 3;

	      break;

	    case SDLK_RETURN:
	      stay = 0;
	      event_type = 1;
	      break;
	    }
	  break;

	default:
	  break;
	}
    }

  return event_type;
}

int
display_in_game_menu (SDL_Renderer* main_renderer,
		      struct size screen_dimensions)
{
  // On charge les images du menu
  SDL_Surface *menu_bg_surface = IMG_Load ("media/menus/main_menu.png");
  SDL_Surface *button_bg_surface = IMG_Load ("media/menus/button1.png");

  SDL_Texture *menu_bg;
  menu_bg = SDL_CreateTextureFromSurface (main_renderer, menu_bg_surface);
  SDL_FreeSurface (menu_bg_surface);

  SDL_Texture *button_bg;
  button_bg = SDL_CreateTextureFromSurface (main_renderer, button_bg_surface);
  SDL_FreeSurface (button_bg_surface);
  
  if (menu_bg == NULL)
    {
      fprintf (stderr, "Error while loading the menu background\n");
      return -1;
    }

  if (button_bg == NULL)
    {
      fprintf (stderr, "Error while loading the buttons’ background\n");
      return -1;
    }


  // On crée les textures des textes
  const int number_of_buttons = 5;
  SDL_Texture *texts[number_of_buttons];

  char* menu_text[] = {_("Save"),
		       _("Save and quit"),
		       _("Settings"),
		       _("Main Menu"),
		       _("Quit")};

  int font_size = 40;
  for (int i = 0; i < number_of_buttons; i++)
    {
      texts[i] = create_texture_from_text (menu_text[i], font_size,
					   {255, 255, 255}, main_renderer);
    }

  int ret;

  // The background rectangle
  SDL_Rect bg_rect = {0, 0, screen_dimensions.x, screen_dimensions.y};

  // On affiche le fond du menu
  struct size blit_coords;
  SDL_Rect rect;
  blit (main_renderer,
	{screen_dimensions.x / 2 - 480 / 2, screen_dimensions.y / 2 - 640 / 2},
	480, 640, menu_bg);

  // On affiche les boutons et le texte par dessus
  SDL_Rect buttons[number_of_buttons];
  for (int i = 0; i < number_of_buttons; i++)
    {
      // Alignement
      blit_coords.x = 48 + screen_dimensions.x / 2 - 480 / 2;
      blit_coords.y = 64 + i * 112 + screen_dimensions.y / 2 - 640 / 2;

      buttons[i] = {.x = blit_coords.x,
		    .y = blit_coords.y,
		    .w = 384,
		    .h = 64};

      blit (main_renderer, blit_coords, 384, 64, button_bg);

      // On récupère la largeur et la hauteur du texte
      SDL_QueryTexture (texts[i], NULL, NULL, &rect.w, &rect.h);

      // Alignement
      blit_coords.x += 384 / 2 - rect.w / 2;
      blit_coords.y += 64 / 2 - rect.h / 2;

      blit (main_renderer, blit_coords, rect.w, rect.h, texts[i]);
    }

  SDL_RenderPresent (main_renderer);

  ret = handle_in_game_menu_events (buttons, number_of_buttons);
  
  // On libère les textures
  SDL_DestroyTexture (button_bg);
  SDL_DestroyTexture (menu_bg);

  for (int i = 0; i < number_of_buttons; i++)
    {
      SDL_DestroyTexture (texts[i]);
    }

  return ret;
}

int
handle_in_game_menu_events (SDL_Rect* buttons, int number_of_buttons)
{
  SDL_Event event;
  struct coordinates click_coords;
  int stay = 1;
  int button;

  while (stay)
    {
      if (SDL_PollEvent (&event) == 0)
	continue;

      switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
	  if (event.button.button == SDL_BUTTON_LEFT)
	    {
	      click_coords.x = event.button.x;
	      click_coords.y = event.button.y;

	      button = find_button (click_coords, buttons, number_of_buttons);

	      if (button == 5)
		return 0;
	      else if (button != 0 && button != 3)
		stay = 0;
	    }
	  break;

	case SDL_KEYDOWN:
	  if (event.key.keysym.sym == SDLK_ESCAPE)
	    return 6;
	  break;

	case SDL_QUIT:
	  return 0;
	  break;

	default:
	  break;
	}
    }
  
  // Afficher les autres menus

  return button;
}

int
about (SDL_Renderer* main_renderer, struct size screen_dimensions)
{
  SDL_Texture *text;

  // On charge le fond du bouton
  SDL_Surface *button_bg_surface = IMG_Load ("media/menus/button1.png");
  SDL_Texture *button_bg;
  button_bg = SDL_CreateTextureFromSurface (main_renderer, button_bg_surface);
  SDL_FreeSurface (button_bg_surface);

  if (button_bg == NULL)
    fprintf (stderr, "Error while loading button1.png.");

  // On crée la texture du texte du bouton
  SDL_Texture *button_text;
  button_text = create_texture_from_text (_("Main menu"), 30,
					  {255, 255, 255}, main_renderer);


  SDL_Rect fill_rect, button, button_text_rect;

  SDL_QueryTexture (button_bg, NULL, NULL, &button.w, &button.h);
  button = {.x = screen_dimensions.x / 2 - button.w / 2,
	    .y = screen_dimensions.y - button.h - 1,
	    .w = button.w, .h = button.h};

  SDL_QueryTexture (button_text, NULL, NULL, &button_text_rect.w, &button_text_rect.h);
  button_text_rect.x = button.x + button.w / 2 - button_text_rect.w / 2;
  button_text_rect.y = button.y + button.h / 2 - button_text_rect.h / 2;

  
  struct size blit_origin = {.x = 10, .y = 0};

  // On affiche un fond noir
  fill_rect = {0, 0, screen_dimensions.x, screen_dimensions.y};
  blit_rect (main_renderer, {0, 0, 0, 255}, fill_rect);


  // On affiche le bouton pour retourner au menu principal
  blit_origin = {.x = button.x, .y = button.y};
  blit (main_renderer, blit_origin, button.w, button.h, button_bg);

  blit_origin = {.x = button_text_rect.x, .y = button_text_rect.y};
  blit (main_renderer, blit_origin, button_text_rect.w, button_text_rect.h, button_text);

  SDL_RenderPresent (main_renderer);

  SDL_DestroyTexture (button_bg);
  SDL_DestroyTexture (button_text);

  if (handle_about_menu_events (&button, 1) == 0)
    return 0;
  else
    return 1;

}

int
handle_about_menu_events (SDL_Rect* buttons, int number_of_buttons)
{
  SDL_Event event;
  struct coordinates click_coords;
  int stay = 1;
  int button;

  while (stay)
    {
      if (SDL_PollEvent (&event) == 0)
	continue;

      switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
	  if (event.button.button == SDL_BUTTON_LEFT)
	    {
	      click_coords.x = event.button.x;
	      click_coords.y = event.button.y;

	      button = find_button (click_coords, buttons, number_of_buttons);

	      if (button != 0)
		return button;

	    }
	  break;

	case SDL_QUIT:
	  return 0;
	  break;

	default:
	  break;
	}
    }
}
