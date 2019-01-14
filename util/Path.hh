#pragma once

#include <string>

/**
 * Root path (DGROOT environment variable).
 */
std::string rootDirPath();


/**
 * Is the DGROOT environment variable set?
 */
bool hasRootDirPath();


/**
 * Core grammar path.
 */
std::string coreBNFFilePath();


/**
 * Boot file path.
 */
std::string bootFilePath();
