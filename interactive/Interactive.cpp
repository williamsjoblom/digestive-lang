//
// Created by wax on 1/25/17.
//

#include <thread>
#include <iostream>
#include <jit/Jit.h>
#include <sstream>
#include <iterator>
#include <jit/JitContext.h>
#include <signal.h>
#include <execinfo.h>
#include <bits/siginfo.h>

#include "Interactive.h"
#include "IO.h"
#include "util/Colors.h"


namespace Interactive {

    void evaluate(Jit* jit, std::string cmd);

    void start(Jit* jit) {
        std::thread t1(loop, jit);
        t1.detach();
        //t1.join();
    }

    void loop(Jit* jit) {

        if (!IO::init()) {
            std::cout << "Failed to open interaction pipe" << std::endl;
            return;
        }

        bool status = false;

        while (true) {
            std::string s;

            std::getline(IO::in, s);

            evaluate(jit, s);

            if (IO::in.eof()) IO::in.clear();
        }
    }

    void evaluate(Jit* jit, std::string line) {
        std::istringstream buffer(line);
        std::vector<std::string> args((std::istream_iterator<std::string>(buffer)),
                                     std::istream_iterator<std::string>());

        if (args.size() < 1) return;

        std::cout << KRED;

        if (args[0] == "reload") {
            if (args.size() != 2) {
                std::cout << "SYNTAX: reload  [path to file]" << std::endl;
            } else {
                jit->reload(args[1]);
            }
        }

        std::cout << RST;
    }


}