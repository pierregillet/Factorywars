 /**
 * @file
 * @author Corentin Bocquillon <0x539@nybble.fr>
 *
 * @section LICENSE
 *
 * Copyright (C) 2016 Corentin Bocquillon
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
 * network.c is the networking part of the game.
 */

#include "network.h"

int
send (const char *IP, unsigned short port, char* data)
{
  int sockfd6, s;
  char port_str[6];
  struct addrinfo hints;
  struct addrinfo *result, *rp;

  /* Convert port to a string */
  snprintf (port_str, 6, "%d", port);

  memset (&hints, 0, sizeof (struct addrinfo));
  hints.ai_family = AF_UNSPEC; // Allow IPv4 and IPv6
  hints.ai_socktype = SOCK_DGRAM; // Datagram socket
  hints.ai_flags = 0;
  hints.ai_protocol = 0; //Any protocol

  s = getaddrinfo (IP, port_str, &hints, &result);
  if (s != 0)
    {
      fprintf (stderr, "Getaddrinfo: %s\n", gai_strerror (s));
      return -1;
    }

  for (rp = result; rp != NULL; rp = rp->ai_next)
    {
      sockfd6 = socket (rp->ai_family, rp->ai_socktype, rp->ai_protocol);
      
      if (sockfd6 == -1)
	continue;
      if (connect (sockfd6, rp->ai_addr, rp->ai_addrlen) != -1)
	break; // Success

      close (sockfd6);
  }

  if (rp == NULL)
    {
      fprintf (stderr, "Couldn't connect\n");
      return -1;
  }

  freeaddrinfo (result);

  if ((unsigned) send (sockfd6, data, strlen (data), 0) != strlen (data))
    {
      fprintf (stderr, "Error while writing on the socket\n");
      return -1;
    }

  return 1;
}

int
run_network_process (unsigned short port, int* pipes)
{
  if (pipe (pipes) && pipe (pipes + 2))
    {
      fprintf (stderr, "Pipe failed.\n");
      return 0;
    }

  pid_t pid;

  pid = fork ();
  if (pid == (pid_t) 0)
    {
      /* Child */
      close (pipes[1]);
      close (pipes[2]);
      handle_network_communication (port, pipes[0], pipes[3]);
      exit (EXIT_SUCCESS);
    }
  else if (pid < (pid_t) 0)
    {
      /* The fork failed */
      fprintf (stderr, "Fork failed.\n");
      return EXIT_FAILURE;
    }
  else
    {
      /* Parent */
      close (pipes[0]);
      close (pipes[3]);
    }
  return 1;
}

void
handle_network_communication (unsigned short port, int read_pipe,
			      int write_pipe)
{
  /* Les constantes */
  /* Constants */
  const int BUFFER_SIZE = 512;
  
  int quit = 0;
  char buffer[BUFFER_SIZE];

  /* On crée le tableau qui contiendra les IP */
  /* We create the array that will contains the IPs */
  unsigned int number_of_player = 1;
  char **IPs = (char**) malloc (number_of_player * sizeof (char*));
  for (unsigned int i = 0; i < number_of_player; i++)
    {
      IPs[i] = (char*) malloc (40 * sizeof (char));
    }
  
  while (!quit)
    {
      /* On lit les informations du tuyau où l’on reçoit les données à envoyer */
      /* Read read_pipe */

      buffer[0] = 0;
      read_pipe_until_null (buffer, BUFFER_SIZE, read_pipe);
      
      if (strlen (buffer) >= 1)
	{
	  printf ("tampon : %s\n", buffer);
	  if (strncmp (buffer, "QUIT", 5) == 0)
	    quit = 1;
	}
  
      /* Send what is from the pipe to the socket */
      
      /* Read the socket */
      /* Interpret */
      /* Send through the pipe if there is something to send */
    }

  close (read_pipe);
  close (write_pipe);
}

int
interpret_data_for_networking_process (char* data, size_t data_size)
{
  int ret = 0;
  /* On extrait la commande */
  /* We extract the command */
  char *token;
  token = strtok (data, " ");

  if (token == NULL)
    ret = -1;
  else
    {
      
    }

  return ret;
}
