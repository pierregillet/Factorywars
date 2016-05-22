#pragma once

#include "save.h"
#include "structures.h"
#include <stdio.h>
#include "config.h"

/*
void display_items (SDL_Renderer** Renderer, std::string path, SDL_Texture** table, struct coordinates screen_origin);
*/
void display_items (SDL_Renderer** Renderer, std::string path, SDL_Texture** table_items, struct coordinates chunk_coords, struct coordinates screen_origin); 
