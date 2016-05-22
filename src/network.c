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

void
send (const struct server_credentials server, const char* data,
      size_t data_size)
{
  int sockfd6, s;
  char port_str[6];
  struct addrinfo hints;
  struct addrinfo *result, *rp;

  /* Convert port to a string */
  snprintf (port_str, 6, "%d", server.port);

  memset (&hints, 0, sizeof (struct addrinfo));
  hints.ai_family = AF_UNSPEC; // Allow IPv4 and IPv6
  hints.ai_socktype = SOCK_DGRAM; // Datagram socket
  hints.ai_flags = 0;
  hints.ai_protocol = 0; //Any protocol

    s = getaddrinfo (server.IP, port_str, &hints, &result);
  if (s != 0)
    {
      fprintf (stderr, "Getaddrinfo: %s\n", gai_strerror (s));
      return;
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
      return;
  }

  freeaddrinfo (result);

  if ((unsigned) send (sockfd6, data, data_size, 0) != data_size)
    {
      fprintf (stderr, "Error while writing on the socket\n");
      return;
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
broadcast (const struct server_credentials *servers,
	   unsigned int number_of_servers, const char* data, size_t data_size)
{
  for (unsigned int i = 0; i < number_of_servers; i++)
    send (servers[i], data, data_size);
}

void
handle_network_communication (unsigned short port, int read_pipe,
			      int write_pipe)
{
  /* Les constantes */
  /* Constants */
  const int BUFFER_SIZE = 512;
  
  int quit = 0;
  int command_type = 0;

  int sockfd6, nread;
  struct sockaddr_storage peer_addr;
  socklen_t peer_addr_len;

  port = (unsigned short) atoi(get_config_value ("port"));
  char buffer[BUFFER_SIZE], tmp_string[BUFFER_SIZE];

  /* On récupère l’interface de connexion */
  /* We get the socket */
  sockfd6 = get_socket (port);

  /* On crée le tableau qui contiendra les IP */
  /* We create the array that will contains the IPs */
  unsigned int number_of_players = 0;
  struct server_credentials *servers = NULL;
  
  while (!quit)
    {
      /* On lit les informations du tuyau où l’on reçoit les données à envoyer */
      /* Read read_pipe */

      buffer[0] = 0;
      read_pipe_until_null (buffer, BUFFER_SIZE, read_pipe);
      command_type = interpret_data_for_networking_process (buffer);
      
      switch (command_type)
	{
	case 0:
	  /* Send what is from the pipe to the socket */
	  broadcast (servers, number_of_players, buffer, strlen (buffer));
	  break;

	case 1:
	  quit = 1;
	  break;
	}
      
      /* Read the socket */
      nread = read_socket (buffer, BUFFER_SIZE, sockfd6, &peer_addr,
			   &peer_addr_len);
      if (nread > 0)
	{
	  strncpy (tmp_string, buffer, BUFFER_SIZE);
	  command_type = interpret_data_for_networking_process (tmp_string);
	  printf ("Buffer : %s, type de commande : %d\n", buffer, command_type);
	}
      else
	continue;

      switch (command_type)
	{
	case 0:
	  write (write_pipe, buffer, nread);
	  break;
	case 2:
	  sendto (sockfd6, "PONG", 5, 0, (struct sockaddr *) &peer_addr,
		  peer_addr_len);
	  break;
	case 3:
	  break;
	case 4:
	  strncpy (tmp_string, buffer, BUFFER_SIZE);

	  servers = (struct server_credentials*)
	    realloc (servers, (number_of_players + 1)
		     * sizeof (struct server_credentials));

	  if (!connect_command (tmp_string, &number_of_players, servers, peer_addr))
	    servers = (struct server_credentials*)
	      realloc (servers,
		       number_of_players * sizeof (struct server_credentials));
	  break;
	default:
	  break;
	}
      if (number_of_players > 0)
	{
	  printf ("Nombre de joueurs : %d\n", number_of_players);
	  for (int j = 0; j < number_of_players; j++)
	    {
	      printf ("j : %d\n", j);
	      printf ("IP : %s, port : %d, nom : %s\n", servers[j].IP, servers[j].port, servers[j].name);
	    }
	}
    }

  close (read_pipe);
  close (write_pipe);
  free (servers);
}

int
interpret_data_for_networking_process (char* data)
{
  int ret = 0;
  char *token = NULL;

  if (strlen (data) < 1)
    ret = -1;
  
  /* Il faut extraire la commande. */
  /* We need to extract the command. */
  if (ret != -1)
    token = strtok (data, " ");
  if (token == NULL)
    ret = -1;
  else if (strcmp (token, "QUIT") == 0)
    ret = 1;
  else if (strcmp (token, "PING") == 0)
    ret = 2;
  else if (strcmp (token, "PONG") == 0)
    ret = 3;
  else if (strcmp (token, "CONNECT") == 0)
    ret = 4;

  return ret;
}

int
connect_command (char* data, unsigned int* number_of_servers,
		 struct server_credentials* servers,
		 struct sockaddr_storage peer_addr)
{
  char *token;
  int ret = 1;

  (*number_of_servers)++;

  token = strtok (data, " ");
  token = strtok (NULL, " ");
  if (token != NULL)
    {
      servers[*number_of_servers - 1].port = (unsigned short) atoi (token);
    }
  else
    {
      (*number_of_servers)--;
      ret = 0;
    }
  
  if (ret != 0)
    {
      token = strtok (NULL, " ");
      if (token != NULL && strlen (token) < 127)
	strncpy (servers[*number_of_servers - 1].name, token, 128);
      else
	{
	  (*number_of_servers)--;
	  ret = 0;
	}
    }
  if (ret != 0)
    {
      inet_ntop (AF_INET6,
		 (const void*) ((sockaddr_in6*)&peer_addr)->sin6_addr.s6_addr,
		 servers[*number_of_servers - 1].IP, INET6_ADDRSTRLEN);

      /* Est-ce que ce nom est déjà présent ? */
      /* Does that name is already present? */
      for (int i = 0; i < *(number_of_servers) - 1; i++)
	{
	  if (strcmp (servers[*(number_of_servers) - 1].name, servers[i].name) == 0)
	    {
	      (*number_of_servers)--;
	      ret = 0;
	    }
	}
    }

  return ret;
}

int
get_socket (unsigned short port)
{
  int sockfd6, s;
  struct addrinfo hints, *result, *rp;

  memset (&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET6; // Allow IPv4 and IPv6
  hints.ai_socktype = SOCK_DGRAM; // Datagram socket
  hints.ai_flags = AI_PASSIVE;	  // For wildcard IP address
  hints.ai_protocol = 0; //Any protocol
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;

  char port_str[6];
  snprintf (port_str, 6, "%d", port);

  s = getaddrinfo (NULL, port_str, &hints, &result);
  if (s != 0)
    {
      exit (EXIT_FAILURE);
    }

  for (rp = result; rp != NULL; rp = rp->ai_next)
    {
      sockfd6 = socket (rp->ai_family, rp->ai_socktype, rp->ai_protocol);
      if(sockfd6 == -1)
	continue;
      
      if (bind (sockfd6, rp->ai_addr, rp->ai_addrlen) != -1)
	break; // Success

      close(sockfd6);
    }

  if (rp == NULL)
    {
      exit (EXIT_FAILURE);
    }

  freeaddrinfo (result);

  int flags = fcntl (sockfd6, F_GETFL, 0);
  fcntl (sockfd6, F_SETFL, flags | O_NONBLOCK);

  return sockfd6;
}

int
read_socket (char* buffer, size_t buf_size, int sockfd6,
	     struct sockaddr_storage* peer_addr, socklen_t* peer_addr_len)
{
  *peer_addr_len = sizeof (struct sockaddr_storage);
  int nread = 0;

  nread = recvfrom (sockfd6, buffer, buf_size, 0,
		    (struct sockaddr*) peer_addr, peer_addr_len);

  return nread;
}
