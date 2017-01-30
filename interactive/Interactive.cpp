//
// Created by wax on 1/25/17.
//

#include <thread>
#include <iostream>

#include "Interactive.h"
#include "IO.h"

namespace Interactive {

    void evaluate(std::string cmd);

    void start() {
        std::thread t1(loop);
        t1.join();
    }

    void loop() {
        if (!IO::init()) {
            std::cout << "Failed to open pipe" << std::endl;
            return;
        }

        while (true) {
            std::string s;
            std::getline(IO::in, s);
            if (!s.empty()) {
                std::cout << "Read: " << s << std::endl;
            }

            if (IO::in.eof()) IO::in.clear();
        }
    }

    void evaluate(std::string cmd) {
        if (cmd == "update") {

        }
    }
}