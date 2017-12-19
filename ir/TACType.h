#ifndef DIG_TACTYPE_H
#define DIG_TACTYPE_H

#include <string>

#include "ast/type/DType.h"

/**
 * Three address type kind.
 */
enum TACKind {
    PTR,
    SIGNED,
    UNSIGNED
};


/**
 * Three address operand type.
 */
class TACType {
public:
    TACKind kind;
    int byteSize;
    
    TACType();
    TACType(DType& type);
    TACType(TACKind kind, int byteSize);
    
    inline bool isSigned();
    inline bool isUnsigned();

    std::string toS() const;
};

#endif
