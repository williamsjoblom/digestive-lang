#include <assert.h>
#include <iostream>

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
