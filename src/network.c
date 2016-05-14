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
 * network.c contain the code for everything about the network
 */

#include "network.h"

int
send (const char *ip, unsigned short port, char* data)
{
  int sockfd6, n, s;
  char port_str[6];
  unsigned int length;
  struct sockaddr_in6 server, from;
  struct addrinfo hints;
  struct addrinfo *result, *rp;

  /* Convert port to a string */
  snprintf (port_str, 6, "%d", port);

  memset (&hints, 0, sizeof (struct addrinfo));
  hints.ai_family = AF_UNSPEC; // Allow IPv4 and IPv6
  hints.ai_socktype = SOCK_DGRAM; // Datagram socket
  hints.ai_flags = 0;
  hints.ai_protocol = 0; //Any protocol

  s = getaddrinfo (ip, port_str, &hints, &result);
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

  if (send (sockfd6, data, strlen (data), 0) != strlen (data))
    {
      fprintf (stderr, "Error while writing on the socket\n");
      return -1;
    }
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
      handle_network_communication;
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
}

void
handle_network_communication (unsigned short port, int read_pipe,
			      int write_pipe)
{
  int quit = 0;
  while (!quit)
    {
      /* Read the read_pipe */

      const int BUFFER_SIZE = 512;

      char buffer[BUFFER_SIZE];

      /* Non blocking read */
      /* int flags = fcntl (read_pipe, F_GETFL, 0); */
      /* fcntl (read_pipe, F_SETFL, flags | O_NONBLOCK); */

      for (int i = 0; i < BUFFER_SIZE - 1; i++)
	{
	  read (read_pipe, buffer + i, sizeof (char)) < 0
	  if (buffer[i] == '\0')
	    break;
	}
  
      /* Interpret */

      

      /* Send if there is something to send */
      /* Read the socket */
      /* Interpret */
      /* Send through the pipe if there is something to send */
    }
}
