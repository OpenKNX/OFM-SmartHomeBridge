#pragma once
#include <string>

/// @brief Converts a string from ISO 8859-15 to UTF-8 encoding.
/// @param c1252 The input string in ISO 8859-15 encoding.
/// @return if the converted string is different from the original, the string is alloceded in the heap
const char* convertISO8859_15ToUTF8(const char* c1252);

std::string convertISO8859_15ToUTF8_string(const char* c1252);