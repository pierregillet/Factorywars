AC_INIT([factorywars], [0.1], [0x539+bug@nybble.fr])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

AC_PROG_CC
AC_PROG_CXX

PKG_CHECK_MODULES([libxml2], [libxml-2.0 >= 2])
PKG_CHECK_MODULES([libSDL2], [sdl2 >= 2])
PKG_CHECK_MODULES([libSDL2Image], [SDL2_image])
LIBS="$LIBS $libSDL2Image_LIBS -lxml2"

AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
