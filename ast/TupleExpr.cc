#include "TupleExpr.hh"
#include "semantic/SemanticError.hh"
#include "ir/TACOp.hh"
#include "genir/Expr.hh"


TupleExpr::TupleExpr(std::vector<Expr *> values) {
    this->values = values;
}


TupleExpr::~TupleExpr() {
    for (Expr* v : values) delete v;
}


TACOp TupleExpr::generate(TACFun* fun) {
    return Generate::tuple(fun, this);
}


void TupleExpr::analyze(Scope *scope) {
    std::vector<DType>* types = new std::vector<DType>();
    
    for (Expr* v : values) {
        v->analyze(scope);
        types->push_back(v->type);
    }

    bool ref = true;
    type = DType(types, ref);
}


bool TupleExpr::equals(const Node &other) const {
    return Node::equals(other);
}


void TupleExpr::dump(size_t indent) {
    std::cout << "(";
    for (Expr* v : values) {
        v->dump(indent);
        std::cout << " ";
    }
    std::cout << ")";
}
