# BNF
The BNF format is used to describe the language grammar. The compiler ships with a core grammar describing digestive. This grammar can be extended, modified or completely replaced by the user.

```
unit = Expr                             @CompilationUnit;

Expr = <num>                            @Number
     | <id>                             @Var
     |  :group "(" Expr ")"
     || :right "-" Expr                 @NegExpr
     || :left Expr "*" Expr             @MulExpr
     |  :left Expr "/" Expr             @DivExpr
     || :left Expr "+" Expr             @AddExpr
     |  :left Expr "-" Expr             @SubExpr
```
_Example grammar for expression parsing._

## `@`-notation
The `@`-notation specifies that a production creates a node in abstract syntax tree.

The identifier following the `@` is name of the node to be created. After a successful parse the AST-construction function is called for every production that creates a node.

Said AST-construction function has the following signature:
```
fun construct(ASTNode parseNode) -> Node
```

#### `ASTNode` vs. `Node`
The parse tree returned from the parser will consist of `ASTNode`'s. These nodes are "stupid" in the sense that they just hold information about its tokens and child nodes.

The purpose of the AST-construction functions is to construct a new tree of `Node`'s from the `ASTNode`'s.

The `Nodes`'s on the other hand are smarter. Every kind of node (i.e. `Number`, `FunDecl` etc.) has its own class inheriting from `Node` (or one of its subclasses). These classes contain functionality for semantic analysis and IR code generation.

