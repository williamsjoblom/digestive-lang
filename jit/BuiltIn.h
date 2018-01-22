#pragma once

/**
 * Forward declarations.
 */
class DType;


namespace BuiltIn {
    /**
     * Print primitive.
     */
    template <typename T>
    void printPrimitive(T i);

    
    /**
     * Print primitive followed by a line break.
     */
    template <typename T>
    void plnPrimitive(T i);

    
    /**
     * Print tuple.
     */
    void printTuple(DType* type, void* ptr);

    
    /**
     * Print tuple followed by a line break.
     */
    void plnTuple(DType* type, void* ptr);
}
