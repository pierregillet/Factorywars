/**
 * @file
 * @author Corentin Bocquillon <0x539@nybble.fr>
 * @author Pierre Gillet <pierre.gillet+factorywars@linuxw.info>
 * @author Loup Fourment
 *
 * @section LICENSE
 *
 * Copyright (C) 2016 Corentin Bocquillon <corentin@nybble.fr>
 *
 * Copyright (C) 2016 Loup Fourment
 *
 * Copyright (C) 2016 Pierre Gillet <pierre.gillet+factorywars@linuxw.info>
 *
 * factorywars is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * factorywars is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with factorywars.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 *
 * main.cpp contain the code of the main function.
 */

#include <vector>
#include <getopt.h>
#include <config.h>
#include <locale.h>

#include "sdl/gui_utils.h"
#include "sdl/game.h"

#include "gettext.h"
#define _(string) gettext (string)

extern "C" {
  #include "network.h"
}

static const struct option longopts[] = {
  {"help", no_argument, NULL, 'h'},
  {"version", no_argument, NULL, 'v'},
  {"server", required_argument, NULL, 's'},
  {"server-port", required_argument, NULL, 'p'},
  {NULL, 0, NULL, 0}
};

static void print_help (void);
static void print_version (void);

const char *program_name = NULL;

int
main (int argc, char *argv[])
{
  setlocale (LC_ALL, "");

#if ENABLE_NLS
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
#endif
  
  int optc;
  int lose = 0;
  char *IP = NULL;
  unsigned short port = 0;

  program_name = argv[0];

  while ((optc = getopt_long (argc, argv, "hvs:p:", longopts, NULL)) != -1)
    {
      switch (optc)
	{
	case 'h':
	  print_help ();
	  exit (EXIT_SUCCESS);
	  break;
	case 's':
	  IP = optarg;
	  break;
	case 'p':
	  port = atoi (optarg);
	  break;
	case 'v':
	  print_version ();
	  exit(EXIT_SUCCESS);
	  break;
	default:
	  lose = 1;
	  break;
	}
    }

  if (lose || optind < argc)
    {
      fprintf (stderr, "Invalid arguments\n");
      print_help ();
      exit (EXIT_FAILURE);
    }

  if (run_gui () != 0)
    {
      return 1;
    }

  return 0;
}

static void
print_help (void)
{
  printf (_("Usage: %s [OPTION]..."), program_name);
  printf ("\n");
  printf (_("The best game in the world!"));
  printf ("\n\n");

  printf (_("-h, --help display this help and exit."));
  printf ("\n\n");

  printf (_("-s, --server <IP> specify the ip of the server you want to join."));
  printf ("\n");
  printf (_("-p, --server-port <port> specify the port of the server you want to join (default: 4284)."));
  printf ("\n\n");
}

static void
print_version (void)
{
  printf ("%s (%s) %s\n", PACKAGE, PACKAGE_NAME, VERSION);
  printf ("\n");

  printf ("\
Copyright (C) 2016 Corentin Bocquillon.\n\
Copyright (C) 2016 Loup Fourment.\n\
Copyright (C) 2016 Pierre Gillet.\n");

  printf (_("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>"));
  printf ("\n");
  printf (_("This is free software: you are free to change and destribute it."));
  printf ("\n");
  printf (_("There is NO WARRANTY, to the extent permitted by law."));
  printf ("\n");
}
