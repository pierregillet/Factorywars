DISTCLEANFILES = *~

EXTRA_DIST = \
	COMPILE.md\
	factorywars.conf\
	README.md\
	save_format.md\
	TODO.md\
	media

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
	src/map.pb-c.c src/map.pb-c.h src/map.proto\
	src/structures.h

AM_CFLAGS = -std=gnu99 $(libxml2_CFLAGS)
AM_CXXFLAGS = -std=gnu++11 $(libxml2_CFLAGS)
