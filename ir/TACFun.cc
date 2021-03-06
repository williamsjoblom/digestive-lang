#include "TACFun.hh"

#include <iostream>
#include <sstream>
#include <type_traits>

#include "TACFunOp.hh"
#include "ast/FunctionDecl.hh"
#include "util/Colors.hh"
#include "TACProgram.hh"


TACFun::TACFun(TACProgram* parent, int id) {
    this->parent = parent;
    
    identifier = "#entry";
    this->id = id;

    parameters = std::vector<TACVar*>();
    returnType = TACType(TACKind::SIGNED, 4);
    instr = std::vector<TAC*>();
    labels = std::vector<TACLabel*>();
    variables = std::vector<TACVar*>();
}


TACFun::TACFun(TACProgram* parent, int id, FunctionDecl* decl) : TACFun(parent, id) {
    identifier = decl->identifier;

    for (VariableDecl* param : *decl->parameters) {
	TACType t = TACType(param->type);
	TACVar* var = newParam(t);
	param->irVar = var;
    }

    returnType = decl->returnType;
}


TACFun::~TACFun() {
    for (unsigned int i { 0 }; i < instr.size(); i++)
	delete instr[i];
    for (unsigned int i { 0 }; i < labels.size(); i++)
	delete labels[i];
    for (unsigned int i { 0 }; i < variables.size(); i++)
	delete variables[i];
}


TAC* TACFun::add(TACC opcode, TACOp s0, TACOp s1, TACOp d) {
    TAC* tac { new TAC(opcode, s0, s1, d) };
    instr.push_back(tac);

    bindWaitingLabels(tac);
    
    return tac;
}


TACLabel* TACFun::newLabel(std::string name) {
    TACLabel* label = new TACLabel();
    
    label->id = labels.size();
    label->tac = nullptr;
    
    if (name.empty()) {
	std::stringstream ss;
	ss << "L#" << label->id;
	label->name = ss.str();
    } else {
	label->name = name;
    }

    labels.push_back(label);
    
    return label;
}


TACLabel* TACFun::label(int id) const {
    return labels[id];
}


void TACFun::bindLabel(TACLabel* label) {
    waitingLabels.push_back(label);
}


void TACFun::bindLabel(TACLabel* label, TAC* tac) {
    if (tac->label != nullptr) {
	*label = *tac->label;
	label = tac->label;
    } else {
	tac->label = label;
    }
    
    label->tac = tac;
}


TACVar* TACFun::newVar(TACType& type, std::string name) {
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


TACVar* TACFun::newParam(TACType& type, std::string name) {
    if (name.empty()) {
	std::stringstream ss;
	ss << "P#" << parameters.size();
	name = ss.str();
    }
    
    TACVar* var = newVar(type, name);
    parameters.push_back(var);
    return var;
}


TACVar* TACFun::var(int id) {
    return variables[id];
}


TACOp TACFun::newImm(TACType& type, unsigned long value) {
    TACOp op(true);
    op.type = type;
    op.kind = TACOpKind::IMMEDIATE;
    op.data.immValue = value;

    return op;
}


/**
 * Construct generic TACType.
 */
template<typename T>
TACType toTACType() {
    TACKind kind;
    if (std::is_integral<T>::value) {
	if (std::is_signed<T>::value)
	    kind = TACKind::SIGNED;
	else
	    kind = TACKind::UNSIGNED;
    } else if (std::is_pointer<T>::value) {
	kind = TACKind::PTR;
    } else {
	assert(false);
    }

    return TACType(kind, sizeof(T));
}


template<typename T>
TACOp TACFun::newVar(std::string name) {
    TACType t = toTACType<T>();
    return newVar(t, name);
}


template<typename T>
TACOp TACFun::newImm(T value) {
    TACType t = toTACType<T>();
    return newImm(t, (unsigned long) value);
}


/**
 * Explicit template instansiation.
 */
template TACOp TACFun::newImm(char);
template TACOp TACFun::newImm(short);
template TACOp TACFun::newImm(int);
template TACOp TACFun::newImm(long);
template TACOp TACFun::newImm(unsigned char);
template TACOp TACFun::newImm(unsigned short);
template TACOp TACFun::newImm(unsigned int);
template TACOp TACFun::newImm(unsigned long);
template TACOp TACFun::newImm(void*);


template TACOp TACFun::newVar<char>(std::string);
template TACOp TACFun::newVar<short>(std::string);
template TACOp TACFun::newVar<int>(std::string);
template TACOp TACFun::newVar<long>(std::string);
template TACOp TACFun::newVar<unsigned char>(std::string);
template TACOp TACFun::newVar<unsigned short>(std::string);
template TACOp TACFun::newVar<unsigned int>(std::string);
template TACOp TACFun::newVar<unsigned long>(std::string);
template TACOp TACFun::newVar<void*>(std::string);


template TACType toTACType<char>();
template TACType toTACType<short>();
template TACType toTACType<int>();
template TACType toTACType<long>();
template TACType toTACType<unsigned char>();
template TACType toTACType<unsigned short>();
template TACType toTACType<unsigned int>();
template TACType toTACType<unsigned long>();
template TACType toTACType<void*>();


void TACFun::dump() {
    std::cout << UNDL(<< BOLD(<< identifier <<) <<)
	      << ":" << std::endl;
    
    for (TAC* tac : instr) {
	tac->dump(this, 1);
	std::cout << std::endl;
    }

    for (TACLabel* label : waitingLabels)
	label->dump(1);
}


void TACFun::bindWaitingLabels(TAC* tac) {
    for (TACLabel* label : waitingLabels)
	bindLabel(label, tac);
    waitingLabels.clear();
}
