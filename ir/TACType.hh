#pragma once

#include <string>

#include "ast/type/DType.hh"

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
    bool ref;
    
    TACType();
    TACType(DType& type);
    TACType(TACKind kind, int byteSize, bool ref=false);

    bool isSigned();
    bool isUnsigned();

    /**
     * Return asmjit typeid of this type.
     */
    int asmjitId();

    std::string toS() const;
};
