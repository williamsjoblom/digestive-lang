//
// Created by wax on 6/30/17.
//

#ifndef DIG_DTYPE_H
#define DIG_DTYPE_H

#include <vector>
#include <string>

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
    std::vector<DType>* tuple;
    const DType* seq;
};

/**
 * Type.
 */
class DType {
public:

    /**
     * Kind.
     */
    DKind kind;

    /**
     * Type data.
     */
    InnerType type;

    /**
     * Size.
     */
    int sz;

    /**
     * Is reference type?
     */
    bool ref;

    /**
     * Label.
     * Used to refer to this type when inside a named tuple.
     */
    std::string label;

    /**
     * Initialize to NIL-type.
     */
    DType();


    /**
     * Copy constructor.
     */
    DType(const DType& o);


    /**
     * Generic constructor.
     */
    DType(DKind kind, int sz, bool ref);


    /**
     * Primitive constructor.
     */
    DType(DPrimitiveKind primitiveKind, int sz, bool ref=false);

    
    /**
     * Tuple constructor.
     */
    DType(std::vector<DType>* tupleTypes, bool ref);

    /**
     * Destructor.
     */
    ~DType();

    int byteSize() const;

    inline bool isNilType() const { return kind == DKind::PRIMITIVE && type.primitive == DPrimitiveKind::NIL; }
    inline bool isPrimitive() const { return kind == DKind::PRIMITIVE; }
    inline bool isTuple() const { return kind == DKind::TUPLE; }
    inline bool isSeq() const { return kind == DKind::ARRAY; }
    inline bool isPtr() const { return isSeq(); }
    inline bool hasLabel() const { return label != ""; }
    void dump() const;

    void copy(const DType& o);

    DType operator=(const DType& o) { copy(o); return *this; }
    bool operator==(const DType& o);
    bool operator!=(const DType& o) { return !(*this == o); }
};


static DType NIL_TYPE(DPrimitiveKind::NIL, 0);

static DType I16_TYPE(DPrimitiveKind::INTEGER, 2);
static DType I32_TYPE(DPrimitiveKind::INTEGER, 4);
static DType I64_TYPE(DPrimitiveKind::INTEGER, 8);

static DType N8_TYPE(DPrimitiveKind::NATURAL, 1);
static DType N16_TYPE(DPrimitiveKind::NATURAL, 2);
static DType N32_TYPE(DPrimitiveKind::NATURAL, 4);
static DType N64_TYPE(DPrimitiveKind::NATURAL, 8);

#endif //DIG_DTYPE_H
