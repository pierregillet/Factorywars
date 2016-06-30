AC_INIT([factorywars], [0.1], [0x539+bug@nybble.fr])
AM_INIT_AUTOMAKE([foreign -Wall -Werror subdir-objects])

AC_PROG_CC
AC_PROG_CXX

PKG_CHECK_MODULES([libxml2], [libxml-2.0 >= 2])
PKG_CHECK_MODULES([libSDL2], [sdl2 >= 2])
PKG_CHECK_MODULES([libSDL2Image], [SDL2_image])
PKG_CHECK_MODULES([PROTOBUFC], [libprotobuf-c >= 1.0.0])
PKG_CHECK_MODULES([SDL2ttf], [sdl2 >= 2])
LIBS="$LIBS $libSDL2Image_LIBS $libxml2_LIBS $PROTOBUFC_LIBS $SDL2ttf_LIBS"

AC_ARG_ENABLE([run-in-place],
  AS_HELP_STRING([--enable-run-in-place],
    [Enable the run in place option to keep every file needed by the game in the directory where we compiled it]))

AS_IF([test "$enable_run_in_place" = "yes"], [
  AC_DEFINE([RUN_IN_PLACE], [1],
  [Define if we need to keep every file needed by the game in the current directory])], [
  AC_DEFINE([RUN_IN_PLACE], [0],
  [Define if we need to keep every file needed by the game in the current directory])])

AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
