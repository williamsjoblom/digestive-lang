#include "IntegerLiteral.h"

#include "ast/type/Types.h"
#include "genir/Expr.h"
#include "ir/TACFun.h"


IntegerLiteral::IntegerLiteral(int value) {
    this->value = value;
    this->type = I32_TYPE;
}


TACOp IntegerLiteral::generate(TACFun* env) {
    return Generate::integerLiteral(env, this);
}


bool IntegerLiteral::equals(const Node& other) const {
    const IntegerLiteral* o = dynamic_cast<const IntegerLiteral*>(&other);
    if (o == nullptr) return false;

    return o->value == value;
}


void IntegerLiteral::dump(size_t indent) {
    std::cout << value;
}
