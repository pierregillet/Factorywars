DISTCLEANFILES = *~

EXTRA_DIST = config.rpath  \
	COMPILE.md\
	factorywars.conf\
	README.md\
	TODO.md\
	src/map.proto src/player-data.proto\
	saves

bin_PROGRAMS = factorywars
factorywars_SOURCES = \
	src/main.cpp\
	src/game_events.cpp src/game_events.h\
	src/gui.cpp src/gui.h\
	src/save.c src/save.h\
	src/utils.c src/utils.h\
	src/items.cpp src/items.h\
	src/XMLParser.c src/XMLParser.h\
	src/player.cpp src/player.h\
	src/config.c src/config.h\
	src/network.c src/network.h\
	src/display_map.cpp src/display_map.h\
	src/display_item.cpp src/display_item.h\
	src/multiplayer.cpp src/multiplayer.h\
	src/map.pb-c.c src/map.pb-c.h\
	src/player-data.pb-c.c src/player-data.pb-c.h\
	src/menu.cpp src/menu.h\
	src/game.cpp src/game.h\
	src/gettext.h\
	src/structures.h

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
