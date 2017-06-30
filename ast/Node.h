//
// Created by wax on 12/14/16.
//

#ifndef DIG_NODE_H
#define DIG_NODE_H

#include <iostream>
#include <assert.h>
#include "Analyzable.h"

class Node : public Analyzable {
public:
    virtual ~Node() { }

    bool operator==(const Node& other) const { return equals(other); }
    bool operator!=(const Node& other) const { return !equals(other); }
    virtual bool equals(const Node& other) const { assert(false); }

    virtual void dump(size_t indent) = 0;
    virtual void dump() { dump(0); }
};

#endif //DIG_NODE_H
