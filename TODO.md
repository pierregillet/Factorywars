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
+ Translate the user’s side of the game in esperanto.
+ Continue to follow [XDG Base Directory Specification](https://standards.freedesktop.org/basedir-spec/basedir-spec-latest.html).
+ Create a website.
+ Use Texlive for documentation.
+ Update the list of dependencies in COMPILE.md.

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
+ Allow to zoom on the map/items in-game.
+ FPS limit.
+ Display using a system of ticks.
+ Handle the case in which if there is no change in the current view, we don't draw everything again (if it is lighter than redrawing everything).
+ Handle windows resizing correctly.

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
+ Switch from XML to JSON for the configuration file.
+ Create functions to modify the configuration file.

Save
----
+ Auto saving.
