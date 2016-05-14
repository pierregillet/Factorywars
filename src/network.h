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
/* #include <fcntl.h> */

/**
 * Send data
 * 
 * @param ip is the ip where the message will be sent.
 * @param port is the port where the server is listening.
 * @param data is the string which will be sent.
 * @return 0 if success, -1 if there is an error.
 */
int send (const char *ip, unsigned short port, char* data);

/**
 * Run the network process and create pipes to communicate with.
 *
 * @param port is the port the server will be listening on.
 * @param pipes is an integer array of four elements.
 * @return 1 if there is no error.
 */
int run_network_process (unsigned short port, int* pipes);

/**
 * This is the networking loop.
 *
 * @param port is the port the server will be listening on.
 * @param read_pipe is the pipe where we send information to send through the network.
 * @param write_pipe is the pipe where we send the information from the network.
 */
void handle_network_communication (unsigned short port, int read_pipe,
				   int write_pipe);
