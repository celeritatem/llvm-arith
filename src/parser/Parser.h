#ifndef PARSER_H
#define PARSER_H

// IOStream forbidden by LLVM guidelines:
// https://llvm.org/docs/CodingStandards.html#include-iostream-is-forbidden
#include "llvm/Support/raw_ostream.h"

#include "AST.h"
#include "lexer/Lexer.h"

class Parser {
public:
  Lexer &lexer;
  Token token;
  bool has_error = false;

  Parser(Lexer &lex) : lexer(lex) { advance(); }

  /// Main entry point to start parsing.
  AST *parse();

  // Parsing helpers:
  // ================
  void error() {
    llvm::errs() << "unexpected token: " << token.text << "\n";
    has_error = true;
  }

  /// Get the next token from the Lexer
  void advance() { lexer.next(token); }

  /// Emit error message if the next token is not what was expected
  bool expect(TokenTag tag) {
    if (token.tag != tag) {
      error();
      return true;
    }
    return false;
  }

  /// Consume the next token with the given tag (emit error instead if it has a
  /// different tag)
  bool consume(TokenTag tag) {
    if (expect(tag)) {
      return true;
    }
    advance();
    return false;
  }

  // Parse non-terminals:
  // ====================
  AST *parse_arith();
  Expr *parse_expr();
  Expr *parse_term();
  Expr *parse_factor();

  private:
  AST *advance_to_end();
};

#endif
