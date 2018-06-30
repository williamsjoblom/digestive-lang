#pragma once

#include <string>

#define ANSI_RST  "\x1B[0m"

namespace TTY {
    
    /**
     * Color.
     */
    enum Color {
	RED = 0,
	GREEN = 1,
	YELLOW = 2,
	BLUE = 3,
	MAGENTA = 4,
	CYAN = 5,
	WHITE = 6,
    };


    /**
     * Color given string with ANSI codes.
     */
    std::string c(std::string s, Color c);

    
    /**
     * Return ANSI escape code for given color.
     */
    inline std::string escapeCode(Color c);
}


