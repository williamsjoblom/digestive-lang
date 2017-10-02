//
// Created by wax on 1/23/17.
//

#include <asmjit/asmjit.h>

#include "ast/VariableDecl.h"

#include "ast/PlnStmt.h"
#include "jit/BuiltIn.h"
#include "gen/CallConv.h"

#include "Gen.h"

namespace Generate {

    void variableDeclaration(X86Compiler &c, VariableDecl* decl) {
        if (decl->value != nullptr) {
            decl->bVar = Generate::cast(c, decl->value, decl->type);
        } else {
            decl->bVar = Generate::typedRegister(c, decl->type);
        }

	// This is a reference type => move the newly generated values to the heap.
	/*if (decl->type.ref) {
	    X86Gp heapPtr = Generate::alloc(c, decl->type.byteSize());
	    X86Mem heapMem = x86::ptr(heapPtr);
	    
	    std::vector<DType> flatType = flattenType(decl->type);
	    int offset = 0;
	    for (int i = 0; i < flatType.size(); i++) {
		X86Gp reg = decl->bVar[i];
		
		heapMem.setOffset(offset);
		c.mov(heapMem, reg);
		
		offset += flatType[i].byteSize();
	    }

	    decl->bVar = { heapPtr };
	    }*/
	
        // Spill variables to stack
	//for (X86Gp var : decl->bVar) {
            //c.spill(var);

	    /*
	    c.mov(var, asmjit::Imm(12));

	    
	    VirtReg* vr = c.getVirtReg(var);
	    X86Mem mem = X86Mem(Init,
				c._nativeGpReg.getType(), vr->getId(),
				Reg::kRegNone, kInvalidValue,
				0, 0, Mem::kSignatureMemRegHomeFlag);
	    
	    X86Gp arg = c.newInt32();
	    c.mov(arg, mem);
	    
	    CCFuncCall* call = c.call(imm_ptr((void*) BuiltIn::pln<unsigned long>), FuncSignature1<void, short>(CallConv::kIdHost));
	    call->setArg(0, arg);

	    
	    std::cout << "Spilled to: " << mem.getAddrType() << std::endl;
	    */
	    //getVarMem(vr);
	//}
    }
}
