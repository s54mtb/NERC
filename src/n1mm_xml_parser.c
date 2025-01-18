/**
 * @file n1mm_xml_parser.c
 * @brief XML parser for N1MMRotor configurations.
 * 
 * This file provides a parser function for handling "sort-of" XML strings
 * that represent N1MMRotor configurations. The implementation supports
 * extensibility for future variations of the XML format.
 * 
 * @author S54MTB, Marko Pavlin
 * @date January 2025
 * @version 1.0
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rotator.h"
#include "n1mm_xml_parser.h"

/**
 * @brief Extract the value of a specific XML tag.
 * 
 * This function locates the specified tag in the XML string and extracts its value.
 * 
 * @param[in] xml The XML string.
 * @param[in] tag The tag to search for (e.g., "rotor").
 * @param[out] value The buffer to store the extracted value.
 * @param[in] value_size The size of the value buffer.
 * @return 1 if the tag is found and value is extracted, 0 otherwise.
 */
int extract_xml_tag_value(const char *xml, const char *tag, char *value, size_t value_size) {
  char open_tag[64], close_tag[64];
  snprintf(open_tag, sizeof(open_tag), "<%s>", tag);
  snprintf(close_tag, sizeof(close_tag), "</%s>", tag);

  const char *start = strstr(xml, open_tag);
  if (!start) {
    return 0;
  }
  start += strlen(open_tag);

  const char *end = strstr(start, close_tag);
  if (!end) {
    return 0;
  }

  size_t length = end - start;
  if (length >= value_size) {
    length = value_size - 1;
  }

  strncpy(value, start, length);
  value[length] = '\0';
  return 1;
}

/**
 * @brief Parse an XML string into an N1MMRotor structure.
 * 
 * This function parses an XML string representing an N1MMRotor configuration
 * and populates the provided N1MMRotor structure.
 * 
 * @param[in] xml The XML string to parse.
 * @param[out] rotor The N1MMRotor structure to populate.
 * @return 1 if parsing is successful, 0 otherwise.
 */
int parse_n1mmrotor_xml(const char *xml, N1MMRotor *rotor) {
  char buffer[128];

  // Initialize the structure with default values
  memset(rotor, 0, sizeof(N1MMRotor));

  // Parse <rotor> tag
  if (extract_xml_tag_value(xml, "rotor", buffer, sizeof(buffer))) {
    strncpy(rotor->rotor, buffer, sizeof(rotor->rotor) - 1);
  }

  // Parse <goazi> tag
  if (extract_xml_tag_value(xml, "goazi", buffer, sizeof(buffer))) {
    rotor->goazi = atof(buffer);
  }

  // Parse <offset> tag
  if (extract_xml_tag_value(xml, "offset", buffer, sizeof(buffer))) {
    rotor->offset = atof(buffer);
  }

  // Parse <bidirectional> tag
  if (extract_xml_tag_value(xml, "bidirectional", buffer, sizeof(buffer))) {
    rotor->bidirectional = atoi(buffer);
  }

  // Parse <freqband> tag
  if (extract_xml_tag_value(xml, "freqband", buffer, sizeof(buffer))) {
    rotor->freqband = atof(buffer);
  }

  return 1; // Parsing successful
}

/**
 * @brief Parse an XML string for specific operations (e.g., <stop>).
 * 
 * This function parses a specialized XML string with operation-specific tags
 * and processes accordingly.
 * 
 * @param[in] xml The XML string to parse.
 * @param[out] rotor The N1MMRotor structure to populate (if applicable).
 * @return 1 if parsing is successful, 0 otherwise.
 */
int parse_special_xml(const char *xml, N1MMRotor *rotor) {
  if (strstr(xml, "<stop>")) {
    char buffer[128];

    // Initialize the structure with default values
    memset(rotor, 0, sizeof(N1MMRotor));

    // Parse <rotor> tag inside <stop>
    if (extract_xml_tag_value(xml, "rotor", buffer, sizeof(buffer))) {
      strncpy(rotor->rotor, buffer, sizeof(rotor->rotor) - 1);
    }

    // Parse <freqband> tag inside <stop>
    if (extract_xml_tag_value(xml, "freqband", buffer, sizeof(buffer))) {
      rotor->freqband = atof(buffer);
    }

    return 1; // Parsing successful
  }

  return 0; // No recognized operation found
}




