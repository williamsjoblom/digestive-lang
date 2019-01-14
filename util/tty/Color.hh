#pragma once

#include <string>

#define ANSI_RST  "\x1B[0m"

namespace TTY {
    
    /**
     * Color.
     */
    enum Color {
	NONE = -1,
	BLACK = 0,
	RED = 1,
	GREEN = 2,
	YELLOW = 3,
	BLUE = 4,
	MAGENTA = 5,
	CYAN = 6,
	WHITE = 7,
    };


    /**
     * Color given string with ANSI codes.
     */
    std::string c(std::string s, Color fg, Color bg=Color::NONE);

    
    /**
     * Return ANSI escape code for given color.
     */
    inline std::string fgEscapeCode(Color c);

    
    /**
     * Return ANSI escape code for given color.
     */
    inline std::string bgEscapeCode(Color c);
}


