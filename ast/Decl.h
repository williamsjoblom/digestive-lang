//
// Created by wax on 12/14/16.
//

#ifndef DIG_DECL_H
#define DIG_DECL_H

#include <string>
#include "Stmt.h"
#include "Analyzable.h"
#include "type/Type.h"

class Decl : public Stmt {
public:
    Type* type;

    std::string identifier;

    Decl(std::string identifier);

    virtual bool equals(const Node& other) const;
    virtual int stackSize() = 0;
};


#endif //DIG_DECL_H
