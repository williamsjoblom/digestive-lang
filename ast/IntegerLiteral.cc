#include "IntegerLiteral.hh"

#include "ast/type/Types.hh"
#include "genir/Expr.hh"
#include "ir/TACFun.hh"


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
