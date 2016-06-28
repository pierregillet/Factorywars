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
+ Create the player textures.
+ Replace screen_height and screen_width with a structure
+ Rewrite gui.cpp.
+ Delete display_blits function.
+ Blit by layers, first the ground textures then the objects.
+ FPS counter.
+ FPS limit.
