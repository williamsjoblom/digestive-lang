//
// Created by wax on 1/31/17.
//

#include <unistd.h>
#include <iostream>
#include "BuiltIn.h"
#include "Backtrace.h"

namespace BuiltIn {
    void pln(int i) {
        std::cout << i << std::endl;
        //Stack::ssd();
        usleep(500 * 1000); // Sleep 500ms between prints to ease testing.
    }
}