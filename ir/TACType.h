#pragma once

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
    
    bool isSigned();
    bool isUnsigned();

    /**
     * Return asmjit typeid of this type.
     */
    int asmjitId();

    std::string toS() const;
};
