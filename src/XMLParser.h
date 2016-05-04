/**
 * @file
 * @author Corentin Bocquillon <0x539+factorywars@nybble.fr>
 *
 * @section LICENSE
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 *
 * XMLParser.h is the header of XMLParser.c.
 */

/**
 * Get the number of an element in a XML file.
 *
 * @param elementName is the element name.
 * @param filename is the XML's file name.
 * @return the number of element whose name is elementName, or if an error occured return -1.
 */
int getNbOfElement(const char *elementName, const char *filename);

/**
 * Get the value of an element.
 *
 * This function return the value of an element by its name, its number of occurence
 * and optionnaly by the value of a specified attribute.
 *
 * @param elementName is the name of the element.
 * @param no is the number of occurence, it starts at 0.
 * @param attributeName is the attribute name, it can be NULL.
 * @param attributeValue is the value of the attribute, it must be NULL if 
 * attributeName is NULL.
 * @param filename is the XML's file name.
 * @return the value of the element or if it does not NULL.
 */
char* getValueOfElement(const char *elementName, const int no,
			const char *attributeName, const char *attributeValue,
			const char *filename);
