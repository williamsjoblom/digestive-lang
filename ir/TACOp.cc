#include "TACOp.hh"

#include <sstream>
#include <assert.h>

#include "TACFun.hh"
#include "TACProgram.hh"

TACOp::TACOp(bool used) {
    ignore = !used;
    type = TACType(TACKind::PTR, 0);
    offset = 0;
}


TACOp::TACOp(TACLabel* label) {
    kind = LABEL;
    data.labelId = label->id;
    type = TACType(TACKind::PTR, sizeof(void*));
    ignore = false;
    offset = 0;
}


TACOp::TACOp(TACVar* var) {
    kind = VARIABLE;
    data.labelId = var->id;
    type = var->type;
    ignore = false;
    offset = 0;
}


TACOp::TACOp(FunctionDecl* fun) {
    kind = FUNCTION;
    data.functionId = fun->irId;
    type = TACType(TACKind::UNSIGNED, 4);
    ignore = false;
    offset = 0;
}


/**
 * To string.
 */
std::string TACOp::toS(TACFun* fun) const {
    if (ignore) return "_";
    
    std::stringstream ss;

    if (type.ref) {
	ss << "&";
    }

    if (kind == IMMEDIATE) {
	ss << "(" << type.toS() << ")";
	ss << "0x" << std::uppercase << std::hex << data.immValue;
    } else if (kind == VARIABLE) {
	ss << "(" << type.toS() << ")";
	std::string name = fun->var(data.varId)->name;
	ss << name;
    } else if (kind == LABEL) {
	std::string name = fun->label(data.labelId)->name;
	ss << name;
    } else if (kind == FUNCTION) {
	TACFun* callee = fun->parent->fun(data.functionId);
	ss << UNDL(<< callee->identifier <<);
    }

    if (offset > 0) {
	ss << " + " << offset;
    } else if (offset < 0) {
	ss << " - " << -offset;
    }
    
    return ss.str();
}
