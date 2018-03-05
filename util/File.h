#pragma once

#include <string>

/**
 * Read contents of text file.
 */
std::string readSourceFile(std::string path);


/**
 * Return root path (DGROOT environment variable).
 */
std::string rootPath();
