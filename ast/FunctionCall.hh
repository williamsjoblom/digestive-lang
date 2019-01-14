#pragma once

#include "Expr.hh"

class FunctionCall : public Expr {
public:
    std::string identifier;
    std::vector<Expr*>* arguments;
    FunctionDecl* declaration;

    FunctionCall(std::string identifier, std::vector<Expr*>* arguments);
    ~FunctionCall();

    TACOp generate(TACFun* fun);
    
    void analyze(Scope* scope);

    bool equals(const Node& other) const;

    void dump(size_t indent);
};
