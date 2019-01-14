#pragma once

#include <functional>

typedef std::size_t hash_t;

/**
 * Order dependent hash of integers.
 */
hash_t orderedHash(std::initializer_list<int> args);
