/**
 * @file
 * @author Corentin Bocquillon <0x539@nybble.fr>
 * @author Pierre Gillet <pierre.gillet+factorywars@linuxw.info>
 * @author Loup Fourment
 *
 * @section LICENSE
 *
 * Copyright (C) 2016 Corentin Bocquillon <0x539@nybble.fr>
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
 * along with factorywars.  If not, see <http://www.gnu.org/licenses/
 *
 * @section DESCRIPTION
 *
 * utils.h is the header of utils.cpp
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <dirent.h>

#include "structures.h"

/**
 * Find the line which described the given chunk in a file.
 * @param chunk_coordinates is the coordinates of the chunk.
 * @param dst is a pointer to the variable where the line will be stored.
 * @param dst_size is the size of dst.
 * @param file_path is the path of the file where we want to find the chunk line.
 * @return A pointer to the line or NULL if there is an error.
 */
char* find_chunk_line_in_file (struct coordinates chunk_coordinates, char* dst, size_t dst_size, const char* file_path);

/**
 * Find the line number of the line describing the given chunk.
 * @param chunk_coordinates is the chunk_coordinates
 * @param file_path is the path to the save file
 * @return the line number or -1 if there is an error. The line number begin at position zero for the first line.
 */
int find_line_number_using_chunk_coordinates (struct coordinates chunk_coordinates, const char* file_path);

/**
 * Translate a struct coordinates to a string.
 * @param coordinates is the struct coordinates to translate.
 * @param dst is the string where the coordinates string will be stored.
 * @param dst_size is the size of the string “dst”.
 * @return A pointer to the coordinates string.
 */
char* coordinates_to_string (struct coordinates coordinates, char* dst, size_t dst_size);

/**
 * Insert line in a file.
 * @param line is the line we want to insert.
 * @param line_size is the length of the line.
 * @param position is the line number where we want to insert the line. The first line position is position 0.
 * @param file_path is the file path.
 * @param replace the line? 0 for no, anything else otherwise.
 * @return 0 if there is an error, something else otherwise.
 */
int insert_line_in_file (char* line, int line_size, int position, const char* file_path, int replace);

/**
 * Write an entire file to a pipe.
 * @param file_path is the path to the file.
 * @param pipe is the pipe where we will write the file.
 */
void write_file_to_pipe (const char* file_path, int pipe);

/**
 * Write an array of char to the pipe.
 * @param pipe is the pipe’s file descriptor.
 * @param message is the array of char which will be written to the pipe.
 */
void write_to_pipe (int pipe, const char* message);

/**
 * Read a pipe until a null byte and store info in the buffer.
 *
 * @param buffer is the variable where data are stored.
 * @param buf_size is the size of the buffer.
 * @param pipe is the pipe we will read.
 * @return how many bytes has been read.
 */
int read_pipe_until_null (char* buffer, size_t buf_size, int pipe);

struct coordinates
get_coordinates_from_string (const char* coordinates_str);

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
 * + 5 is returned if it is a move command.
 * + 6 is returned if it is a new_player command.
 */
int get_command_type (const char* data);
