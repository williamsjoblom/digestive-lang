#include <assert.h>
#include <iostream>
#include <vector>

#include "ast/type/DType.h"

int neededRegisterCount(DType& type) {
    if (type.isTuple()) {
	int count = 0;
	
	std::vector<DType>* tupleTypes = type.type.tuple;
	for (int i = 0; i < tupleTypes->size(); i++) {
	    count += neededRegisterCount(tupleTypes->at(i));
	}

	return count;
    } else if (type.isPrimitive()) {
	return 1;
    } else {
	assert(false);
    }
}

std::vector<DType> flattenType(DType& type) {
    if (type.isPrimitive()) {
	return { type };
    } else if (type.isTuple()) {
	std::vector<DType> result;
	for (DType contained : *type.type.tuple) {
	    std::vector<DType> c = flattenType(contained);
	    result.insert(result.end(), c.begin(), c.end()); // Concat vectors
	}
	return result;
    } else {
	assert(false); // Not implemented
    }
}
