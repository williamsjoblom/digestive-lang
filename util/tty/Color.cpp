#include "Color.h"


std::string colorToANSI[] = {
    "\x1B[31m",
    "\x1B[32m",
    "\x1B[33m",
    "\x1B[34m",
    "\x1B[35m",
    "\x1B[36m",
    "\x1B[37m"
};


namespace TTY {

    std::string c(std::string s, Color c) {
	return escapeCode(c) + s + ANSI_RST;
    }

    
    std::string escapeCode(Color c) {
	return colorToANSI[c];
    }
}
