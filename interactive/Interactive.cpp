//
// Created by wax on 1/25/17.
//

#include <thread>
#include <iostream>

#include "Interactive.h"
#include "IO.h"

namespace Interactive {

    void writePrompt();

    void start() {
        std::thread t1(loop);
        t1.join();
    }

    void loop() {
        IO::init();

        while (true) {
            writePrompt();
            std::string s;
            IO::in >> s;
            std::cout << "Read: " << s << std::endl;
        }
    }

    void writePrompt() {
        IO::out << "dig> ";
        //IO::out.flush();
    }
}