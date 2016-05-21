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
 * network.h the header file of network.c
 */

#pragma once

#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ifaddrs.h>
#include "utils.h"
#include "config.h"

struct server_credentials
{
  char IP4[INET_ADDRSTRLEN], IP6[INET6_ADDRSTRLEN];
  unsigned short port;
};

/**
 * Send data
 * 
 * @param ip is the ip where the message will be sent.
 * @param port is the port where the server is listening.
 * @param data is the string which will be sent.
 * @param data_size is the number of bytes to send.
 * @return 1 if success, -1 if there is an error.
 */
void send (const struct server_credentials server, const char* data,
	   size_t data_size);

/**
 * Send data to everyone.
 *
 * @param servers is the list of servers.
 * @param data is the string which will be sent.
 * @param data_size is the number of bytes to send.
 */
void broadcast (const struct server_credentials *servers,
	       unsigned int number_of_servers, const char* data,
	       size_t data_size);

/**
 * Run the network process and create pipes to communicate with.
 *
 * @param port is the port the server will be listening on.
 * @param pipes is an integer array of four elements.
 * @return 1 if there is no error.
 */
int run_network_process (unsigned short port, int* pipes);

/**
 * Open a socket for a UDP server.
 *
 * @param port is the port where we will listen.
 * @return the socket or -1 if there is an error.
 */
int get_socket (unsigned short port);

/**
 * Read the socket.
 *
 * @param buffer is the buffer where we will store the informations.
 * @param buf_size is the size of the buffer.
 * @param sockfd6 is the socket we will read.
 * @return how many bytes has been read.
 */
int read_socket (char* buffer, size_t buf_size, int sockfd6);

/**
 * This is the networking loop.
 *
 * @param port is the port the server will be listening on.
 * @param read_pipe is the pipe where we receive the informations to send through the network.
 * @param write_pipe is the pipe where we send the information from the network.
 */
void handle_network_communication (unsigned short port, int read_pipe,
				   int write_pipe);

/**
 * Interpret given data and return a code depending on what actions needs
 * to be performed.
 *
 * @param data is the data to interpret.
 * @param data_size is the size of the data array.
 * @return 
 * + -1 on error and a value greater or equal than zero on success.
 * + 0 is returned if we just need to forward the message to the socket
 * or to the pipe.
 * + 1 is returned if we need to stop the process.
 * + 2 is returned if it is a ping.
 * + 3 is returned if it is a pong.
 * + 4 is returned if it is a connect command.
 */
int interpret_data_for_networking_process (char* data);

/**
 * Store in the servers’ crendentials array the new server credentials.
 *
 * @param data is the string containing the connect command and its parameters.
 * @param servers is the servers’ credentials array.
 * @return 0 if there is an error, 1 if success.
 */
int connect_command (const char* data, unsigned int* number_of_servers,
		     struct server_credentials* servers);
