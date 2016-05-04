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
 * XMLParser.c parse XML files.
 */

#include <string.h>
#include <libxml/xmlreader.h>
#include <stdio.h>

int
getNbOfElement (const char *elementName, const char *filename)
{
  int nbOfElement = 0;

  LIBXML_TEST_VERSION;

  xmlTextReaderPtr reader;
  int ret;

  reader = xmlReaderForFile (filename, NULL, 0);
  if (reader == NULL)
    return -1;

  ret = xmlTextReaderRead (reader);
  while (ret == 1)
    {
      if (strcmp (elementName, (char*) xmlTextReaderConstName (reader)) == 0)
	{
	  ++nbOfElement;
	}
      ret = xmlTextReaderRead (reader);
    }

  xmlCleanupParser ();
  xmlMemoryDump ();

  return nbOfElement /= 2;
}

char*
getValueOfElement (const char *elementName, const int no, const char *attributeName, const char *attributeValue, const char *filename)
{
  const xmlChar *value;
  int elementNumber = -1;
  int close = 1;

  LIBXML_TEST_VERSION;

  xmlTextReaderPtr reader;
  int ret;

  reader = xmlReaderForFile (filename, NULL, 0);
  if (reader == NULL)
    return NULL;

  ret = xmlTextReaderRead (reader);
  while (ret == 1)
    {
      if (strcmp (elementName, (char* ) xmlTextReaderConstName (reader)) == 0)
	{
	  if ((attributeName == NULL
	       || strcmp (attributeValue, 
			  (char*) xmlTextReaderGetAttribute (reader, (const unsigned char*) attributeName)) == 0))
	    {
	      if (close)		/* Is it the end-tag? */
		{
		  ++elementNumber;
		  close=0;
		}
	      else
		close=1;
	      ret = xmlTextReaderRead (reader);
	      if (elementNumber == no && xmlTextReaderNodeType (reader) == 3)
		{
		  value = xmlTextReaderConstValue (reader);
		  break;
		}
	    }
	}
      ret = xmlTextReaderRead (reader);
    }

  xmlCleanupParser ();
  xmlMemoryDump ();
  return (char*) value;
}
