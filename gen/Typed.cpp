//
// Created by wax on 2/24/17.
//

#include <ast/type/PrimitiveType.h>
#include "Typed.h"
#include "TypedVar.h"

namespace Generate {
    void typedMov(X86Compiler& c, TypedVar* o0, TypedVar* o1) {
        if (o0->var->isGp()) {
            if (o1->type->isInt()) {
                if(o0->var->getSize() > o1->var->getSize())
                    c.movzx(*o0->gpCast(), *o1->gpCast());
                else
                    c.mov(*o0->gpCast(), *o1->gpCast());
            } else if(o1->type->isFloat()) {
                X86GpVar casted = c.newGpVar(o0->var->getVarType());
                o1->gpCast(c, casted);
                c.mov(*o0->gpCast(), casted);
            } else assert(false);
        }
    }

    void mov(X86Compiler& c, X86Var* var, X86GpVar& result) {
        if (var->isGp()) {
            X86GpVar* v = dynamic_cast<X86GpVar*>(var);
            if (v == nullptr) assert(false);

            if(result.getSize() > var->getSize()) {
                c.movzx(result, *v);
                return;
            } else if (result.getSize() == var->getSize()) {
                c.mov(result, *v);
                return;
            } else {
                c.mov(result, *v);
                return;
            }
        } else if (var->isXmm()) {
            X86XmmVar* v = dynamic_cast<X86XmmVar*>(var);
            if (v == nullptr) assert(false);

            if (var->getSize() == 32) {
                c.cvttss2si(result, *v);
            } else if (var->getSize() == 64) {
                c.cvttsd2si(result, *v);
            }
        }
    }
}