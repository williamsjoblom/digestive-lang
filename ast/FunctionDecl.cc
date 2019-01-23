#include "FunctionDecl.hh"

#include <limits>

#include "genir/Function.hh"
#include "util/PrettyPrint.hh"

FunctionDecl::FunctionDecl(std::string identifier, std::vector<VariableDecl*>* parameters, BlockStmt* body,
                           DType returnType, bool dumpAssembly, bool dumpIr) : Decl(identifier),
                                                                               parameters{parameters},
                                                                               body{body},
                                                                               bHandleIndex{-1},
                                                                               codeSize{0},
                                                                               irId{UINT_MAX},
                                                                               returnType{returnType},
                                                                               dumpAssembly{dumpAssembly},
                                                                               dumpIr{dumpIr},
                                                                               baPrototype{nullptr} { }


FunctionDecl::~FunctionDecl() {
    for (VariableDecl* parameter : *parameters) delete parameter;
    delete parameters;
    delete body;
    delete baPrototype;
}


void FunctionDecl::analyze(Scope* scope) {
    scope->declare(this);
    Scope* innerScope { new Scope(scope, this) };

    for (VariableDecl* param : *this->parameters) {
        param->analyze(innerScope);
    }

    body->analyze(innerScope);
}


void FunctionDecl::dump(size_t indent) {
    printIndent(indent);
    std::cout << "func " << identifier << std::endl;
    body->dump(indent + 1);
}


void FunctionDecl::generate(TACFun* env) {
    Generate::function(env, this);
}


bool FunctionDecl::equals(const Node &other) const {
    const FunctionDecl* o { dynamic_cast<const FunctionDecl*>(&other) };
    if (o == nullptr) return false;

    return matchesSignature(*o) && *body == *o->body;
}


bool FunctionDecl::matchesSignature(const FunctionDecl &other) const {
    if (other.parameters->size() != parameters->size()) return false;
    
    for (unsigned int i { 0 }; i < parameters->size(); i++) {
        VariableDecl* parameter = (*parameters)[i];
        VariableDecl* otherParameter = (*other.parameters)[i];
        
        if (*parameter != *otherParameter) return false;
    }
    
    return Decl::equals(other);
}


int FunctionDecl::stackSize() {
    return sizeof(void*);
}


/**
 * Add single parameter to function prototype.
 */
int addParamToPrototype(DType type, FuncSignatureX* prototype) {
    int count { 0 };
    
    if (type.isPrimitive()) {
        prototype->addArg(TypeIdOf<int>::kTypeId);
        count++;
    } else if (type.isTuple()) {
	for (unsigned int i { 0 }; i < type.type.tuple->size(); i++) {
	    addParamToPrototype(type.type.tuple->at(i), prototype);
	    count++;
	}
    } else {
	assert(false); 	// Not implemented.
    }
    
    return count;
}


/**
 * Add parameters to function prototype according to calling convention.
 */
int addParamsToPrototype(std::vector<VariableDecl*>* params, FuncSignatureX* prototype) {
    int count { 0 };
    for (unsigned int i { 0 }; i < params->size(); i++) {
	count += addParamToPrototype(params->at(i)->type, prototype);
    }

    return count;
}


FuncSignatureX FunctionDecl::bCreatePrototype() {
    if (baPrototype == nullptr) {
        baPrototype = new FuncSignatureX(CallConv::kIdHostCDecl);
        baPrototype->setRet(TypeIdOf<int>::kTypeId);

	addParamsToPrototype(parameters, baPrototype);
    }

    return *baPrototype;
}



