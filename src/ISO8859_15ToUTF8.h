#pragma once
#include "string.h"
#include <string>


/// @brief Converts a string from CP1252 to UTF-8 encoding.
/// @param c1252 The input string in CP1252 encoding.
/// @return if the converted string is different from the original, the string is alloceded in the heap
const char* convertISO8859_15ToUTF8(const char* cp1252);

std::string convertISO8859_15ToUTF8_string(const char* cp1252);