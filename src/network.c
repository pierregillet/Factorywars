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
send (const struct server_credentials server, const char* data)
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

  send (sockfd6, data, strlen (data) + 1, 0);
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
    send (servers[i], data);
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

  int count = 0;
  
  while (!quit)
    {
      /* On lit les informations du tuyau où l’on reçoit les données à envoyer */
      /* Read read_pipe */

      buffer[0] = 0;
      read_pipe_until_null (buffer, BUFFER_SIZE, read_pipe);
      command_type = get_command_type (buffer);
      
      switch (command_type)
	{
	case 0:
	  /* Send what is from the pipe to the socket */
	  broadcast (servers, number_of_players, buffer, strlen (buffer));
	  break;

	case 5:
	  /* Send what is from the pipe to the socket */
	  if (count == 5)
	    {
	      broadcast (servers, number_of_players, buffer, strlen (buffer));
	      count = 0;
	    }
	  else
	    count++;
	  break;

	case 1:
	  quit = 1;
	  break;
	}

      sleep (0.002);
      
      /* Read the socket */
      nread = read_socket (buffer, BUFFER_SIZE, sockfd6, &peer_addr,
			   &peer_addr_len);
      if (nread > 0)
	{
	  strncpy (tmp_string, buffer, BUFFER_SIZE);
	  command_type = get_command_type (tmp_string);
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

	  if (!connect_command (tmp_string, &number_of_players, servers,
				peer_addr, write_pipe))
	    servers = (struct server_credentials*)
	      realloc (servers,
		       number_of_players * sizeof (struct server_credentials));
	  break;

	case 5:
	  move_command (buffer, &number_of_players, servers, peer_addr, write_pipe);
	  break;

	case 6:
	  
	  break;

	default:
	  break;
	}
      if (number_of_players > 0)
	{
	  /* printf ("Nombre de joueurs : %d\n", number_of_players); */
	  /* for (unsigned int j = 0; j < number_of_players; j++) */
	  /*   { */
	  /*     printf ("j : %d\n", j); */
	  /*     printf ("IP : %s, port : %d, nom : %s\n", servers[j].IP, servers[j].port, servers[j].name); */
	  /*   } */
	}
    }

  close (read_pipe);
  close (write_pipe);
  free (servers);
}

int
connect_command (const char* data, unsigned int* number_of_servers,
		 struct server_credentials* servers,
		 struct sockaddr_storage peer_addr,
		 int write_pipe)
{
  char *token;
  int ret = 1;

  const int BUFFER_SIZE = strlen (data) + 128;
  
  char buffer[BUFFER_SIZE];
  strncpy (buffer, data, strlen (data) + 1);

  (*number_of_servers)++;

  token = strtok (buffer, " ");
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
      get_ip (servers[*number_of_servers - 1].IP,
	      peer_addr);

      /* Est-ce que ce nom est déjà présent ? */
      /* Does that name is already present? */
      for (unsigned int i = 0; i < *(number_of_servers) - 1; i++)
	{
	  if (strcmp (servers[*(number_of_servers) - 1].name, servers[i].name) == 0)
	    {
	      (*number_of_servers)--;
	      ret = 0;
	    }
	}
    }

  if (ret == 1)
    {
      /* On lui transfert un message de connexion */
      /* We send him a connect */
      snprintf (buffer, BUFFER_SIZE, "CONNECT %s %s",
		get_config_value ("port"), get_config_value ("name"));
      send (servers[*(number_of_servers) - 1], buffer);

      /* On transfert à tout le monde une commande nouveau joueur */
      /* We send to everyone a new player command */
      snprintf (buffer, BUFFER_SIZE, "NEWPLAYER %s %d", servers[*(number_of_servers) - 1].IP, servers[*(number_of_servers) - 1].port);
      broadcast (servers, *(number_of_servers) - 1, buffer, strlen (buffer));

      /* On doit enlever le port */
      /* We need to delete the port */
      snprintf (buffer, strlen (data), "CONNECT %s",
		servers[*(number_of_servers) - 1].name);

      write (write_pipe, buffer, strlen (buffer) + 1);
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

int
get_ip (char* IP, struct sockaddr_storage peer_addr)
{
  const char* ret;
  ret = inet_ntop (AF_INET6,
		   (const void*)((sockaddr_in6*)&peer_addr)->sin6_addr.s6_addr,
		   IP, INET6_ADDRSTRLEN);

  
  if (ret == NULL)
    return 0;
  else
    return 1;
}

int
is_connected (unsigned int* number_of_servers,
	      struct server_credentials* servers,
	      const char* IP)
{
  int ret = 0;
  for (unsigned int i = 0; i < *number_of_servers; i++)
    {
      if (strcmp (IP, servers[i].IP) == 0)
	ret = 1;
    }

  return ret;
}

void move_command (const char* data,
		   unsigned int* number_of_servers,
		   struct server_credentials* servers,
		   struct sockaddr_storage peer_addr,
		   int write_pipe)
{
  const int BUFFER_SIZE = 256;
  
  char IP[INET6_ADDRSTRLEN], buffer[BUFFER_SIZE], *token, tmp_str[BUFFER_SIZE];

  strncpy (tmp_str, data, BUFFER_SIZE);

  get_ip (IP, peer_addr);
  
  if (!is_connected (number_of_servers, servers, IP))
    return;

  int server_no = 0;

  for (unsigned int i = 0; i < *number_of_servers; i++)
    {
      if (strcmp (servers[i].IP, IP) == 0)
	server_no = i;
    }

  token = strtok (tmp_str, " ");

  snprintf (buffer, BUFFER_SIZE, "%s %s",
	    token, servers[server_no].name);

  token = strtok (NULL, " ");
  snprintf (buffer + strlen (buffer), BUFFER_SIZE, " %s", token);

  write (write_pipe, buffer, strlen (buffer) + 1);
}

void
shutdown_network_process (int write_pipe)
{
  char msg[] = "QUIT";
  write (write_pipe, msg, strlen (msg) + 1);
}

void
new_player_command (const char* data,
		    struct server_credentials* servers,
		    unsigned int* number_of_servers)
{
  /* NEWPLAYER */
  const int BUFFER_SIZE = strlen (data);

  char buffer[BUFFER_SIZE], IP[BUFFER_SIZE], port[BUFFER_SIZE], *token;

  strncpy (buffer, data, BUFFER_SIZE);
  
  token = strtok (buffer, " ");
  token = strtok (NULL, " ");

  strncpy (IP, token, BUFFER_SIZE);

  token = strtok (NULL, " ");
  strncpy (port, token, BUFFER_SIZE);

  struct server_credentials client;
  strncpy (client.IP, IP, strlen (data));
  client.port = (unsigned short) atoi (port);

  snprintf (buffer, BUFFER_SIZE, "CONNECT %s %s", IP, port);
  send (client, buffer);
}
