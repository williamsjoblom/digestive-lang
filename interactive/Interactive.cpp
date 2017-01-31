//
// Created by wax on 1/25/17.
//

#include <thread>
#include <iostream>
#include <jit/Jit.h>

#include "Interactive.h"
#include "IO.h"

namespace Interactive {

    void evaluate(Jit* jit, std::string cmd);

    void start(Jit* jit) {
        std::thread t1(loop, jit);
        t1.detach();
        //t1.join();
    }

    void loop(Jit* jit) {
        if (!IO::init()) {
            std::cout << "Failed to open pipe" << std::endl;
            return;
        }

        while (true) {
            std::string s;
            std::getline(IO::in, s);
            if (!s.empty()) {
                evaluate(jit, s);
            }

            if (IO::in.eof()) IO::in.clear();
        }
    }

    void evaluate(Jit* jit, std::string cmd) {
        jit->reload("/home/wax/test2.dg");
    }
}