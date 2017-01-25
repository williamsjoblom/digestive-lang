//
// Created by wax on 1/25/17.
//

#include <iostream>
#include <cstring>
#include <climits>
#include <unistd.h>

#include "IO.h"

namespace IO {
    std::ifstream in;
    std::ofstream out;

    std::string getWorkingDir();

    bool init() {

        std::string path = getWorkingDir();

        std::string inPath  = path + "/in";
        std::string outPath = path + "/out";

        std::cout << "Waiting for connection" << std::endl;
        out.open(outPath);
        in.open(inPath);

        if (in.fail()) {
            std::cout << "Could not open file 'in': " << std::strerror(errno) << std::endl;
            return false;
        }

        if (out.fail()) {
            std::cout << "Could not open file 'in': " << std::strerror(errno) << std::endl;
            return false;
        }

        std::cout << "Connected!" << std::endl;

        return true;
    }

    std::string getWorkingDir() {
        char buf[PATH_MAX];
        getcwd(buf, PATH_MAX);
        return std::string(buf);
    }

}

