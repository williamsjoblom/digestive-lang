#include "Hash.h"

#include <cstdarg>


/**
 * Ported from Python's tuple hash implementation.
 */
template<int n>
hash_t orderedHash(int num...) {
    va_list args;
    va_start(args, num);

    hash_t value = 0x345678;
    for (int i = 0; i < n; i++)
	value = (1000003 * value) ^ va_arg(args, int);
    value ^= n;
    if (value == -1)
	value = -2;
    
    va_end(args);

    return value;
}


/**
 * Template instantiation.
 */
template hash_t orderedHash<2>(int...);
template hash_t orderedHash<3>(int...);
template hash_t orderedHash<4>(int...);
template hash_t orderedHash<5>(int...);
template hash_t orderedHash<6>(int...);
template hash_t orderedHash<7>(int...);
template hash_t orderedHash<8>(int...);





