//
// Created by wax on 1/25/17.
//

#include <iostream>
#include <cstring>
#include <climits>
#include <unistd.h>
#include <limits.h>

#include "IO.h"

namespace IO {
    std::ifstream in;

    std::string getWorkingDir();

    bool init() {
        std::string path = getWorkingDir();
        std::string inPath  = path + "/in";

        in.open(inPath);

        if (in.fail()) {
            std::cout << "Could not open file 'in': " << std::strerror(errno) << std::endl;
            return false;
        }

        return true;
    }

    std::string getWorkingDir() {
        char buf[PATH_MAX];
        getcwd(buf, PATH_MAX);
        return std::string(buf);
    }

}

