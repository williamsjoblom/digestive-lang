//
// Created by wax on 2/24/17.
//

#include "TypedVar.h"
#include "ast/type/PrimitiveType.h"

TypedVar::TypedVar(X86Var* var, Type* type) {
    this->var = var;
    this->type = type;
}


X86GpVar* TypedVar::i8(X86Compiler& c) {
    const PrimitiveType* t = dynamic_cast<const PrimitiveType*>(type);
    if (t == nullptr) assert(false); // Invalid cast that should be caught in semantic analysis.

    if (t->kind == PrimitiveKind::INTEGER) {
        X86GpVar* v = dynamic_cast<X86GpVar*>(var);
        if (v == nullptr) assert(false);

        if (t->size == 8) {
            return new X86GpVar(*v);
        } else {
            return new X86GpVar(v->r8());
        }
    } else if (t->kind == PrimitiveKind::REAL) {
        X86XmmVar* v = dynamic_cast<X86XmmVar*>(var);
        if (v == nullptr) assert(false);

        if (t->size == 32) {
            X86GpVar result = c.newInt32();
            c.cvttss2si(result, *v);
            return new X86GpVar(result.r8());
        } else if (t->size == 64) {
            X86GpVar result = c.newInt32();
            c.cvttsd2si(result, *v);
            return new X86GpVar(result.r8());
        }


    }

    assert(false); // Unreachable
}

X86GpVar* TypedVar::i16(X86Compiler &c) {
    const PrimitiveType* t = dynamic_cast<const PrimitiveType*>(type);
    if (t == nullptr) assert(false); // Invalid cast that should be caught in semantic analysis.

    if (t->kind == PrimitiveKind::INTEGER) {
        X86GpVar* v = dynamic_cast<X86GpVar*>(var);
        if (v == nullptr) assert(false);

        if(t->size == 8) {
            X86GpVar* result = new X86GpVar(c.newInt16());
            c.movzx(*result, *v);
            return result;
        } else if (t->size == 16) {
            return new X86GpVar(*v);
        } else {
            return new X86GpVar(v->r16());
        }
    } else if (t->kind == PrimitiveKind::REAL) {
        X86XmmVar* v = dynamic_cast<X86XmmVar*>(var);
        if (v == nullptr) assert(false);

        if (t->size == 32) {
            X86GpVar result = c.newInt32();
            c.cvttss2si(result, *v);
            return new X86GpVar(result.r16());
        } else if (t->size == 64) {
            X86GpVar result = c.newInt32();
            c.cvttsd2si(result, *v);
            return new X86GpVar(result.r16());
        }
    }

    assert(false); // Unreachable
}

X86GpVar* TypedVar::i32(X86Compiler &c) {
    const PrimitiveType* t = dynamic_cast<const PrimitiveType*>(type);
    if (t == nullptr) assert(false); // Invalid cast that should be caught in semantic analysis.

    if (t->kind == PrimitiveKind::INTEGER) {
        X86GpVar* v = dynamic_cast<X86GpVar*>(var);
        if (v == nullptr) assert(false);

        if(t->size == 8) {
            X86GpVar* result = new X86GpVar(c.newInt32());
            c.movzx(*result, *v);
            return result;
        } else if (t->size == 16) {
            X86GpVar* result = new X86GpVar(c.newInt32());
            c.movzx(*result, *v);
            return result;
        } else if (t->size == 32) {
            return new X86GpVar(*v);
        } else {
            return new X86GpVar(v->r32());
        }
    } else if (t->kind == PrimitiveKind::REAL) {
        X86XmmVar* v = dynamic_cast<X86XmmVar*>(var);
        if (v == nullptr) assert(false);

        if (t->size == 32) {
            X86GpVar* result = new X86GpVar(c.newInt32());
            c.cvttss2si(*result, *v);
            return result;
        } else if (t->size == 64) {
            X86GpVar* result = new X86GpVar(c.newInt32());
            c.cvttsd2si(*result, *v);
            return result;
        }
    }

    assert(false); // Unreachable
}

X86GpVar* TypedVar::i64(X86Compiler &c) {
    const PrimitiveType* t = dynamic_cast<const PrimitiveType*>(type);
    if (t == nullptr) assert(false); // Invalid cast that should be caught in semantic analysis.

    if (t->kind == PrimitiveKind::INTEGER) {
        X86GpVar* v = dynamic_cast<X86GpVar*>(var);
        if (v == nullptr) assert(false);

        if(t->size == 8) {
            X86GpVar* result = new X86GpVar(c.newInt64());
            c.movzx(*result, *v);
            return result;
        } else if (t->size == 16) {
            X86GpVar* result = new X86GpVar(c.newInt64());
            c.movzx(*result, *v);
            return result;
        } else if (t->size == 32) {
            X86GpVar* result = new X86GpVar(c.newInt64());
            c.movzx(*result, *v);
            return result;
        } else if (t->size == 64) {
            return new X86GpVar(*v);
        } else {
            assert(false);
        }
    } else if (t->kind == PrimitiveKind::REAL) {
        X86XmmVar* v = dynamic_cast<X86XmmVar*>(var);
        if (v == nullptr) assert(false);

        if (t->size == 32) {
            X86GpVar* result = new X86GpVar(c.newInt64());
            c.cvttss2si(*result, *v);
            return result;
        } else if (t->size == 64) {
            X86GpVar* result = new X86GpVar(c.newInt64());
            c.cvttsd2si(*result, *v);
            return result;
        }
    }

    assert(false); // Unreachable
}

X86XmmVar* TypedVar::f32(X86Compiler &c) {
    const PrimitiveType* t = dynamic_cast<const PrimitiveType*>(type);
    if (t == nullptr) assert(false); // Invalid cast that should be caught in semantic analysis.

    if (t->kind == PrimitiveKind::INTEGER) {
        X86GpVar* v = dynamic_cast<X86GpVar*>(var);
        if (v == nullptr) assert(false);

        if (t->size < 32) {
            X86GpVar zx = c.newInt32();
            X86XmmVar* result = new X86XmmVar(c.newXmmSs());
            c.movzx(zx, *v);
            c.cvtsi2ss(*result, zx);
            return result;
        } else {
            X86XmmVar* result = new X86XmmVar(c.newXmmSs());
            c.cvtsi2ss(*result, *v);
        }
    } else if (t->kind == PrimitiveKind::REAL) {
        X86XmmVar* v = dynamic_cast<X86XmmVar*>(var);
        if (v == nullptr) assert(false);

        if (t->size == 32) {
            return new X86XmmVar(*v);
        } else if (t->size == 64) {
            X86XmmVar* result = new X86XmmVar(c.newXmmSs());
            c.cvtsd2ss(*result, *v);
            return result;
        }
    }

    assert(false); // Unreachable;
}

X86XmmVar* TypedVar::f64(X86Compiler &c) {
    const PrimitiveType* t = dynamic_cast<const PrimitiveType*>(type);
    if (t == nullptr) assert(false); // Invalid cast that should be caught in semantic analysis.

    if (t->kind == PrimitiveKind::INTEGER) {
        X86GpVar* v = dynamic_cast<X86GpVar*>(var);
        if (v == nullptr) assert(false);

        if (t->size < 32) {
            X86GpVar zx = c.newInt32();
            X86XmmVar* result = new X86XmmVar(c.newXmmSd());
            c.movzx(zx, *v);
            c.cvtsi2sd(*result, zx);
            return result;
        } else {
            X86XmmVar* result = new X86XmmVar(c.newXmmSd());
            c.cvtsi2sd(*result, *v);
        }
    } else if (t->kind == PrimitiveKind::REAL) {
        X86XmmVar* v = dynamic_cast<X86XmmVar*>(var);
        if (v == nullptr) assert(false);

        if (t->size == 32) {
            X86XmmVar* result = new X86XmmVar(c.newXmmSd());
            c.cvtss2sd(*result, *v);
            return result;
        } else if (t->size == 64) {
            return new X86XmmVar(*v);
        }
    }

    assert(false); // Unreachable;
}

X86Var* TypedVar::cast(Type* type) {
    const PrimitiveType* t = dynamic_cast<const PrimitiveType*>(type);
    if (t == nullptr) assert(false); // Casting not implemented for non primitive types.

    if (t->kind == PrimitiveKind::INTEGER) {
        X86GpVar* v = dynamic_cast<X86GpVar*>(type);
        if (v == nullptr) assert(false);

        if(t->size == 8) {
            X86GpVar* result = new X86GpVar(c.newInt64());
            c.movzx(*result, *v);
            return result;
        } else if (t->size == 16) {
            X86GpVar* result = new X86GpVar(c.newInt64());
            c.movzx(*result, *v);
            return result;
        } else if (t->size == 32) {
            X86GpVar* result = new X86GpVar(c.newInt64());
            c.movzx(*result, *v);
            return result;
        } else if (t->size == 64) {
            return new X86GpVar(*v);
        } else {
            assert(false);
        }
    } else if (t->kind == PrimitiveKind::REAL) {
        X86XmmVar* v = dynamic_cast<X86XmmVar*>(type);
        if (v == nullptr) assert(false);

        if (t->size == 32) {
            X86GpVar* result = new X86GpVar(c.newInt64());
            c.cvttss2si(*result, *v);
            return result;
        } else if (t->size == 64) {
            X86GpVar* result = new X86GpVar(c.newInt64());
            c.cvttsd2si(*result, *v);
            return result;
        }
    }

}

