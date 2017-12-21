#include "TACOp.h"

#include <sstream>
#include <assert.h>

#include "TACFun.h"
#include "TACProgram.h"

TACOp::TACOp(bool used) {
    ignore = !used;
    type = TACType(TACKind::PTR, 0);
}


TACOp::TACOp(TACLabel* label) {
    kind = LABEL;
    data.labelId = label->id;
    type = TACType(TACKind::PTR, sizeof(void*));
    ignore = false;
}


TACOp::TACOp(TACVar* var) {
    kind = VARIABLE;
    data.labelId = var->id;
    type = var->type;
    ignore = false;
}


TACOp::TACOp(FunctionDecl* fun) {
    kind = FUNCTION;
    data.functionId = fun->irId;
    type = TACType(TACKind::UNSIGNED, 4);
    ignore = false;
}


/**
 * To string.
 */
std::string TACOp::toS(TACFun* fun) const {
    if (ignore) return "_";
    
    std::stringstream ss;

    if (kind == IMMEDIATE) {
	ss << "(" << type.toS() << ")";
	ss << std::hex << data.immValue;
    } else if (kind == VARIABLE) {
	ss << "(" << type.toS() << ")";
	std::string name = fun->var(data.varId)->name;
	ss << name;
    } else if (kind == LABEL) {
	std::string name = fun->label(data.labelId)->name;
	ss << name;
    } else if (kind == FUNCTION) {
	TACFun* callee = fun->parent->funFromId(data.functionId);
	ss << UNDL(<< callee->identifier <<);
    }

    return ss.str();
}
