#include "Hash.h"

#include <cstdarg>
#include <catch.hpp>
#include <initializer_list>
#include <iostream>

/**
 * Ported from Python's tuple hash implementation.
 */
hash_t orderedHash(std::initializer_list<int> args) {
    hash_t value = 0x345678;
    for (int arg : args)
	value = (1000003 * value) ^ arg;
    value ^= args.size();
    if (value == -1)
	value = -2;
    
    return value;
}


/****************************************************************
 * Unit tests.
 ****************************************************************/

TEST_CASE("ordered integer hashes") {
    hash_t h0;
    hash_t h1;

    h0 = orderedHash({0, 0});
    h1 = orderedHash({1, 0});
    REQUIRE(h0 != h1);

    h0 = orderedHash({0, 0});
    h1 = orderedHash({0, 1});
    REQUIRE(h0 != h1);
}
