//
// Created by wax on 2/24/17.
//

#ifndef DIG_TYPEDVAR_H
#define DIG_TYPEDVAR_H

#include "Gen.h"

class TypedVar {
public:
    X86Var* var;
    Type* type;

    TypedVar(X86Var* var, Type* type);

    X86Var* cast(Type* type);

    X86GpVar* gpCast();
    void gpCast(X86Compiler& c, X86GpVar& result);

    X86XmmVar* xmmCast();
    void xmmCast(X86Compiler& c, X86XmmVar& result);

    X86GpVar* i8(X86Compiler& c);
    X86GpVar* i16(X86Compiler& c);
    X86GpVar* i32(X86Compiler& c);
    X86GpVar* i64(X86Compiler& c);

    X86XmmVar* f32(X86Compiler& c);
    X86XmmVar* f64(X86Compiler& c);
};


#endif //DIG_TYPEDVAR_H
