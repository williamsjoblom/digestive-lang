
#include "File.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>

#include "globals.h"

std::string readSourceFile(std::string path) {
    std::string expandedPath = realpath(path.c_str(), nullptr);
    std::ifstream t(expandedPath);
    std::stringstream buffer;

    if (!t.is_open()) {
	if (verbose)
	    std::cout << "Failed to open file: " << path << std::endl;
	throw 1;
    }

    buffer << t.rdbuf();

    return buffer.str();
}


std::string rootPath() {
    char* cstr = getenv("DGROOT");
    if (cstr != nullptr)
	return std::string(cstr);
    else
	return "";
}
