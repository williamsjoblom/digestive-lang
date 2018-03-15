#pragma once

#include <functional>

typedef std::size_t hash_t;

/**
 * Order dependent hash of integers.
 */
template<int n>
hash_t orderedHash(int i...);


