DISTCLEANFILES = *~

EXTRA_DIST = config.rpath  \
	COMPILE.md\
	factorywars.conf\
	README.md\
	TODO.md\
	src/tile.proto src/player-data.proto\
	saves

bin_PROGRAMS = factorywars
factorywars_SOURCES = \
	src/main.cpp\
	src/config.c src/config.h\
	src/gettext.h\
	src/items.cpp src/items.h\
	src/map.cpp src/map.h\
	src/tile.pb-c.c src/tile.pb-c.h\
	src/multiplayer.cpp src/multiplayer.h\
	src/network.c src/network.h\
	src/player.cpp src/player.h\
	src/player-data.pb-c.c src/player-data.pb-c.h\
	src/structures.h\
	src/utils.c src/utils.h\
	src/XMLParser.c src/XMLParser.h\
	src/sdl/display_item.cpp src/sdl/display_item.h\
	src/sdl/display_map.cpp src/sdl/display_map.h\
	src/sdl/game.cpp src/sdl/game.h\
	src/sdl/game_events.cpp src/sdl/game_events.h\
	src/sdl/gui_exit.cpp src/sdl/gui_exit.h\
	src/sdl/gui_init.cpp src/sdl/gui_init.h\
	src/sdl/gui_utils.cpp src/sdl/gui_utils.h\
	src/sdl/menu.cpp src/sdl/menu.h

factorywars_LDADD = $(LIBINTL)

texturesdir = $(pkgdatadir)/textures
dist_textures_DATA = \
	media/fonts/FreeSans.ttf media/hud/toolbar.png\
	media/menus/button1.png media/menus/main_menu.png\
	media/textures/arbre.png media/textures/biome1.png\
	media/textures/biome2.png media/textures/LEFT.png\
	media/textures/RIGHT.png media/textures/pierre1.png\
	media/textures/pierre2.png media/textures/pierre3.png

fontsdir = $(pkgdatadir)/fonts
dist_fonts_DATA = media/fonts/FreeSans.ttf

AM_CFLAGS = -std=gnu99 $(libxml2_CFLAGS)
AM_CXXFLAGS = -std=gnu++11 $(libxml2_CFLAGS)
AM_CPPFLAGS = \
	-DLOCALEDIR=\"$(localedir)\"\
	-DFONTSDIR=\"$(fontsdir)\"\
	-DTEXTURESDIR=\"$(texturesdir)\"

SUBDIRS = po

ACLOCAL_AMFLAGS = -I m4
