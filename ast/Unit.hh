#pragma once

#include "BlockStmt.hh"
#include "FunctionDecl.hh"

class Unit : public Node {
public:
    BlockStmt* statements;
    std::vector<FunctionDecl*> functions;

    Unit(std::vector<Stmt*> statements, std::vector<FunctionDecl*> functions);
    ~Unit();

    void analyze(Scope* scope);
    virtual bool equals(const Node& other) const;
    void dump(size_t indent);

    unsigned int functionCount();
    std::string symbolFromPtr(void* ptr);
};
