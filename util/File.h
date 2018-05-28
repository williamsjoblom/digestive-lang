#pragma once

#include <string>

/**
 * Read contents of text file.
 */
std::string readSourceFile(std::string path);


/**
 * Return true if 'path' is a readable file.
 */
bool isReadableFile(std::string path);
