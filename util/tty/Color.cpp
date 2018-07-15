#include "Color.h"

#include <sstream>

/**
 * Foreground color to ANSI escape code.
 */


/**
 * Background color to ANSI escape code.
 */


namespace TTY {

    std::string c(std::string s, Color fg, Color bg) {
	std::stringstream ss;
	ss << fgEscapeCode(fg);
	ss << bgEscapeCode(bg);
	ss << s;
	ss << ANSI_RST;
	
	return ss.str();
    }

    
    std::string fgEscapeCode(Color c) {
	static const std::string toANSI[] = {
	    "\x1B[30m",
	    "\x1B[31m",
	    "\x1B[32m",
	    "\x1B[33m",
	    "\x1B[34m",
	    "\x1B[35m",
	    "\x1B[36m",
	    "\x1B[37m"
	};
	
	if (c == NONE) return "";
	return toANSI[c];
    }

    std::string bgEscapeCode(Color c) {
	static const std::string toANSI[] = {
	    "\x1B[40m",
	    "\x1B[41m",
	    "\x1B[42m",
	    "\x1B[43m",
	    "\x1B[44m",
	    "\x1B[45m",
	    "\x1B[46m",
	    "\x1B[47m"
	};

	if (c == NONE) return "";
	return toANSI[c];
    }
}
