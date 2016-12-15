//
// Created by wax on 12/15/16.
//

#include "lexer/TokenQueue.h"
#include "ast/BlockStmt.h"
#include "Parse.h"

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