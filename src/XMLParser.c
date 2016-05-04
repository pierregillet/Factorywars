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
getValueOfElement (const char *elementName, const int no,
		   const char *attributeName, const char *attributeValue,
		   const char *filename)
{
  const xmlChar *value;
  int elementNumber = -1;
  int end_tag = 0;

  LIBXML_TEST_VERSION;

  xmlTextReaderPtr reader;

  reader = xmlReaderForFile (filename, NULL, 0);
  if (reader == NULL)
    return NULL;

  int same_or_null_attribute = 0;
  int same_element_name = 0;
  int success = 0;
  char* cur_attribute_name, *cur_element_name;

  while (xmlTextReaderRead (reader))
    {
      cur_attribute_name = (char*) xmlTextReaderGetAttribute
	(reader, (unsigned char*) attributeName);

      cur_element_name = (char*) xmlTextReaderConstName (reader);

      if (attributeValue == NULL)
	same_or_null_attribute = 1;
      else if (strcmp (attributeValue, cur_attribute_name ) == 0)
	same_or_null_attribute = 1;
      else
	same_or_null_attribute = 0;

      if (strcmp (elementName, cur_element_name) == 0)
	same_element_name = 1;
      else
	same_element_name = 0;

      if (same_element_name && same_or_null_attribute)
	{
	  if (!end_tag)	/* Is it the end-tag? */
	    {
	      ++elementNumber;
	      end_tag = 1;

	      /* Read one further */
	      xmlTextReaderRead (reader);
	    }
	  else
	    end_tag = 0;

	  if (elementNumber == no && xmlTextReaderNodeType (reader) == 3)
	    {
	      value = xmlTextReaderConstValue (reader);
	      success = 1;
	      break;
	    }
	}

    }

  xmlCleanupParser ();
  xmlMemoryDump ();

  value = (success) ? value : NULL;

  return (char*) value;
}
