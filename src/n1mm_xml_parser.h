/**
 * @file n1mm_xml_parser.h
 * @brief Header file for the XML parser for N1MMRotor configurations.
 * 
 * This header defines the structures and function prototypes for parsing
 * "sort-of" XML strings representing N1MMRotor configurations.
 * 
 * @author s54mtb, Marko Pavlin
 * @date January 2025
 * @version 1.0
 */

#ifndef ROTATOR_PARSER_H
#define ROTATOR_PARSER_H

#include <stddef.h>  // For size_t
#include "rotator.h"

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
int extract_xml_tag_value(const char *xml, const char *tag, char *value, size_t value_size);

/**
 * @brief Parse an XML string into an N1MMRotor structure.
 *
 * @param[in] xml The XML string to parse.
 * @param[out] rotor The N1MMRotor structure to populate.
 * @return 1 if parsing is successful, 0 otherwise.
 */
int parse_n1mmrotor_xml(const char *xml, N1MMRotor *rotor);

/**
 * @brief Parse an XML string for specific operations (e.g., <stop>).
 *
 * @param[in] xml The XML string to parse.
 * @param[out] rotor The N1MMRotor structure to populate (if applicable).
 * @return 1 if parsing is successful, 0 otherwise.
 */
int parse_special_xml(const char *xml, N1MMRotor *rotor);



#endif // ROTATOR_PARSER_H