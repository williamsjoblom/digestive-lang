#include "TACProgram.h"


TACProgram::TACProgram() {
    functions = std::vector<TACFun*>();
    entry = nullptr;
}


TACProgram::~TACProgram() {
    for (TACFun* fun : functions)
	delete fun;

    if (entry != nullptr)
	delete entry;
}


TACFun* TACProgram::fun(int id) {    
    if (id == 0) return entry;
    return functions.at(id - 1);
}


void TACProgram::dump() {
    for (TACFun* fun : functions) {
	fun->dump();
	std::cout << std::endl;
    }

    entry->dump();
}
