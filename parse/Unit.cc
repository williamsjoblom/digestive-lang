#include "lexer/TokenQueue.hh"
#include "ast/Unit.hh"
#include "Parse.hh"


namespace Parse {
    Unit* unit(TokenQueue& tokens) {
        std::vector<Stmt*> statements;
        std::vector<FunctionDecl*> functions;

        try {
            while (!tokens.empty()) { // TODO clean up and make pretty
                FunctionDecl* func = Parse::function(tokens);
                if (func != nullptr) {
                    functions.push_back(func);
                } else {
                    Stmt *stmt = Parse::statement(tokens);
                    if (stmt != nullptr) statements.push_back(stmt);
                }
            }

            return new Unit(statements, functions);
        } catch(int e) {
	    // AST clean-up.
            for (Stmt* stmt : statements)
                delete stmt;

            for (FunctionDecl* func : functions)
                delete func;

            throw e;
        }
    }
}
