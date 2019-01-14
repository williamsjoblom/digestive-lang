#include "lexer/TokenQueue.hh"
#include "ast/BlockStmt.hh"
#include "Parse.hh"

namespace Parse {
    BlockStmt* file(TokenQueue& tokens) {
        std::vector<Stmt*> statements;
        try {
            while (!tokens.empty()) {
                Stmt* statement = Parse::statement(tokens);
                statements.push_back(statement);
            }

            return new BlockStmt(statements);
        } catch(int e) {
            for (Stmt *stmt : statements)
                delete stmt;

            throw e;
        }
    }
}
