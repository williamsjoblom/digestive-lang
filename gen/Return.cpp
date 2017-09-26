//
// Created by wax on 1/23/17.
//

#include "gen/Gen.h"

namespace Generate {
    void statement(X86Compiler &c, ReturnStmt* stmt) {
        Regs regs = Generate::cast(c, stmt->expression, stmt->returnType);

        if (stmt->returnType.isPrimitive()) {
            assert(regs.size() == 1);
            c.ret(regs[0]);
        } else if(stmt->returnType.isTuple()) {
	    int size = 0;
	    for (DType type : *stmt->returnType.type.tuple)
		size += type.byteSize();
	    
	    X86Mem returnStack = c.newStack(size, 4);
	    X86Gp returnPointer = c.newIntPtr("Return ptr");
	    c.lea(returnPointer, returnStack);

	    returnStack.setSize(1);

	    for (X86Gp reg : regs) {
		int sz = 4;
		c.mov(returnStack, reg);
		returnStack.addOffset(sz);
	    }

	    c.ret(returnPointer);
	    
	    /*
            int stackOffset = 0;
            for (X86Gp reg : regs) {
                c.mov(x86::ptr(x86::rsp, -(reg.getSize() + stackOffset)), reg);
                stackOffset += reg.getSize();
            }
	    */

            /*for (X86Gp reg : regs)
                c.push(reg);
                */
        } else {
            assert(false);
        }
    }
}
