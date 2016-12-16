//
// Created by wax on 12/15/16.
//

#ifndef DIG_SCOPE_H
#define DIG_SCOPE_H


#include <map>
#include <set>

class FunctionDecl;
class VariableDecl;

class Scope {
public:
    Scope* parent;

    Scope();
    Scope(Scope* parent);

    ~Scope();

    void declare(VariableDecl* variableDecl);
    void declare(FunctionDecl* functionDecl);

    bool contains(std::string identifier);

    void addChild(Scope* scope);
    void removeChild(Scope* scope);

    VariableDecl* resolveVariable(std::string identifier);
    FunctionDecl* resolveFunction(std::string identifier);

    void dump() { dump(0); }
    void dump(size_t indent);

private:
    std::set<Scope*> children;

    std::map<std::string, VariableDecl*> variables;
    std::map<std::string, FunctionDecl*> functions;
};


#endif //DIG_SCOPE_H
