#pragma once

#include "save.h"
#include "structures.h"
#include <stdio.h>
#include "gui.h"

void display_items (SDL_Renderer** Renderer,
		    std::string path,
		    SDL_Texture** table_items,
		    struct coordinates screen_origin,
		    struct chunk_info chunk,
		    int l,
		    int m); 

void load_items (SDL_Renderer** Renderer, SDL_Texture** table);

