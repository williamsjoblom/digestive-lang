#include "Program.h"


void Generate::unit(TACProgram& program, Unit* unit) {
    int funId = 1;
    for (FunctionDecl* decl : unit->functions) {
	TACFun* funcEnv = new TACFun(&program, funId++, decl);
	decl->generate(funcEnv);

	program.functions.push_back(funcEnv);
    }

    TACFun* entryFun = new TACFun(&program, 0);
    unit->statements->generate(entryFun);
    entryFun->add(TACC::ret, entryFun->newImm<int>(0), TACOp(), TACOp());

    
    program.entry = entryFun;
}
