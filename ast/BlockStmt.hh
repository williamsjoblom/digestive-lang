#pragma once

#include <vector>

#include "Stmt.hh"

class TACFun;

class BlockStmt : public Stmt {
public:
    BlockStmt(std::vector<Stmt*> statements);
    ~BlockStmt();

    void generate(TACFun* fun);
    
    void analyze(Scope* scope);
    virtual bool equals(const Node& other) const;

    void dump(size_t indent);

private:
    std::vector<Stmt*> statements;
};
