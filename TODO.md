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
+ Create a website.
+ Use Texlive for documentation.
+ Update the list of dependencies in COMPILE.md.
+ Randomly generate maps (maybe perlin noise algorithm).

Main
----
+ Put the main loop in the main.cpp file. We need to discuss if it’s necessary.

Players
-------
+ Use the player’s inventory system.
+ Prevent players to move when there is an object where they want to move.

Network
-------
+ Create the tcp server which will send the map.
+ Send through the network when we delete/add an object on the map.
+ Run the handle_data_from_network_pipe function in the loop.

GUI
---
+ Replace screen_height and screen_width with a structure.
+ Blit by layers, first the ground textures then the objects.
+ Blit squares individually (not chunks).
+ Allow to zoom on the map/items in-game.
+ FPS limit.
+ Display using a system of ticks.
+ Handle the case in which if there is no change in the current view, we don't draw everything again (if it is lighter than redrawing everything).
+ Handle windows resizing.

Events
------


Textures
--------
+ Create the player textures.
+ Create ores’ textures.
+ Create the in-game objects textures.
+ Create menu’s textures.
+ Create the inventory’s textures.

Configuration
-------------
+ Switch from XML to JSON.
+ Create functions to modify the configuration file.

Save
----
+ Auto saving.
+ Create super chunks for saving, we need to discuss the size of those chunks but maybe 128*128 squares (64 times smaller than for now, 8*8 of current chunks).
