#include "TACEnv.h"

#include <iostream>
#include <sstream>

#include "TACEnvOp.h"


TACEnv::TACEnv() { }


TACEnv::~TACEnv() {
    for (int i = 0; i < program.size(); i++)
	delete program[i];
    for (int i = 0; i < labels.size(); i++)
	delete labels[i];
    for (int i = 0; i < variables.size(); i++)
	delete variables[i];
}


TAC* TACEnv::add(TACC opcode, TACOp s0, TACOp s1, TACOp d) {
    TAC* tac = new TAC(opcode, s0, s1, d);
    program.push_back(tac);

    bindWaitingLabels(tac);
    
    return tac;
}


TACLabel* TACEnv::newLabel(std::string name) {
    TACLabel* label = new TACLabel();
    labels.push_back(label);

    label->id = labels.size() - 1;
    label->tac = nullptr;
    
    if (name.empty()) {
	std::stringstream ss;
	ss << "L#" << label->id;
	label->name = ss.str();
    } else {
	label->name = name;
    }
    
    return label;
}


TACLabel* TACEnv::label(int id) const {
    return labels[id];
}


void TACEnv::bindLabel(TACLabel* label) {
    waitingLabels.push_back(label);
}


void TACEnv::bindLabel(TACLabel* label, TAC* tac) {
    label->tac = tac;
}


TACVar* TACEnv::newVar(TACType& type, std::string name) {
    TACVar* var = new TACVar();
    variables.push_back(var);

    var->id = variables.size() - 1;
    var->type = type;
    
    if (name.empty()) {
	std::stringstream ss;
	ss << "V#" << var->id;
	var->name = ss.str();
    } else {
	var->name = name;
    }

    return var;
}


TACVar* TACEnv::var(int id) {
    return variables[id];
}


TACOp TACEnv::newImm(TACType& type, unsigned long value) {
    TACOp op(true);
    op.type = type;
    op.kind = TACOpKind::IMMEDIATE;
    op.data.immValue = value;

    return op;
}


void TACEnv::dump() {
    for (TAC* tac : program) {
	tac->dump(this);
	std::cout << std::endl;
    }
}


void TACEnv::bindWaitingLabels(TAC* tac) {
    for (TACLabel* label : waitingLabels)
	label->tac = tac;
    waitingLabels.clear();
}
