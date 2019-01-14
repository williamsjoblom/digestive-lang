#pragma once

#include <string>

#include "Stmt.hh"
#include "Analyzable.hh"

class Decl : public Stmt {
public:
    std::string identifier;
    Decl(std::string identifier);

    virtual bool equals(const Node& other) const;
    virtual int stackSize() = 0;
};
