//
// Created by wax on 6/30/17.
//

#ifndef DIG_DTYPE_H
#define DIG_DTYPE_H

#include <vector>

enum class DPrimitiveKind {
    NIL,
    INTEGER,
    NATURAL
};

enum class DKind {
    PRIMITIVE,
    TUPLE,
    ARRAY,
};

class DType;
union InnerType {
    DPrimitiveKind primitive;
    std::vector<const DType*>* tuple;
    const DType* seq;
};


class DType {
public:
    DKind kind;
    InnerType type;
    int sz;

    DType(const DType& o);
    DType(DKind kind, int sz);
    DType(DPrimitiveKind primitiveKind, int sz);
    ~DType();

    int byteSize() const;

    inline bool isPrimitive() const { return kind == DKind::PRIMITIVE; }
    inline bool isTuple() const { return kind == DKind::TUPLE; }
    inline bool isSeq() const { return kind == DKind::ARRAY; }
};


const DType NIL_TYPE = DType(DPrimitiveKind::NIL, 0);

const DType I16_TYPE = DType(DPrimitiveKind::INTEGER, 2);
const DType I32_TYPE = DType(DPrimitiveKind::INTEGER, 4);
const DType I64_TYPE = DType(DPrimitiveKind::INTEGER, 8);

const DType N8_TYPE  = DType(DPrimitiveKind::INTEGER, 1);
const DType N16_TYPE = DType(DPrimitiveKind::INTEGER, 2);
const DType N32_TYPE = DType(DPrimitiveKind::INTEGER, 4);
const DType N64_TYPE = DType(DPrimitiveKind::INTEGER, 8);


#endif //DIG_DTYPE_H
