//
// Created by wax on 1/31/17.
//

#include "BuiltIn.h"

#include <unistd.h>
#include <iostream>
#include <stdarg.h>

#include "Backtrace.h"
#include "ast/type/DType.h"


/**
 * Print primitive with type 't' at given address.
 */
void printTuplePrimitive(DType& t, void* ptr) {
    if (t.hasLabel()) {
	std::cout << t.label << ": ";
    }
    
    if (t.type.primitive == DPrimitiveKind::INTEGER) {
	switch(t.byteSize()) {
	case 1: BuiltIn::printPrimitive(*((int8_t*) ptr)); break;
	case 2: BuiltIn::printPrimitive(*((int16_t*) ptr)); break;
	case 4: BuiltIn::printPrimitive(*((int32_t*) ptr)); break;
	case 8: BuiltIn::printPrimitive(*((int64_t*) ptr)); break;
	default: std::cout << "?"; break;
	}
    } else if (t.type.primitive == DPrimitiveKind::NATURAL) {
	switch(t.byteSize()) {
	case 1: BuiltIn::printPrimitive(*((uint8_t*) ptr)); break;
	case 2: BuiltIn::printPrimitive(*((uint16_t*) ptr)); break;
	case 4: BuiltIn::printPrimitive(*((uint32_t*) ptr)); break;
	case 8: BuiltIn::printPrimitive(*((uint64_t*) ptr)); break;
	default: std::cout << "?"; break;
	}
    } else {
	std::cout << "?";
    }
}


namespace BuiltIn {
    template <typename T>
    void printPrimitive(T i) {
	std::cout << i;
    }

    template void printPrimitive(short);
    template void printPrimitive(int);
    template void printPrimitive(long);

    template void printPrimitive(char);
    template void printPrimitive(unsigned short);
    template void printPrimitive(unsigned int);
    template void printPrimitive(unsigned long);

    
    template <typename T>
    void plnPrimitive(T i) {
	printPrimitive(i);
	std::cout << std::endl;
	//usleep(250 * 1000);
    }
    
    template void plnPrimitive(short);
    template void plnPrimitive(int);
    template void plnPrimitive(long);

    template void plnPrimitive(char);
    template void plnPrimitive(unsigned short);
    template void plnPrimitive(unsigned int);
    template void plnPrimitive(unsigned long);

    
    void printTuple(DType& t, void* ptr) {
	if (t.isPrimitive()) {
	    printTuplePrimitive(t, ptr);
	} else if (t.isTuple()) {
	    std::vector<DType>* tupleTypes = t.type.tuple;

	    std::cout << "(";
	
	    uint64_t offset = 0;
	    for (int i = 0; i < tupleTypes->size(); i++) {
		DType innerType = tupleTypes->at(i);
		void* innerPtr = (char*)ptr + offset;
		if (innerType.ref)
		    innerPtr = *((void**) innerPtr);
	    
		printTuple(innerType, innerPtr);
	    
		offset += innerType.byteSize();

		if (i < tupleTypes->size() - 1)
		    std::cout << ", ";
	    }
	
	    std::cout << ")";
	}
    }
    

    void plnTuple(DType* type, void* ptr) {
	DType t = *type;
	printTuple(t, ptr);
    
	std::cout << std::endl;
    }
}

