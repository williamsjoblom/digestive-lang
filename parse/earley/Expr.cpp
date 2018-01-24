#include "Expr.h"

#include <string>
#include <sstream>
#include <vector>

#include "BNF.h"
#include "ast/Expr.h"
#include "lexer/TokenQueue.h"
#include "lexer/Token.h"



BNFGrammar buildGrammar() {
    
    
}


namespace Earley {
    Expr* parse(TokenQueue& tokens) {
	BNFGrammar g = buildGrammar();
    }
}
