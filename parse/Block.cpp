//
// Created by wax on 12/15/16.
//

#include "lexer/TokenQueue.h"
#include "ast/BlockStmt.h"
#include "Parse.h"

namespace Parse {
    BlockStmt* block(TokenQueue& tokens) {
        if(!tokens.eat(LBRACK)) return nullptr;

        std::vector<Stmt*> statements;
        try {
            while (!tokens.eat(RBRACK)) {
                Stmt *statement = Parse::statement(tokens);
                statements.push_back(statement);
            }

            return new BlockStmt(statements);
        } catch(int e) {
            for (Stmt* stmt : statements)
                delete stmt;
            throw e;
        }
    }
}
