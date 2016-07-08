TODO
====

General
-------
+ Fill the index page of the documentation generated with doxygen with the 
content of the README.md file.
+ Create a contribution guide.
+ Finish the documentation of every function.
+ Translate the documentation in french.
+ Translate the documentation in esperanto.
+ Translate the user’s side of the game in french and esperanto.
+ Follow [XDG Base Directory Specification](https://standards.freedesktop.org/basedir-spec/basedir-spec-latest.html).
+ Create a mailing list.
+ Create a website.
+ Auto saving.
+ Use Texlive for documentation.
+ Use GNU gettext for translation.

Main
----
+ Put the main loop in the main.cpp file.

Players
-------
+ Use the player’s inventory system.

Network
-------
+ Create the tcp server which will send the map.
+ Send through the network when we delete/add an object on the map.
+ Run the handle_data_from_network_pipe function in the loop.

GUI
---
+ Replace screen_height and screen_width with a structure
+ Rewrite most of gui.cpp (it is a mess in there !).
+ Blit by layers, first the ground textures then the objects.
+ Allow to zoom on the map/items in-game.
+ FPS limit.
+ Display menus.
+ Display using a system of ticks.
+ Handle the case in which if there is no change in the current view, we don't draw everything again (if it is lighter than redrawing everything).
+ In blit function, change the structure from size to coordinates.

Events
------
+ Replace the clicks_state and keyboard_state arrays with structures (to make it readable when used).

Textures
--------
+ Create the player textures.
+ Create ores’ textures.
+ Create the in-game objects textures.
+ Create menu’s textures.

Configuration
-------------
+ Switch from XML to JSON.
+ Create functions to modify the configuration file.
