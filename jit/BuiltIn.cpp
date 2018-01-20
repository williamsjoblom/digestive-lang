//
// Created by wax on 1/31/17.
//

#include <unistd.h>
#include <iostream>
#include "BuiltIn.h"
#include "Backtrace.h"
#include <stdarg.h>


namespace BuiltIn {
    template <typename T>
    void pln(T i) {
        std::cout << i << std::endl;
	//usleep(250 * 1000);
    }

    template void pln(short);
    template void pln(int);
    template void pln(long);

    template void pln(char);
    template void pln(unsigned short);
    template void pln(unsigned int);
    template void pln(unsigned long);
}

